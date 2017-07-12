
CXX = g++
CXXFLAGS = -Wall -fPIC
ROOTCONFIG=`root-config --cflags --glibs`

rebinBC537_bkgsub: rebinBC537_bkgsub.cxx
	$(CXX) $(CXXFLAGS) $(ROOTCONFIG) -o $@ $^

clean:
	rm -r rebinBC537_bkgsub
