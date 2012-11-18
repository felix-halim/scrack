#include "crack.h"

int view_query(int a, int b){
  merge_ripple(ci, arr, N, pins, pdel, a, b);  // merge qualified updates

  int L1,R1; find_piece(ci, N, a, L1,R1);
  int L2,R2; find_piece(ci, N, b, L2,R2);
  int i1 = R1, i2 = L2;
  
  msize = 0;
  n_touched += R1 - L1;

  if (L1==L2){    // a and b are in the same piece
    assert(R1==R2);
    if (L1 < R1){
      #ifdef MIN_PCSZ
        if (R1 - L1 > MIN_PCSZ){
          mdd1r_split_and_materialize<2>(L1,R1, a,b);
        } else {
          return crack(a,b);
        }
      #else
        mdd1r_split_and_materialize<2>(L1,R1, a,b);
      #endif
    }
  } else {    // b and b are in different piece, order doesn't matter
    n_touched += R2 - L2;
    if (L1 < R1){
      #ifdef MIN_PCSZ
        if (R1 - L1 > MIN_PCSZ){
          mdd1r_split_and_materialize<0>(L1,R1, a,b);  // do the same algo on the first piece
        } else {
          add_crack(ci, N, a, i1 = partition(arr, a, L1, R1));  // 2-split
        }
      #else
        mdd1r_split_and_materialize<0>(L1,R1, a,b);  // do the same algo on the first piece
      #endif
    }
    if (L2 < R2){
      #ifdef MIN_PCSZ
        if (R2 - L2 > MIN_PCSZ){
          mdd1r_split_and_materialize<1>(L2,R2, a,b);  // do the same algo on the second piece
        } else {
          add_crack(ci, N, b, i2 = partition(arr, b, L2, R2));  // 2-split
        }
      #else
        mdd1r_split_and_materialize<1>(L2,R2, a,b);  // do the same algo on the second piece
      #endif
    }
  }
  return msize + max(0, i2 - i1);
}

int count_query(int a, int b){
  return view_query(a,b);
}
