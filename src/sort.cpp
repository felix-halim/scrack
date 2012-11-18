#include "tester.h"      // require implementations of init,insert,remove,query
#include <math.h>

int *arr, sorted, allocN, N;        // the dataset array

void init(int *a, int n, int cap){
  arr = new int[allocN = N = n];  // for updates expansion
  for (int i=0; i<N; i++)
    arr[i] = a[i];    // copy all
  sorted = 0;
}

void insert(int v){
  assert(N < allocN);      // sort doesn't support update
  arr[N++] = v;
  sorted = 0;
}

void remove(int v){
  int i;
  if (sorted){
    i = std::lower_bound(arr, arr+N, v) - arr;
  } else {
    for (i=0; i<N && arr[i]!=v; i++);
  }
  assert(i<N && arr[i] == v);    // must exists
  arr[i] = arr[--N];
  sorted = 0;
}

int view_query(int a, int b){
  if (!sorted){
    n_touched += N;
    sort(arr, arr+N);
    sorted = 1;
  }
  assert(a <= b);
  int i1 = std::lower_bound(arr, arr+N, a) - arr;
  int i2 = std::lower_bound(arr+i1, arr+N, b) - arr;
  n_touched += (int) (log(N) / log(2) + 0.5);
  return i2 - i1;
}

int count_query(int a, int b){
  return view_query(a,b);
}

