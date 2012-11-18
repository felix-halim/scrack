#include "tester.h"       // require implementations of init,insert,remove,query
#include "crackers.h"

multiset<int> pins, pdel; // pending updates (insert / delete)
int *arr, N;              // the dataset array
int *marr, msize;         // for materialization
ci_type ci;               // the cracker index

void init(int *a, int n, int cap){
  ci.clear();
  msize = 0;
  N = n;
  marr = new int[cap];
  arr = new int[cap];     // for updates expansion
  for (int i=0; i<N; i++) arr[i] = a[i];  // copy all
}

void insert(int v){
  if (pdel.count(v)){
    pdel.erase(pdel.lower_bound(v));    // don't insert if exists in pdel
  } else {
    pins.insert(v);
  }
}

void remove(int v){
  if (pins.count(v)){
    pins.erase(pins.lower_bound(v));    // don't delete if exists in pins
  } else {
    pdel.insert(v);
  }
}

// MDD1R : Materialized DD1R
template <int CHECK>
int mdd1r_split_and_materialize(int L, int R, value_type a, value_type b){
  value_type X = arr[L + rand()%(R-L)];      // pick a pivot value X randomly in index [L,R)

  // optimization based on where [a,b] relative to X
  if (b < X){
    if (CHECK==0){                  // left check
      for (R--; L<=R; ){            // split [L,R) based on X and materialize
        while (L<=R && arr[L] < X){
          if (a<=arr[L]) marr[msize++] = (arr[L]);  // check left only
          L++;
        }
        while (L<=R && arr[R] >= X) R--;  // the arr[R] don't need to be checked for materialization!
        if (L<R) swap(arr[L],arr[R]);
      }
    } else if (CHECK==1){           // right check
      for (R--; L<=R; ){            // split [L,R) based on X and materialize
        while (L<=R && arr[L] < X){
          if (arr[L]<b) marr[msize++] = (arr[L]);  // check right only
          L++;
        }
        while (L<=R && arr[R] >= X) R--;  // the arr[R] don't need to be checked for materialization!
        if (L<R) swap(arr[L],arr[R]);
      }
    } else {                        // both checks
      for (R--; L<=R; ){            // split [L,R) based on X and materialize
        while (L<=R && arr[L] < X){
          if (a<=arr[L] && arr[L]<b) marr[msize++] = (arr[L]);  // check both and materialize
          L++;
        }
        while (L<=R && arr[R] >= X) R--;  // the arr[R] don't need to be checked for materialization!
        if (L<R) swap(arr[L],arr[R]);
      }
    }
  } else if (X <= a){
    if (CHECK==0){
      for (R--; L<=R; ){            // split [L,R) based on X and materialize
        while (L<=R && arr[L] < X) L++;    // the arr[L] don't need to be checked for materialization!
        while (L<=R && arr[R] >= X){
          if (a<=arr[R]) marr[msize++] = (arr[R]);  // check left
          R--;
        }
        if (L<R) swap(arr[L],arr[R]);
      }
    } else if (CHECK==1){
      for (R--; L<=R; ){            // split [L,R) based on X and materialize
        while (L<=R && arr[L] < X) L++;    // the arr[L] don't need to be checked for materialization!
        while (L<=R && arr[R] >= X){
          if (arr[R]<b) marr[msize++] = (arr[R]);  // check right
          R--;
        }
        if (L<R) swap(arr[L],arr[R]);
      }
    } else {
      for (R--; L<=R; ){            // split [L,R) based on X and materialize
        while (L<=R && arr[L] < X) L++;    // the arr[L] don't need to be checked for materialization!
        while (L<=R && arr[R] >= X){
          if (a<=arr[R] && arr[R]<b) marr[msize++] = (arr[R]);  // check both
          R--;
        }
        if (L<R) swap(arr[L],arr[R]);
      }
    }
  } else {
    for (R--; L<=R; ){              // split [L,R) based on X and materialize
      while (L<=R && arr[L] < X){
        if (a<=arr[L]) marr[msize++] = (arr[L]);  // materialize
        L++;
      }
      while (L<=R && arr[R] >= X){
        if (arr[R]<b) marr[msize++] = (arr[R]);  // materialize
        R--;
      }
      if (L<R) swap(arr[L],arr[R]);
    }
  }
  return add_crack(ci, N, X, L);          // add crack on random value X
}

int crack(value_type a, value_type b){
  merge_ripple(ci, arr, N, pins, pdel, a, b);  // merge qualified updates

  tree_t.start();
  int L1, R1, i1; find_piece(ci, N, a, L1, R1);
  int L2, R2, i2; find_piece(ci, N, b, L2, R2);
  tree_t.stop();

  n_touched += R1 - L1;   // examine the left tuple

  crack_t.start();
  if (L1==L2){            // a and b is on the same piece
    assert(R1 == R2);
    split_ab(arr, L1,R1, a, b, i1, i2);  // 3-split in one scan 
  } else {                // a and b is on different piece
    n_touched += R2 - L2; // examine the right piece
    i1 = partition(arr, a, L1, R1);
    i2 = partition(arr, b, L2, R2);
  }
  crack_t.stop();

  tree_t.start();
  add_crack(ci, N, a, i1);
  add_crack(ci, N, b, i2);
  tree_t.stop();

  return i2 - i1;    // return number of qualified tuples
}

// do random crack on the target piece, then materialize
int random_crack_and_materialize(value_type a, value_type b){  
  merge_ripple(ci, arr, N, pins, pdel, a, b);  // merge qualified updates
  int L1,R1,i1; find_piece(ci, N, a, L1,R1);
  int L2,R2,i2; find_piece(ci, N, b, L2,R2);
  assert(L1 != L2 || R1 == R2);
  n_touched += R1 - L1;
  if (L1 < R1){
    value_type X1 = arr[L1 + rand()%(R1-L1)];
    add_crack(ci, N, X1, partition(arr,X1,L1,R1));
  }
  if (L1 != L2){
    n_touched += R2 - L2;
    if (L2 < R2){
      value_type X2 = arr[L2 + rand()%(R2-L2)];
      add_crack(ci, N, X2, partition(arr,X2,L2,R2));
    }
  }
  msize = 0;
  materialize_it(ci, arr, N, a, b, i1, i2, marr, msize);
  return msize + max(0, i2 - i1);
}
