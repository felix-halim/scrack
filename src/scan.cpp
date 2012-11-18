#include "tester.h"           // require implementations of init,insert,remove,query

int *arr, allocN, N;          // the dataset array
int *marr, msize;

void init(int *a, int n, int cap){
  marr = new int[n];
  arr = new int[allocN = N = n];    // for updates expansion
  for (int i=0; i<N; i++)
    arr[i] = a[i];           // copy all
}

void insert(int v){
  assert(N < allocN);
  arr[N++] = v;
}

void remove(int v){
  for (int i=0; i<N; i++)
    if (arr[i] == v){
      arr[i] = arr[--N];
      return;
    }
  assert(0);
}

int view_query(int a, int b){
  msize = 0;
  for (int i=0; i<N; i++) 
    if (arr[i] >= a && arr[i] < b)
      marr[msize++] = arr[i];
  n_touched += N;
  return msize;      // returns 1 means all results are fully materialized
}

int count_query(int a, int b){
  return view_query(a,b);
}

