
void plotEdgeFit(int val = 4, bool proton = true)
{
    TFile * f = NULL;
    TH1F * h1 = NULL;

    if(proton == true) {
        f = TFile::Open("BC501A_bkgsub.root");
        h1 = (TH1F*)f->Get(Form("ProtonCal%d",val));
    }
    else {
        f = TFile::Open("BC537_bkgsub.root");
        h1 = (TH1F*)f->Get(Form("ScionixCal%d",val));
    }
    h1->SetStats(kFALSE);
    for(int i=0; i<h1->GetNbinsX(); i++) { if(h1->GetBinContent(i) < 0) h1->SetBinContent(i,0); }

    TH1F tmp(*h1);
    for(int i=0; i<tmp.GetNbinsX()/3.; i++) {
        tmp.SetBinContent(i,0);
    }

    double maxBin = tmp.GetMaximumBin();
    double maxBinCont = tmp.GetMaximum();
    //double maxBin = h1->GetMaximumBin();
    //double maxBinCont = h1->GetMaximum();

    double C10 = maxBinCont*0.125;
    double C90 = maxBinCont*0.875;

    //double C50 = (C10 + C90) / 2.;
    double C50 = maxBinCont/2.;
    
    int idx = maxBin;
    double L = 0;
    double L10 = 0;
    double L90 = 0;
    bool found = false;
    bool found10 = false;
    bool found90 = false;
    while(!found || !found10 || !found90) {
        if((h1->GetBinContent(idx) < C50) && !found) {
            found = true;
            L = h1->GetBinCenter(idx-1);
            std::cout << " found 50% " << std::endl;
        }
        if((h1->GetBinContent(idx) < C10) && !found10) {
            found10 = true;
            L10 = h1->GetBinCenter(idx-1);
            std::cout << " found 10% " << std::endl;
        }
        if((h1->GetBinContent(idx) < C90) && !found90) {
            found90 = true;
            L90 = h1->GetBinCenter(idx-1);
            std::cout << " found 90% " << std::endl;
        }
        idx++;
    }
    
    //L = (L90+L10)/2.; 

    std::cout << "MAX PH = " << maxBinCont << " @ L = " << h1->GetBinCenter(maxBin) << std::endl;
    std::cout << "PH(87.5%) = " << C90 << " @ L = " << L90 << "\t PH(50%) = " << C50 << " @ L = " << L << "\t PH(12.5%) = " << C10 << " @  L = " << L10 << std::endl;
    

    h1->SetTitle("");
    h1->GetYaxis()->SetRangeUser(0,h1->GetMaximum()*1.1);
    h1->GetXaxis()->SetLabelSize(0.032);
    h1->GetXaxis()->SetNdivisions(505);    

    h1->Draw("h");
    
    //TLine * line10 = new TLine(L10,-maxBinCont*0.05,L10,h1->GetBinContent(h1->FindBin(L10)));
    //TLine * line50 = new TLine(L,  -maxBinCont*0.05,L,h1->GetBinContent(h1->FindBin(L)));
    //TLine * line90 = new TLine(L90,-maxBinCont*0.05,L90,h1->GetBinContent(h1->FindBin(L90)));
    TLine * line10 = new TLine(L10,0,L10,h1->GetBinContent(h1->FindBin(L10)));
    TLine * line50 = new TLine(L,  0,L,h1->GetBinContent(h1->FindBin(L)));
    TLine * line90 = new TLine(L90,0,L90,h1->GetBinContent(h1->FindBin(L90)));
    
    line10->SetLineWidth(3);    
    line50->SetLineWidth(3);    
    line90->SetLineWidth(3);    
    line10->SetLineStyle(2);
    line50->SetLineStyle(2);
    line90->SetLineStyle(2);

    line10->Draw("same");    
    line50->Draw("same");    
    line90->Draw("same");    

    TLatex * text10 = new TLatex(L10*0.98,-maxBinCont*0.1,"L_{12.5}");
    TLatex * text50 = new TLatex(  L*0.98,-maxBinCont*0.1,"L");
    TLatex * text90 = new TLatex(L90*0.98,-maxBinCont*0.1,"L_{87.5}");

    text10->Draw("same");
    text50->Draw("same");
    text90->Draw("same");




}
