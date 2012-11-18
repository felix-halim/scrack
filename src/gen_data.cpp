#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "random.h"

using namespace std;

int main(int argc, char *argv[]){
  int N;
  Random r(140384);
  sscanf(argv[1],"%d",&N);
  int *arr = new int[N];
  for (int i=0; i<N; i++) arr[i] = abs(r.nextInt());
  // for (int i=0; i<N; i++) arr[i] = i;
  // for (int i=0; i<N; i++)
  //   swap(arr[i],arr[r.nextInt(N)]);
  
  char fn[100];
  sprintf(fn,"%d.data",N);
  FILE *out = fopen(fn,"wb");
  int nw = fwrite(arr, sizeof(int), N, out);
  assert(nw == N);
  fclose(out);
}
