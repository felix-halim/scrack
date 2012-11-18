#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]){
	if (argc < 5){ fprintf(stderr,"usage: %s [nqueries] [npoints] [is_cumulative] [is_float]\n", argv[0]); exit(1); }

	int Q = atoi(argv[1]), 
		nPoints = atoi(argv[2]),
		is_cumul = argv[3][0] - '0',
		is_float = argv[4][0] - '0';

	double X = exp(log((double)Q)/nPoints),		// will generate nPoints in log 10 scale 
		nextSample = pow(X,0), 
		linGap = (double) Q/nPoints,
		value = 0, cvalue = 0;

	vector<double> values;
	vector<int> xs;

	for (int i=1; i<=Q; i++){
		int nr = scanf("%lf",&value);
//		fprintf(stderr, "val %lf\n", value);
		assert(nr == 1);
		cvalue += value;
		if (i >= nextSample || i == Q){
			xs.push_back(i);
			values.push_back(is_cumul? cvalue : value);
			nextSample = (is_cumul<2)? pow(X,xs.size()) : (linGap * xs.size());
		}
	}
	
	printf("[");
	for (int i=0; i<int(values.size()); i++){
		if (i) printf(",");
		if (is_float) printf("[%d,%.6lf]",xs[i],values[i]);
		else printf("[%d,%.0lf]",xs[i],values[i]);
	}
	printf("];\n");
}
