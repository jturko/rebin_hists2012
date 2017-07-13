
void makePDF()
{
    TFile * f = TFile::Open("BC537_bkgsub.root");    

    TH1F * ph = NULL;
    TH1F * bkg = NULL;
    TH1F * sub = NULL;

    TCanvas * c = new TCanvas();
    std::string name = "outputBC537.pdf";

    for(int i=0; i<=63; i++) {
        ph = (TH1F*)f->Get(Form("ScionixCal_wbkg%d",i));
        bkg = (TH1F*)f->Get(Form("BkgCal%d",i));
        sub = (TH1F*)f->Get(Form("ScionixCal%d",i));
    
        ph->SetLineColor(kBlack);
        bkg->SetLineColor(kBlue);
        sub->SetLineColor(kRed);
        
        ph->Draw("h");
        bkg->Draw("same h");
        sub->Draw("same h");    
                
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
    
        ph->SetLineColor(kBlack);
        bkg->SetLineColor(kBlue);
        sub->SetLineColor(kRed);
        
        ph->Draw("h");
        bkg->Draw("same h");
        sub->Draw("same h");    
                
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


    


}
