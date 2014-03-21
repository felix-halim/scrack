CC			=	g++
CFLAGS		=	-O3 -Wall -I. -funroll-loops
SRCDIR		=	src
OUTDIR		=	bin
SHELL		=	/bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .c .o

all: $(OUTDIR)/sort \
     $(OUTDIR)/scan \
     $(OUTDIR)/crack \
     $(OUTDIR)/ddc \
     $(OUTDIR)/ddr \
     $(OUTDIR)/mdd1r \
     $(OUTDIR)/mdd1rp \
     $(OUTDIR)/selective \
     $(OUTDIR)/ai \
     $(OUTDIR)/res_parser \
     $(OUTDIR)/res_table \
     $(OUTDIR)/gen_data

CRACKERS_H_DEP	=	$(SRCDIR)/crackers.h $(SRCDIR)/hash.h
TESTER_H_DEP	=	$(SRCDIR)/tester.h $(SRCDIR)/workload.h $(SRCDIR)/random.h
CRACK_H_DEP		=	$(SRCDIR)/crack.h $(TESTER_H_DEP) $(CRACKERS_H_DEP)

$(OUTDIR)/sort: $(SRCDIR)/sort.cpp $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/sort $(SRCDIR)/sort.cpp -lz

$(OUTDIR)/scan: $(SRCDIR)/scan.cpp $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/scan $(SRCDIR)/scan.cpp -lz

$(OUTDIR)/crack: $(SRCDIR)/crack.cpp $(CRACK_H_DEP)
	$(CC) $(CFLAGS) -DMAX_CRACKERS=2000000000 -o $(OUTDIR)/crack $(SRCDIR)/crack.cpp -lz
	$(CC) $(CFLAGS) -DRANDOM_CRACK_PER_QUERY=1 -o $(OUTDIR)/naive_r1x $(SRCDIR)/crack.cpp -lz
	$(CC) $(CFLAGS) -DRANDOM_CRACK_PER_QUERY=2 -o $(OUTDIR)/naive_r2x $(SRCDIR)/crack.cpp -lz
	$(CC) $(CFLAGS) -DRANDOM_CRACK_PER_QUERY=4 -o $(OUTDIR)/naive_r4x $(SRCDIR)/crack.cpp -lz
	$(CC) $(CFLAGS) -DRANDOM_CRACK_EVERY_NTH_QUERY=1 -o $(OUTDIR)/naive_r1th $(SRCDIR)/crack.cpp -lz
	$(CC) $(CFLAGS) -DRANDOM_CRACK_EVERY_NTH_QUERY=2 -o $(OUTDIR)/naive_r2th $(SRCDIR)/crack.cpp -lz
	$(CC) $(CFLAGS) -DRANDOM_CRACK_EVERY_NTH_QUERY=4 -o $(OUTDIR)/naive_r4th $(SRCDIR)/crack.cpp -lz
	$(CC) $(CFLAGS) -DRANDOM_CRACK_EVERY_NTH_QUERY=8 -o $(OUTDIR)/naive_r8th $(SRCDIR)/crack.cpp -lz

$(OUTDIR)/ddc: $(SRCDIR)/ddc.cpp $(CRACK_H_DEP)
	$(CC) $(CFLAGS) -DMAX_NCRACK=1 -DCRACK_AT=128 -o $(OUTDIR)/dd1c $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=32 -o $(OUTDIR)/ddc32 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=64 -o $(OUTDIR)/ddc64 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=128 -o $(OUTDIR)/ddc128 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=256 -o $(OUTDIR)/ddc256 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=512 -o $(OUTDIR)/ddc512 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=1024 -o $(OUTDIR)/ddc1024 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=4096 -o $(OUTDIR)/ddcL2 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=8192 -o $(OUTDIR)/ddc8192 $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=1000000 -o $(OUTDIR)/ddc1M $(SRCDIR)/ddc.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=10000000 -o $(OUTDIR)/ddc10M $(SRCDIR)/ddc.cpp -lz
	cp $(OUTDIR)/ddc128 $(OUTDIR)/ddc

$(OUTDIR)/ddr: $(SRCDIR)/ddr.cpp $(CRACK_H_DEP)
	$(CC) $(CFLAGS) -DCRACK_AT=128 -o $(OUTDIR)/dd1rs $(SRCDIR)/dd1rs.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1 -DCRACK_AT=128 -o $(OUTDIR)/dd1r $(SRCDIR)/ddr.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=128 -o $(OUTDIR)/ddr128 $(SRCDIR)/ddr.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=1024 -o $(OUTDIR)/ddr1024 $(SRCDIR)/ddr.cpp -lz
	$(CC) $(CFLAGS) -DMAX_NCRACK=1000 -DCRACK_AT=8192 -o $(OUTDIR)/ddr8192 $(SRCDIR)/ddr.cpp -lz
	cp $(OUTDIR)/ddr128 $(OUTDIR)/ddr

$(OUTDIR)/mdd1r: $(SRCDIR)/mdd1r.cpp $(CRACK_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/mdd1r $(SRCDIR)/mdd1r.cpp -lz
	$(CC) $(CFLAGS) -DMIN_PCSZ=1000 -o $(OUTDIR)/mdd1r1k $(SRCDIR)/mdd1r.cpp -lz
	$(CC) $(CFLAGS) -DMIN_PCSZ=10000 -o $(OUTDIR)/mdd1r10k $(SRCDIR)/mdd1r.cpp -lz
	$(CC) $(CFLAGS) -DMIN_PCSZ=100000 -o $(OUTDIR)/mdd1r100k $(SRCDIR)/mdd1r.cpp -lz
	$(CC) $(CFLAGS) -DMIN_PCSZ=1000000 -o $(OUTDIR)/mdd1r1m $(SRCDIR)/mdd1r.cpp -lz

$(OUTDIR)/mdd1rp: $(SRCDIR)/mdd1rp.cpp $(CRACK_H_DEP)
	$(CC) $(CFLAGS) -DPERCENTAGE=1 -o $(OUTDIR)/mdd1rp1 $(SRCDIR)/mdd1rp.cpp -lz
	$(CC) $(CFLAGS) -DPERCENTAGE=5 -o $(OUTDIR)/mdd1rp5 $(SRCDIR)/mdd1rp.cpp -lz
	$(CC) $(CFLAGS) -DPERCENTAGE=10 -o $(OUTDIR)/mdd1rp10 $(SRCDIR)/mdd1rp.cpp -lz
	$(CC) $(CFLAGS) -DPERCENTAGE=50 -o $(OUTDIR)/mdd1rp50 $(SRCDIR)/mdd1rp.cpp -lz
	$(CC) $(CFLAGS) -DPERCENTAGE=100 -o $(OUTDIR)/mdd1rp100 $(SRCDIR)/mdd1rp.cpp -lz
	cp $(OUTDIR)/mdd1rp10 $(OUTDIR)/mdd1rp

$(OUTDIR)/selective: $(SRCDIR)/selective.cpp $(CRACK_H_DEP)
	$(CC) $(CFLAGS) -DPROBABILISTIC=0.5 -o $(OUTDIR)/selective50p $(SRCDIR)/selective.cpp -lz
	$(CC) $(CFLAGS) -DPROBABILISTIC=0.2 -o $(OUTDIR)/selective20p $(SRCDIR)/selective.cpp -lz
	$(CC) $(CFLAGS) -DPROBABILISTIC=0.8 -o $(OUTDIR)/selective80p $(SRCDIR)/selective.cpp -lz
	$(CC) $(CFLAGS) -DDETERMINISTIC=2 -o $(OUTDIR)/selective2d $(SRCDIR)/selective.cpp -lz
	$(CC) $(CFLAGS) -DDETERMINISTIC=10 -o $(OUTDIR)/selective10d $(SRCDIR)/selective.cpp -lz
	$(CC) $(CFLAGS) -DDETERMINISTIC=100 -o $(OUTDIR)/selective100d $(SRCDIR)/selective.cpp -lz
	touch $(OUTDIR)/selective

$(OUTDIR)/ai: $(SRCDIR)/ai.cpp $(SRCDIR)/ai.h $(TESTER_H_DEP) $(CRACKERS_H_DEP)
	$(CC) $(CFLAGS) -DCRACK_AT=128 -DCOMPACT_EVERY=1000 -DAICC -DAI_IPS=1000000 -o $(OUTDIR)/aicc1k $(SRCDIR)/ai.cpp -lz
	$(CC) $(CFLAGS) -DCRACK_AT=128 -DCOMPACT_EVERY=1000000000 -DAICC -DAI_IPS=1000000 -o $(OUTDIR)/aicc $(SRCDIR)/ai.cpp -lz
	$(CC) $(CFLAGS) -DCRACK_AT=128 -DCOMPACT_EVERY=1000000000 -DAICS -DAI_IPS=1000000 -o $(OUTDIR)/aics $(SRCDIR)/ai.cpp -lz
	$(CC) $(CFLAGS) -DCRACK_AT=128 -DCOMPACT_EVERY=1000000000 -DAISS -DAI_IPS=1000000 -o $(OUTDIR)/aiss $(SRCDIR)/ai.cpp -lz
	$(CC) $(CFLAGS) -DCRACK_AT=128 -DCOMPACT_EVERY=1000000000 -DAICC1R -DAI_IPS=1000000 -o $(OUTDIR)/aicc1r $(SRCDIR)/ai.cpp -lz
	$(CC) $(CFLAGS) -DCRACK_AT=128 -DCOMPACT_EVERY=1000000000 -DAICS1R -DAI_IPS=1000000 -o $(OUTDIR)/aics1r $(SRCDIR)/ai.cpp -lz
	touch $(OUTDIR)/ai

$(OUTDIR)/res_parser: $(SRCDIR)/res_parser.cpp
	$(CC) $(CFLAGS) -o $(OUTDIR)/res_parser $(SRCDIR)/res_parser.cpp -lz

$(OUTDIR)/res_table: $(SRCDIR)/res_table.cpp
	$(CC) $(CFLAGS) -o $(OUTDIR)/res_table $(SRCDIR)/res_table.cpp -lz

$(OUTDIR)/gen_data: $(SRCDIR)/gen_data.cpp
	$(CC) $(CFLAGS) -o $(OUTDIR)/gen_data $(SRCDIR)/gen_data.cpp -lz

data/1000000.data: $(OUTDIR)/gen_data
	$(OUTDIR)/gen_data 1000000
	mv 1000000.data data

data/10000000.data: $(OUTDIR)/gen_data
	$(OUTDIR)/gen_data 10000000
	mv 10000000.data data

data/100000000.data: $(OUTDIR)/gen_data
	$(OUTDIR)/gen_data 100000000
	mv 100000000.data data

data/skyserver.data:
	(cd data; \
	 wget http://felix-halim.net/research/scrack/data/skyserver.data; \
	 wget http://felix-halim.net/research/scrack/data/skyserver.queries)

.PHONY : clean
clean:
	rm -fr $(OUTDIR)/* res/*
