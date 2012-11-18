Stochastic Database Cracking
======

This is the source codes for the experiments in the 
[Stochastic Database Cracking](http://vldb.org/pvldb/vol5/p502_felixhalim_vldb2012.pdf) paper.

To run a particular algorithm on a particular dataset, execute:

    ./run.sh [data] [algo] [nqueries] [workload] [selectivity] [timelimit]


\[data\] is one of the following:
- 100000000.data
- skyserver.data (it will be downloaded on demand around 2.2GB)

\[algo\] is one of the following:
- crack
- sort
- scan
- ddc
- ddr
- dd1c
- dd1r
- mdd1r
- mdd1rp1
- mdd1rp5
- mdd1rp10
- mdd1rp50
- naive_r1th
- naive_r2th
- naive_r4th
- naive_r8th
- naive_r1x
- naive_r2x
- aicc
- aicc1r
- aics
- aics1r
- aiss

\[nqueries\] is an integer denoting the number of queries to be executed.

\[workload\] is one of the following:
- Random
- Sequential
- SeqNoOver
- SeqRevOver
- SeqAlternate
- SeqRand
- ZoomIn
- SeqZoomIn
- ZoomOut
- SeqZoomOut
- Skew
- ConsRandom
- SkyServer (downloaded on demand)

\[selectivity\] is a floating point, e.g.:
- 0.5 (means 50% selectivity)
- 1e-2 (means 1% selectivity)
- 1e-7 (means 0.00001% selectivity)

\[timelimit\] is an integer denoting the maximum runtime in seconds before it is prematurely terminated (if exceeded).

Example runs:

    ./run.sh 100000000.data crack 100000 Random 1e-2 30
    ./run.sh skyserver.data dd1r 200000 SkyServer 1e-7 60
