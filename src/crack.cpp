#include "crack.h"      // require implementations of "query"

extern int n_cracks;
Random rr;

void naive_random_crack(){
  value_type x = arr[rr.nextInt(N)];
  int L,R; find_piece(ci, N, x,L,R);
  n_touched += R-L;
  add_crack(ci, N, x, partition(arr, x,L,R));
}

int view_query(int a, int b){
  #ifdef RANDOM_CRACK_PER_QUERY
    for (int i=0; i<RANDOM_CRACK_PER_QUERY; i++)
      naive_random_crack();
  #endif
  
  #ifdef RANDOM_CRACK_EVERY_NTH_QUERY
    static int nth = 0;
    if (++nth % RANDOM_CRACK_EVERY_NTH_QUERY == 0)
      naive_random_crack();
  #endif

  int cnt = crack(a,b);
  n_cracks += ci.size();
  return cnt;
}

int count_query(int a, int b){
  view_query(a,b);

  int L=0, cnt=0;
  ci_iter it1, it2;
  if (ci.count(a)){
    it1 = ci.lower_bound(a);
    L = it1->second.pos;
  } else {
    int L1,R1;
    it1 = find_piece(ci, N, a, L1, R1);
    for (int i=L1; i<R1; i++)
      if (arr[i] >= a && arr[i] < b) cnt++;
    L = R1;
    it1++;
  }
  assert(it1 != ci.end());
  it1++;

  if (ci.count(b)){
    it2 = ci.lower_bound(b);
  } else {
    int L2, R2;
    it2 = find_piece(ci, N, b, L2, R2);
    for (int i=L2; i<R2; i++)
      if (arr[i] >= a && arr[i] < b) cnt++;
    assert(it1 != it2);
    it2--;
  }

  while (true){
    cnt += it1->second.prev_pos() - L;
    L = it1->second.pos;
    if (it1 == it2) break;
    it1++;
  }
  return cnt;
}
