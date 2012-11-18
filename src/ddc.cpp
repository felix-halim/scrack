#include "crack.h"

int ddc_find(value_type v, int ncrack, int crack_at){    // n is the number of cracks allowed
  int L,R; find_piece(ci, N, v, L, R);
  n_touched += R - L;                   // the number of examined tuples
  while (ncrack-- > 0 && R - L > crack_at){      // split if the piece size is > CRACK_AT
    int M = (L+R)/2;                    // split exactly in half
    nth_element(arr+L, arr+M, arr+R);   // split it using order statistics
    add_crack(ci, N, arr[M], M);        // add crack on the middle
    if (v < arr[M]) R = M; else L = M;  // go to the correct sub-piece
  }
  return add_crack(ci, N, v, partition(arr, v, L,R));  // crack again on v
}

int ddc_query(int a, int b, int ncrack, int crack_at){
  int i2 = ddc_find(b, ncrack, crack_at);  // unlimited cracks allowed plus one crack on v2
  int i1 = ddc_find(a, ncrack, crack_at);  // unlimited cracks allowed plus one crack on v1
  return i2 - i1;                          // not materialized
}

int view_query(int a, int b){
  return ddc_query(a,b,MAX_NCRACK,CRACK_AT);
}

int count_query(int a, int b){
  return ddc_query(a,b,MAX_NCRACK,CRACK_AT);
}
