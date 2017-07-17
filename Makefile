
CXX = g++
CXXFLAGS = -Wall -fPIC 
ROOTCONFIG=`root-config --cflags --glibs --libs` 

rebinBC501A_bkgsub: rebinBC501A_bkgsub.cxx
	$(CXX) $^ -o $@ $(CXXFLAGS) $(ROOTCONFIG) 

rebinBC537_bkgsub: rebinBC537_bkgsub.cxx
	$(CXX) $^ -o $@ $(CXXFLAGS) $(ROOTCONFIG)

makePDF: makePDF.cxx
	$(CXX) $^ -o $@ $(CXXFLAGS) $(ROOTCONFIG)  

clean:
	rm -f rebinBC501A_bkgsub rebinBC537_bkgsub makePDF

all: rebinBC501A_bkgsub rebinBC537_bkgsub makePDF
