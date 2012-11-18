#include "tester.h" // require implementations of init,insert,remove,query
#include "ai.h"     // needs COMPACT_EVERY, CRACK_AT to be defined

int *marr;          // materialize the query results
AI ai;
	
void init(int *a, int n, int cap){
  marr = new int[cap];
  ai.init(a,n);
}

void insert(int v){ ai.insert(v); }

void remove(int v){ ai.erase(v); }

int count_query(int a, int b){
  assert(0);
  return 0;
}

int view_query(int a, int b){
  ai.examined = 0;

  #ifdef AICC // hybrid crack-crack
    int cnt = ai.crack_crack(a,b,marr);
  #endif

  #ifdef AICS // hybrid crack-sort
    int cnt = ai.crack_sort(a,b,marr);
  #endif

  #ifdef AISS // hybrid sort-sort
    int cnt = ai.sort_sort(a,b,marr);
  #endif

  #ifdef AICC1R // hybrid crack-crack with 1 stochastic crack
    int cnt = ai.scrack_scrack(a,b,marr);
  #endif

  #ifdef AICS1R // hybrid crack-sort with 1 stochastic crack
    int cnt = ai.scrack_sort(a,b,marr);
  #endif
  
  #ifdef AICCRP2 // hybrid crack-crack with rounding to nearest power of two
    int aa = a, bb;
    while (aa&(aa-1)) aa &= aa-1;
    if (aa == (1<<30)) bb = 2147483647; else bb = aa<<1;
    ai.crack_crack(aa,bb,marr);
    int cnt = ai.crack_crack(a,b,marr);
  #endif

  #ifdef AICCRMSZ // hybrid crack-crack with rounding to a partition with minimum size = RMSZ
    int aa = (a/RMSZ)*RMSZ, bb = (int) min((long long)b - (b%RMSZ) + RMSZ, 2147483647LL);
    ai.crack_crack(aa,bb,marr);
    int cnt = ai.crack_crack(a,b,marr);
  #endif

  n_touched = ai.examined;
  return cnt;
}
