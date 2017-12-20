COMPILER = g++
FLAGS = -std=c++17
SRCS = icv.cpp
LIB = -lopencv_core -lopencv_imgcodecs
OUTDIR = ./bin/
OUTFILE = icv

TESTDIR = ./test/
TESTFILES = $(TESTDIR)umji.48x48.jpg $(TESTDIR)blue.225x225.png $(TESTDIR)red.20x20.gif
TESTOUTPUT = test.out

INSTALLER = install
BINDIR = /usr/bin

all: $(OUTFILE)

$(OUTFILE):
	$(COMPILER) $(FLAGS) $(SRCS) $(LIB) -o $(OUTDIR)$(OUTFILE)

bin-clean:
	rm -rf $(OUTDIR)$(OUTFILE)

test-clean:
	rm -rf $(TESTOUTPUT)

clean:
	rm -rf $(OUTDIR)$(OUTFILE) $(TESTOUTPUT)

test-i:
	$(MAKE) $(OUTFILE)
	./$(OUTDIR)$(OUTFILE) $(TESTFILES) -o $(TESTOUTPUT)

test-d:
	$(MAKE) $(OUTFILE)
	./$(OUTDIR)$(OUTFILE) $(TESTDIR) -o $(TESTOUTPUT)

install:
	$(MAKE) $(OUTFILE)
	install -m 557 $(OUTDIR)$(OUTFILE) $(BINDIR)

