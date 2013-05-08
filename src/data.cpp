#include <stdio.h>

#define MAXN (1 << 25)

int arr[MAXN];

int main(int argc, char *argv[]){
	FILE *in = fopen(argv[1],"rb");
	if (!in){ fprintf(stderr,"Error opening file %s\n",argv[1]); return 1; }
	while (true) {
		int N = fread(arr, sizeof(int), MAXN, in);
		for (int i = 0; i < N; i++) printf("%d\n", arr[i]);
		if (N < MAXN) break;
	}
}
