COMPILER = g++
SRCS = icv.cpp
LIB = -lopencv_core -lopencv_imgcodecs
OUTFILE = icv

TESTDIR = ./test/
TESTFILES = $(TESTDIR)umji.48x48.jpg $(TESTDIR)blue.225x225.png $(TESTDIR)red.20x20.gif
TESTOUTPUT = test.out

INSTALLER = install
BINDIR = /usr/bin

all: $(OUTFILE)

$(OUTFILE):
	$(COMPILER) $(SRCS) $(LIB) -o $(OUTFILE)

bin-clean:
	rm -rf $(OUTFILE)

test-clean:
	rm -rf $(TESTOUTPUT)

clean:
	rm -rf $(OUTFILE) $(TESTOUTPUT)

test-i:
	$(MAKE) $(OUTFILE)
	./$(OUTFILE) $(TESTFILES) -o $(TESTOUTPUT)

test-d:
	$(MAKE) $(OUTFILE)
	./$(OUTFILE) $(TESTDIR) -o $(TESTOUTPUT)

install:
	$(MAKE) $(OUTFILE)
	install -m 557 $(OUTFILE) $(BINDIR)

