#include <stdio.h>

#define MAXN (1 << 25)

int arr[MAXN];

int main(int argc, char *argv[]){
	FILE *in = fopen(argv[1],"rb");
	if (!in){ fprintf(stderr,"Error opening file %s\n",argv[1]); return 1; }
	while (true) {
		int total = 0;
		int* arrpos = arr;
		while (int sumtot = fread(arrpos, sizeof(int), 1000, in)) {
			total += sumtot;
			arrpos += sumtot;
		}
		int N = total;
		if (ferror(in)){ perror("Error while reading file"); fprintf(stderr,"Error reading %s!\n", argv[1]); exit(1); }
		for (int i = 0; i < N; i++) printf("%d\n", arr[i]);
		if (N < MAXN) break;
	}
}
