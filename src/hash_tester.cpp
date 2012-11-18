#include "hash.h"
#include <stdio.h>
#include <assert.h>
#include <map>

using namespace std;

int main(){
  map<int,int> m;
  Hash h(1000000);
  int a=0,b=0,c=0;

  for (int i=0; i<1000000; i++)
    h.set(i,i);

  fprintf(stderr,"%d\n",1);

  for (int i=0; i<100000; i++){
    int k = rand() % 1000000 + 1;
    int v = rand() % 1000000 + 1;
    if (m.count(k)) continue;
    m[k] = v;
    h.set(k,v);
    a++;
  }

  fprintf(stderr,"inserted\n");

  for (int i=0; i<2000000; i++){
    if (i%100000==0) fprintf(stderr,".");
    int k = rand() % 1000000 + 1;
    int v = rand() % 1000000 + 1;
    if (rand()%2){
      // add
      if (m.count(k)) continue;
      m[k] = v;
      h.set(k,v);
      b++;
    } else {
      // remove
      if (!m.count(k)) continue;
      m.erase(k);
      h.erase(k);
      c++;
    }
  }

  fprintf(stderr,"\nchecking %d %d %d\n",a,b,c);

  map<int,int>::iterator it = m.begin();
  while (it != m.end()){
    int k = it->first, v = it->second;
    assert(h.get(k) == v);
    it++;
  }
}

