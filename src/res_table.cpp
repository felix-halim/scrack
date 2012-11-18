#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define REP(i,n) for (int i=0,_n=n; i<_n; i++)

int main(int argc, char *argv[]){
	vector<string> algos;
	for (int i=1; i<argc; i++)
		algos.push_back(argv[i]);

	string prefix = "res/fig3_", suffix = ".out";

	const char *ws[17] = {
		"random",
		"seqover",
		"seqinv",
		"seqrand",
		"seqnoover",
		"seqalt",
		"consrandom",
		"zoomin",
		"zoomout",
		"seqzoomin",
		"seqzoomout",
		"skew",
		"zoomoutalt",
		"skewzoomoutalt",
		"periodic",
		"mixed",
		"skyserver" };

	vector<vector<double> > arr(17, vector<double>(algos.size()));

	REP(i,algos.size()) REP(j,17){
		string fn = prefix + ws[j] + "_" + algos[i] + suffix;
		freopen(fn.c_str(), "r", stdin);

		int cnt, examined;
		double sch_t, up_t, csch_t = 0, cup_t = 0;
		scanf("%lf",&cup_t);
		while (scanf("%d %d %lf %lf",&cnt,&examined,&up_t,&sch_t)==4){
			cup_t += up_t;
			csch_t += sch_t;
		}
		arr[j][i] = cup_t + csch_t;
	}

	printf("%15s","");
	REP(i,algos.size()) printf("%7.7s", algos[i].c_str()); puts("");
	REP(i,17){
		double min_rt = *min_element(arr[i].begin(), arr[i].end());
		printf("%-15s",ws[i]);
		REP(j,algos.size()){
			if (arr[i][j] == min_rt)
				printf("& %7s","[BEST]");
			else
				printf("& %7.2lf", arr[i][j] / min_rt);
		}
		printf("& %7.2lf\n",min_rt);
	}	
}
