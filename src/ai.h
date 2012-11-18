#include "crackers.h"

//#define AI_IPS 1000000      // the Initial Partition Size

int USE_STOCHASTIC_AI = 1;

class AI_Bucket { public:
  value_type *arr;
  int n;          // the element container and it's size
  int lo, hi;     // the minimum and maximum value in this bucket
  int capacity;   // the maximum number of elements in this container
  int fetched;    // number of element that has been fetched to final partititons
  bool isSorted;  // is the elements in this container sorted?
  ci_type crack;  // the RBT to store the cracker piece positions and holes
  map<int,pair<int,int> > fetchedRange;  // the cracker pieces that already fetched
  AI_Bucket *next;

  AI_Bucket(int cp){
    assert(cp>0);
    crack.clear();
    arr = new int[capacity = cp];
    n = fetched = 0;
    lo = (1LL<<31)-1;
    hi = -1;
    isSorted = false;
    next = NULL;
  }

  ~AI_Bucket(){
    delete[] arr;
  }

  void insert(int x){    // append an element to this bucket
    arr[n++] = x;
    lo = min(lo, x);
    hi = max(hi, x);
    assert(n<=capacity);
  }
  
  int calc_fetched(){
    int nf = 0;
    FORE(it,fetchedRange){
      nf += it->second.second - it->second.first;
    }
    return nf;
  }
  
  void compact(){
    if (crack.empty()) return;
    int i = -1, j = -1, nf = 0;
    FORE(it,fetchedRange){
      if (i == -1) i = it->second.first;
      else while (j < it->second.first) arr[i++] = arr[j++];
      j = it->second.second;
      nf += it->second.second - it->second.first;
    }
    if (j!=-1) while (j < n) arr[i++] = arr[j++];
    n = i;
    fetched = 0;
    crack.clear();
    fetchedRange.clear();
    assert(next==NULL);
    assert(!isSorted);
  }

  template <bool USE_CRACKING>
  int index(int v, int &examined){            // find the index of this value
    if (USE_CRACKING){                        // find using cracking
      int L,R; 
      find_piece(crack,n,v,L,R);              // find the piece [L,R)
      examined += R - L;
      if (USE_STOCHASTIC_AI){
        if (R - L > CRACK_AT){                // split if the piece size is > CRACK_AT
          int X = arr[L + rand()%(R-L)];      // split in random position
          int M = partition(arr, X, L,R);      // add crack on X
          add_crack(crack, n, X, M);
          if (v < X) R = M; else L = M;       // go to the correct sub-piece
        }
      }
      int p = partition(arr,v, L,R);
      return add_crack(crack, n, v, p);        // crack the container on value v (query driven)
    } else {                                   // find the position of v on this sorted bucket
      if (!isSorted){                         // sort the bucket if it's not yet sorted
        sort(arr, arr+n);
        isSorted = true;
      }
      return lower_bound(arr,arr+n,v) - arr;  // binary search through it
    }
  }
};

bool buk_size_cmp(AI_Bucket *x, AI_Bucket *y) {
  return x->n > y->n;
}

class AI {
  multiset<int> pending_insert, pending_delete;  // pending updates (insert / delete)
  map<int,AI_Bucket*> fp; // final partitions orderings
  vector<AI_Bucket*> IP;  // the unsorted initial partitions
  int nth_query;          // a counter how many queries has elapsed
  
  typedef map<int,AI_Bucket*>::iterator mit;

  mit f(int v){
    mit it = fp.lower_bound(v); // handling off by one search
    if (it == fp.end()){ if (it != fp.begin()) it--; }
    else if (it == fp.begin()){}
    else if (it->first > v){ it--; }
    else { assert(it->first == v); }
    return it;
  }
  
  void debug_fp(){
    for (mit it = fp.begin(); it != fp.end(); it++){
      AI_Bucket *b = it->second;
      fprintf(stderr,"f = %d (%d, %d %d) :: ",it->first,b->n,b->lo,b->hi);
      REP(i, b->n) fprintf(stderr,"%d ",b->arr[i]);
      fprintf(stderr,"\n");
    }
  }
  
  // copy all values [v1,v2) from initial partitions to a final partition 
  template <int VARIANT>  // 0 = CRACK CRACK, 1 = CRACK SORT, 2 = SORT SORT (this happens during compile time!)
  AI_Bucket* extract(int v1, int v2){
    vector<value_type> t;
    REP(i,IP.size()){
      int i1 = IP[i]->index<VARIANT!=2>(v1,examined);
      int i2 = IP[i]->index<VARIANT!=2>(v2,examined);
      if (IP[i]->fetchedRange.count(v1)){
        pair<int,int> &p = IP[i]->fetchedRange[v1];
        IP[i]->fetched -= p.second - p.first;
        p.first = min(p.first, i1);
        p.second = max(p.second, i2);
        IP[i]->fetched += p.second - p.first;
      } else {
        IP[i]->fetchedRange[v1] = make_pair(i1,i2);
        IP[i]->fetched += i2 - i1;
      }
      assert(i1 <= i2);
      while (i1 < i2) t.push_back(IP[i]->arr[i1++]);
    }
    if (!t.size()) return NULL;
    AI_Bucket *b = new AI_Bucket(t.size()*2+100);  // creates a final partition
    REP(i,t.size()) b->insert(t[i]);
    assert(b->n > 0);
    assert(!fp.count(v1));  // the tree will index the smallest element
    return fp[v1] = b;      // index the final partition order
  }

  template <int VARIANT>    // 0 = CRACK CRACK, 1 = CRACK SORT, 2 = SORT SORT (this happens during compile time!)
  void scan_holes(int v1, int v2){
    vector<AI_Bucket*> prev;
    mit it = f(v1);
    assert(it != fp.end());
    AI_Bucket *b = it->second;
    while (b->hi+1 < v2){
      while (b->hi+1 < v2 && b->next){
        prev.push_back(b);
        b = b->next;  // one jump is enough
      }
      if (b->hi+1 >= v2) break;
      it = fp.lower_bound(b->hi+1);
      AI_Bucket *nb = NULL;
      if (it==fp.end()){
        nb = extract<VARIANT>(b->hi+1,v2);
        if (!nb) break;
        assert(nb);
      } else if (it->first != b->hi+1){
        nb = extract<VARIANT>(b->hi+1,it->first);
        if (!nb) nb = it->second;
      } else {
        nb = it->second;
      }
      b->next = nb;
    }
    REP(i,prev.size()) prev[i]->next = b;
    b->index<VARIANT==0>(v2,examined);
  }

  value_type* reallocate(value_type *a, int n, int cap){
    assert(n<=cap);
    value_type *b = new value_type[cap];
    memcpy(b,a,sizeof(value_type)*n);
    delete[] a;
    return b;
  }
  
  void scan_updates(int v1, int v2){
    mit it = f(v1);
    assert(it != fp.end());
    while (it != fp.end() && it->first < v2){
      AI_Bucket *b = it->second;
      value_type x = it->first;
      value_type y = v2;
      it++;
      if (it != fp.end() && it->first < v2) y = it->first;
      int need = b->n + pending_insert.size();
      if (need >= b->capacity){
        b->arr = reallocate(b->arr, b->n, need);
        b->capacity = need;
      }
      b->hi = max(b->hi, merge_ripple(b->crack,b->arr,b->n,pending_insert,pending_delete,x,y));
      assert(b->n <= b->capacity);
    }
  }

  void check_fp(){
    REP(i,IP.size()) check(IP[i]->crack,IP[i]->arr,IP[i]->n,pending_insert,pending_delete);
    FORE(it, fp){
      AI_Bucket *b = it->second;
      fprintf(stderr,"fp[%d] = %d %d\n",it->first,b->lo,b->hi);
      check(b->crack, b->arr, b->n,pending_insert,pending_delete);
    }
  }    

  template<int VARIANT>
  void fetch_to_fp(value_type v1, value_type v2){
    mit it = f(v1);
    if (it == fp.end()){
      extract<VARIANT>(v1,v2);
    } else if (v1 < it->first) {
      if (v2 < it->first){
        extract<VARIANT>(v1,v2);
      } else {
        extract<VARIANT>(v1,it->first);
        scan_holes<VARIANT>(v1,v2);
      }
    } else if (v1 > it->second->hi){
      it++;
      if (it==fp.end()){
        extract<VARIANT>(v1,v2);
      } else if (v2 < it->first){
        extract<VARIANT>(v1,v2);
      } else {
        extract<VARIANT>(v1,it->first);
        scan_holes<VARIANT>(v1,v2);
      }
    } else {
      it->second->index<VARIANT==0>(v1,examined);
      scan_holes<VARIANT>(v1,v2);
    }
  }

  void do_ip_compaction(){
    int tn=0, tf=0;
    REP(i,IP.size()) tn += IP[i]->n, tf += IP[i]->fetched;
    if (tf < tn / 2) return;
    
    REP(i,IP.size()) IP[i]->compact();
    sort(IP.begin(), IP.end(), buk_size_cmp);
    int i=0, j=IP.size()-1;
    examined += j;
    while (i<j){
      int space = IP[i]->capacity - IP[i]->n;
      int have = IP[j]->n;
      int move = min(space, have);
      memmove(IP[i]->arr + IP[i]->n, IP[j]->arr + IP[j]->n - move, move * sizeof(value_type));
      IP[i]->n += move;
      IP[j]->n -= move;
      if (IP[j]->n){
        i++;
      } else {
        delete IP[j--];
        IP.pop_back();
      }
    }
    fprintf(stderr,"c");
  }

  template<int VARIANT> // 0 = CRACK CRACK, 1 = CRACK SORT, 2 = SORT SORT (this happens during compile time!)
  int select(value_type v1, value_type v2, value_type *marr){
    nth_query++;
    if (IP.size() > 1 && nth_query % COMPACT_EVERY == 0) do_ip_compaction();
    
    crack_t.start();
    fetch_to_fp<VARIANT>(v1, v2);
    crack_t.stop();

    scan_updates(v1,v2);
    
    int midx = 0;
    #ifdef DEBUG  // materialize and check the result for debugging
      mit it = f(v1);
      // TODO
      if (it!=fp.begin()) it--; // it could be that the result spoiled to the prev bucket!
      assert(it!=fp.end());
      if (v2 > it->second->hi){
        int i = it->second->index<VARIANT==0>(v1,examined);
        for (; i < it->second->n; i++)
          if (it->second->arr[i] > 0)
            marr[midx++] = it->second->arr[i];
        for (it++; it!=fp.end() && v2 > it->first; it++){
          if (v2 <= it->second->hi) break;
          int j = it->second->index<VARIANT==0>(v1,examined);
          for (; j < it->second->n; j++)
            if (it->second->arr[j]>0 && it->second->arr[j] < v2)
              marr[midx++] = it->second->arr[j];
        }
        if (it!=fp.end()){
          int i1 = it->second->index<VARIANT==0>(v1,examined),
            i2 = it->second->index<VARIANT==0>(v2,examined);
          for (int j=i1; j<i2; j++)
            if (it->second->arr[j]>0) marr[midx++] = it->second->arr[j];
        }
      } else {
        int i = it->second->index<VARIANT==0>(v1,examined);
        int j = it->second->index<VARIANT==0>(v2,examined);
        for (int k=i; k<j; k++) if (it->second->arr[k]>0)
          marr[midx++] = it->second->arr[k];
      }
    #endif
    return midx;
  }

public :
  int examined;      // the number of examined tuples in a query
  
  AI(){}
  
  void init(int *arr, int n){            // creates initial partitions on the given aray and its size
    int j=0, nb = (n+AI_IPS-1) / AI_IPS;  // calculate the number of needed initial partitions
    IP.resize(nb);
    REP(i,nb){
      IP[i] = new AI_Bucket(AI_IPS);    // create an partition
      REP(k,min(n-j,AI_IPS))        // fill it with parts of the arr
        IP[i]->insert(arr[j++]);  
    }
    assert(j==n);              // sanity check that everything is inserted
  }

  ~AI(){  // destructor
    REP(i,IP.size()) delete IP[i];      // delete all initial partitions
    FORE(i,fp) delete i->second;      // delete all final partitions
  }
  
  void insert(int v){
    if (pending_delete.count(v)) pending_delete.erase(v);    // don't insert if exists in pdel
    else pending_insert.insert(v);
  }

  void erase(int v){
    if (pending_insert.count(v)) pending_insert.erase(v);    // don't delete if exists in pins
    else pending_delete.insert(v);
  }

  // the function implementation is generated during compile time
  // based on the VARIANT of the select<VARIANT> (0=CrackCrack, 1=CrackSort, 2=SortSort)
  int crack_crack(int v1, int v2, int *marr){
    USE_STOCHASTIC_AI = 0;
    return select<0>(v1,v2,marr);
  }
  int crack_sort(int v1, int v2, int *marr){
    USE_STOCHASTIC_AI = 0;
    return select<1>(v1,v2,marr);
  }
  int sort_sort(int v1, int v2, int *marr){
    return select<2>(v1,v2,marr);
  }
  
  int scrack_scrack(int v1, int v2, int *marr){
    USE_STOCHASTIC_AI = 1;
    return select<0>(v1,v2,marr);
  }
  int scrack_sort(int v1, int v2, int *marr){
    USE_STOCHASTIC_AI = 1;
    return select<1>(v1,v2,marr);
  }
};
