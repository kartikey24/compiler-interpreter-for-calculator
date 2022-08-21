

CPP = g++
BISON = bison
FLEX = flex
TGT = lp
SCAN = $(TGT).l
PARSE = $(TGT).y

OBJ = scan.o parse.tab.o main.o support.o code.o

CFLAGS = -g -fpermissive

HEADERS = common-headers.hh code.hh term-types.hh

$(TGT): $(OBJ)
	$(CPP) --std=c++0x  $(OBJ) -o $(TGT) -ly 

scan.o: scan.c $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c $<

parse.tab.o:parse.tab.c $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c  $<

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c $<

scan.c : $(SCAN) parse.tab.h
	$(FLEX) -l --yylineno -o scan.c $(SCAN)

parse.tab.c parse.tab.h : $(PARSE)
	$(BISON) -b parse -dv  $(PARSE)

clean :
	rm -f *.o *.output
	rm -f $(TGT) 
	rm -rf parse.tab.c parse.tab.h scan.c 
