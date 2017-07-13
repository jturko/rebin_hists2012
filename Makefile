
CXX = g++
CXXFLAGS = -Wall -fPIC
ROOTCONFIG=`root-config --cflags --glibs`

rebinBC501A_bkgsub: rebinBC501A_bkgsub.cxx
	$(CXX) $(CXXFLAGS) $(ROOTCONFIG) -o $@ $^

rebinBC537_bkgsub: rebinBC537_bkgsub.cxx
	$(CXX) $(CXXFLAGS) $(ROOTCONFIG) -o $@ $^

makePDF: makePDF.cxx
	$(CXX) $(CXXFLAGS) $(ROOTCONFIG) -o $@ $^

clean:
	rm -r rebinBC501A_bkgsub rebinBC537_bkgsub makePDF

all: rebinBC501A_bkgsub rebinBC537_bkgsub makePDF
