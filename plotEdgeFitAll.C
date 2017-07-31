
#include "plotEdgeFit.C"

void plotEdgeFitAll(bool proton = false, bool print = false) 
{
    gErrorIgnoreLevel = kInfo;

    TCanvas * c = new TCanvas();
    for(int j=0; j<=63; j++) {
        plotEdgeFit(j,proton);
        if(print) {
            if(j==0) c->Print("plotEdgeFitAll.pdf(","pdf");
            else if(j==63) c->Print("plotEdgeFitAll.pdf)","pdf");
            else c->Print("plotEdgeFitAll.pdf","pdf");
        }
    }
    




}

