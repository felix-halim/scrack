#include "crack.h"

map<int, pair<value_type,pair<int,int> > > partial_crack;

// Partial MDD1R : Materialize DD1R (Partial)
bool mdd1rp_split_and_materialize(pair<value_type,pair<int,int> > &P, int L, int R, value_type a, value_type b, int nswap){
  value_type X = P.first;
  int &pL = P.second.first, &pR = P.second.second;

  // optimization based on where [a,b] relative to X
  if (b < X){
    n_touched -= R-pR;
    assert(pR<=R);
    R = pR; // skip the right section

    // fast scan the left
    while (L<pL){
      if (a<=arr[L] && arr[L]<b) marr[msize++] = (arr[L]);  // materialize
      L++;
    }

    for (; L<=R; ){        // split [L,R) based on X and materialize
      while (L<=R && arr[L] < X){
        if (a<=arr[L] && arr[L]<b) marr[msize++] = (arr[L]);  // materialize
        L++;
      }
      while (L<=R && arr[R] >= X) R--;  // the arr[R] don't need to be checked for materialization!
      if (L<R){
        swap(arr[L],arr[R]);
        if (nswap-- <= 0) break;
      }
    }
  } else if (X <= a){
    n_touched -= pL-L;
    assert(L<=pL);
    L = pL; // skip the left section

    // fast scan the right
    while (pR<R){
      if (a<=arr[R] && arr[R]<b) marr[msize++] = (arr[R]);  // materialize
      R--;
    }

    for (; L<=R; ){        // split [L,R) based on X and materialize
      while (L<=R && arr[L] < X) L++;    // the arr[L] don't need to be checked for materialization!
      while (L<=R && arr[R] >= X){
        if (a<=arr[R] && arr[R]<b) marr[msize++] = (arr[R]);  // materialize
        R--;
      }
      if (L<R){
        swap(arr[L],arr[R]);
        if (nswap-- <= 0) break;
      }
    }
  } else {
    // fast scan left and right
    while (L<pL){
      if (a<=arr[L] && arr[L]<b) marr[msize++] = (arr[L]);  // materialize
      L++;
    }
    while (pR<R){
      if (a<=arr[R] && arr[R]<b) marr[msize++] = (arr[R]);  // materialize
      R--;
    }

    for (; L<=R; ){        // split [L,R) based on X and materialize
      while (L<=R && arr[L] < X){
        if (a<=arr[L]) marr[msize++] = (arr[L]);  // materialize
        L++;
      }
      while (L<=R && arr[R] >= X){
        if (arr[R]<b) marr[msize++] = (arr[R]);  // materialize
        R--;
      }
      if (L<R){
        swap(arr[L],arr[R]);
        if (nswap-- <= 0) break;
      }
    }
  }
  pL = L; pR = R;
  if (L>R){
    add_crack(ci, N, X, L);  // full crack on random value X
    return true;
  }
  while (L<=R){
    if (arr[L]>=a && arr[L]<b) marr[msize++] = (arr[L]);
    L++;
  }
  return false;
}

template <int CHECK>
void mdd1rp_find(int L, int R, value_type a, value_type b, int nswap){
  if (R-L < 1000000){ // full crack if the piece size is less than 1M tuples
    mdd1r_split_and_materialize<CHECK>(L,R,a,b);
    return;
  }
  if (!partial_crack.count(L)){ // pick a pivot value X randomly in index [L,R)
    partial_crack[L] = make_pair(arr[L + rand()%(R-L)], make_pair(L,R-1)); 
  }
  if (mdd1rp_split_and_materialize(partial_crack[L], L, R-1, a,b, nswap)){
    assert(partial_crack.count(L));
    partial_crack.erase(L);  // remove from partial crack
  }
}

int mdd1rp(value_type a, value_type b, int p){
//  merge_ripple(ci, arr, N, pins, pdel, a, b);  // merge qualified updates
  int L1,R1; find_piece(ci, N, a, L1,R1);
  int L2,R2; find_piece(ci, N, b, L2,R2);
  msize = 0;
  n_touched += R1 - L1;
  if (L1==L2){
    assert(R1==R2);
    if (L1 < R1) mdd1rp_find<2>(L1,R1,a,b,N*p/100);
  } else {
    n_touched += R2 - L2;
    if (L1 < R1) mdd1rp_find<0>(L1,R1,a,b,N*p/100);
    if (L2 < R2) mdd1rp_find<1>(L2,R2,a,b,N*p/100);
  }
  int i1 = R1;
  int i2 = L2;
  return msize + max(0, i2 - i1);
}

int view_query(int a, int b){
  return mdd1rp(a,b,PERCENTAGE);
}

int count_query(int a, int b){
  return view_query(a,b);
}
