
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>

#include "TStyle.h"

#include "TGaxis.h"

//void makePDF()
int main()
{
    gStyle->SetTitleW(1.2);
    
    TFile * f = TFile::Open("BC537_bkgsub.root");    

    TH1F * ph = NULL;
    TH1F * bkg = NULL;
    TH1F * sub = NULL;
    TH1F * no = NULL;

    TCanvas * c = new TCanvas();
    std::string name = "outputBC537.pdf";

    TLegend * leg = NULL;

    TGaxis::SetMaxDigits(3);

    for(int i=0; i<=63; i++) {
        ph = (TH1F*)f->Get(Form("ScionixCal_wbkg%d",i));
        bkg = (TH1F*)f->Get(Form("BkgCal%d",i));
        sub = (TH1F*)f->Get(Form("ScionixCal%d",i));
        no = (TH1F*)f->Get(Form("NoCutsCal%d",i));

        ph->SetLineColor(kBlack);
        bkg->SetLineColor(kBlue);
        sub->SetLineColor(kRed);
        no->SetLineColor(kGreen);

        ph->SetStats(0); 
        ph->SetTitle(Form("Run %d",i));        
        ph->GetXaxis()->SetTitle("Pulse height (MeVee)");
        ph->GetYaxis()->SetTitle("Counts / bin");
        ph->GetXaxis()->SetTitleSize(0.045);        
        ph->GetYaxis()->SetTitleSize(0.05);        
        ph->GetXaxis()->SetLabelSize(0.045);        
        ph->GetYaxis()->SetLabelSize(0.04);        
        ph->GetYaxis()->SetNdivisions(505);
        ph->GetYaxis()->SetTitleOffset(1.0);

        if(leg) delete leg; 
        leg = new TLegend(0.4,0.7,0.89,0.89);
        leg->AddEntry(no,"Raw PHS","l");
        leg->AddEntry(ph,"PHS w/ TOF cuts","l");
        leg->AddEntry(sub,"PHS w/ TOF cuts and bkg. sub.","l");
        leg->AddEntry(bkg,"Background","l");
        leg->SetTextSize(0.04);       
        
        if(i == 30) {
            ph->SetTitle("Pulse-height spectra of 1.5 MeV neutrons in BC-537");
        }

        ph->Draw("h");
        bkg->Draw("same h");
        sub->Draw("same h");
        no->Draw("same h");
        leg->Draw();    
                
        std::string tmp = name;
        if(i==0) {
            tmp += "(";
            c->Print(tmp.c_str(),"pdf");
        }
        else if(i==63) {
            tmp += ")";
            c->Print(tmp.c_str(),"pdf");
        }
        else {
            c->Print(tmp.c_str(),"pdf");
        }

    }
    
    f = TFile::Open("BC501A_bkgsub.root");
    name = "outputBC501A.pdf";
    
    for(int i=0; i<=63; i++) {
        ph = (TH1F*)f->Get(Form("ProtonCal_wbkg%d",i));
        bkg = (TH1F*)f->Get(Form("BkgCal%d",i));
        sub = (TH1F*)f->Get(Form("ProtonCal%d",i));
        no = (TH1F*)f->Get(Form("NoCutsCal%d",i));
 
        //no->Scale(ph->Integral()/no->Integral());
   
        ph->SetLineColor(kBlack);
        bkg->SetLineColor(kBlue);
        sub->SetLineColor(kRed);
        no->SetLineColor(kGreen);
        
        ph->SetStats(0); 
        ph->SetTitle(Form("Run %d",i));        
        ph->GetXaxis()->SetTitle("Pulse height (MeVee)");
        ph->GetYaxis()->SetTitle("Counts / bin");
        ph->GetXaxis()->SetTitleSize(0.045);        
        ph->GetYaxis()->SetTitleSize(0.05);        
        ph->GetXaxis()->SetLabelSize(0.045);        
        ph->GetYaxis()->SetLabelSize(0.04);
        ph->GetYaxis()->SetNdivisions(505);
        ph->GetYaxis()->SetTitleOffset(1.0);

        ph->GetYaxis()->SetRangeUser(0,2*ph->GetMaximum());
        
        if(leg) delete leg; 
        leg = new TLegend(0.4,0.7,0.89,0.89);
        leg->AddEntry(no,"Raw PHS","l");
        leg->AddEntry(ph,"PHS w/ TOF cuts","l");
        leg->AddEntry(sub,"PHS w/ TOF cuts and bkg. sub.","l");
        leg->AddEntry(bkg,"Background","l");
        leg->SetTextSize(0.04);       
        
        if(i == 30) {
            ph->SetTitle("Pulse-height spectra of 1.5 MeV neutrons in NE-213");
        }

        //no->Draw("h");
        //ph->Draw("same");
        ph->Draw("h");
        no->Draw("same h");    
        bkg->Draw("same h");
        sub->Draw("same h");    
        leg->Draw("same");        
        
        std::string tmp = name;
        if(i==0) {
            tmp += "(";
            c->Print(tmp.c_str(),"pdf");
        }
        else if(i==63) {
            tmp += ")";
            c->Print(tmp.c_str(),"pdf");
        }
        else {
            c->Print(tmp.c_str(),"pdf");
        }

    }
    delete c;


    

    return 0;
}
