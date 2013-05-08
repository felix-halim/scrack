function run {
	DATA=$1 # 100000000.data, skyserver.bin
	PROG=$2 # program name
	Q=$3 		# number of queries to be performed
	WORK=$4 # query workload: Random, Skyserver
	SEL=$5  # selectivity: 1e-2 ... 1e-6
	UPDT=$6 # NOUP, HFLV, ROLL, TRASH, DELETE
  T=$7    # maximum runtime limit in seconds
	Qret=view # view, count

	make -s -j 4
	make -s data/$DATA # prepare the dataset
	# make -s bin/$PROG # prepare the program
	bin/$PROG data/$DATA $Q $SEL $WORK $UPDT $Qret $T # run the program

	FN="${PROG}_${DATA}_${SEL}_${WORK}_${UPDT}_${Qret}"
	mv res/result_size.gz res/${FN}.result_size.gz
	mv res/examined.gz res/${FN}.examined.gz
	mv res/n_cracks.gz res/${FN}.n_cracks.gz
	mv res/n_touched.gz res/${FN}.n_touched.gz
	mv res/n_trash.gz res/${FN}.n_trash.gz
	mv res/n_rippled.gz res/${FN}.n_rippled.gz
	mv res/mri_t.gz res/${FN}.mri_t.gz
	mv res/mrd_t.gz res/${FN}.mrd_t.gz
	mv res/crack_t.gz res/${FN}.crack_t.gz
	mv res/tree_t.gz res/${FN}.tree_t.gz
	mv res/update_t.gz res/${FN}.update_t.gz
	mv res/search_t.gz res/${FN}.search_t.gz
	mv res/total_t.gz res/${FN}.total_t.gz
	mv res/res_q res/$FN.q
	Q=`cat res/$FN.q`
	# echo "q = $Q"

	# printf "var ${FN}_n_cracks = " >> runs.js; gunzip -c res/$FN.n_cracks.gz | bin/res_parser $Q 100 0 0 >> runs.js
	# printf "var ${FN}_cmri_t = " >> runs.js; gunzip -c res/$FN.mri_t.gz | bin/res_parser $Q 100 1 1 >> runs.js
	# printf "var ${FN}_cmrd_t = " >> runs.js; gunzip -c res/$FN.mrd_t.gz | bin/res_parser $Q 100 1 1 >> runs.js
	# printf "var ${FN}_crack_t = " >> runs.js; gunzip -c res/$FN.crack_t.gz | bin/res_parser $Q 100 1 1 >> runs.js
	# printf "var ${FN}_tree_t = " >> runs.js; gunzip -c res/$FN.tree_t.gz | bin/res_parser $Q 100 1 1 >> runs.js
	printf "var ${FN}_cupdate_t = " >> runs.js; gunzip -c res/$FN.update_t.gz | bin/res_parser $Q 100 1 1 >> runs.js
	printf "var ${FN}_csearch_t = " >> runs.js; gunzip -c res/$FN.search_t.gz | bin/res_parser $Q 100 1 1 >> runs.js
	printf "var ${FN}_ctotal_t = " >> runs.js; gunzip -c res/$FN.total_t.gz | bin/res_parser $Q 100 1 1 >> runs.js
	# printf "var ${FN}_total_t = " >> runs.js; gunzip -c res/$FN.total_t.gz | bin/res_parser $Q 100 0 1 >> runs.js
	# printf "var ${FN}_examined = " >> runs.js; gunzip -c res/$FN.examined.gz | bin/res_parser $Q 100 0 0 >> runs.js
	# printf "var ${FN}_trashed = " >> runs.js; gunzip -c res/$FN.n_trash.gz | bin/res_parser $Q 100 0 0 >> runs.js
	# printf "var ${FN}_rippled = " >> runs.js; gunzip -c res/$FN.n_rippled.gz | bin/res_parser $Q 100 0 0 >> runs.js
	# printf "var ${FN}_cexamined = " >> runs.js; gunzip -c res/$FN.examined.gz | bin/res_parser $Q 100 1 0 >> runs.js
	# printf "var ${FN}_ctrashed = " >> runs.js; gunzip -c res/$FN.n_trash.gz | bin/res_parser $Q 100 1 0 >> runs.js
	# printf "var ${FN}_crippled = " >> runs.js; gunzip -c res/$FN.n_rippled.gz | bin/res_parser $Q 100 1 0 >> runs.js
	printf "var ${FN}_ctouched = " >> runs.js; gunzip -c res/$FN.n_touched.gz | bin/res_parser $Q 100 1 0 >> runs.js
}


# rm bin/*
rm -f runs.js

if [ "$1" == "batch" ]; then
	T=3
	./run.sh skyserver.data crack 10000 SkyServer 1e-2 NOUP $T
	./run.sh 100000000.data crack 10000 Random 1e-2 NOUP $T
	./run.sh 100000000.data sort 10000 Random 1e-2 NOUP $T
	./run.sh 100000000.data scan 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data ddc 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data ddr 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data dd1c 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data dd1r 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data mdd1r 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data mdd1rp1 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data mdd1rp5 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data mdd1rp10 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data mdd1rp50 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data naive_r1th 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data naive_r2th 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data naive_r4th 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data naive_r8th 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data naive_r1x 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data naive_r2x 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data aicc 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data aicc1r 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data aics 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data aics1r 1000 Random 1e-2 NOUP $T
	./run.sh 100000000.data aiss 1000 Random 1e-2 NOUP $T
elif [ "$1" == "get-skyserver-dataset" ]; then
	make -s data/skyserver.data
	g++ src/data.cpp -o bin/data
	bin/data data/skyserver.data
elif [ "$1" == "get-skyserver-queries" ]; then
	make -s data/skyserver.data
	g++ src/query.cpp -o bin/query
	bin/query < data/skyserver.queries
else
	run $1 $2 $3 $4 $5 $6 $7
fi
