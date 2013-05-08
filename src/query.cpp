#include <stdio.h>

int nth;
double ascension;

int main(int argc, char *argv[]){
	while (scanf("%d %lf", &nth, &ascension) != EOF) {
		printf("%.0lf\n", ascension * 1e6);
	}
}
