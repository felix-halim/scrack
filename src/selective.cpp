#include "crack.h"

Random rr;

int view_query(int a, int b){
  #ifdef DETERMINISTIC
    static int nth = 0;
    int cnt = (++nth % DETERMINISTIC == 0)? random_crack_and_materialize(a,b) : crack(a,b);
  #endif

  #ifdef PROBABILISTIC
    int cnt = (rr.nextDouble() < PROBABILISTIC)? random_crack_and_materialize(a,b) : crack(a,b);
  #endif

  return cnt;
}

int count_query(int a, int b){
  return view_query(a,b);
}
