{
    TFile * f = TFile::Open("2.0MeV_50.root");
    tree->Draw("value[4]*500/16128 >> h(500,0,500)","value[4]>0");
    h->SetStats(0);
    gPad->SetLogy();
    
    h->SetTitle("TOF spectrum of 1.5 MeV neutrons in BC-537");
    h->GetXaxis()->SetTitle("TOF (ns)");
    h->GetYaxis()->SetTitle("Counts / ns");

    double a = 500./16128.;
    TLine l1(7733*a,0,7733*a,5e5);
    TLine l2(7960*a,0,7960*a,5e5);
    l1.SetLineColor(kRed);
    l2.SetLineColor(kRed);
    l1.SetLineWidth(2);
    l2.SetLineWidth(2);
    l1.SetLineStyle(2);
    l2.SetLineStyle(2);
    l1.Draw("same");
    l2.Draw("same");

    TLine l3(9000*a,0,9000*a,5e5);
    TLine l4(13000*a,0,13000*a,5e5);
    l3.SetLineColor(kBlue);
    l4.SetLineColor(kBlue);
    l3.SetLineWidth(2);
    l4.SetLineWidth(2);
    l3.SetLineStyle(2);
    l4.SetLineStyle(2);
    l3.Draw("same");
    l4.Draw("same");

    TLatex t1((7733)*a,1e3,"neutron pulse");
    TLatex t2((13000-9000)*a,1e3,"background");
    t1.SetTextSize(0.04);
    t2.SetTextSize(0.04);
    t1.SetTextColor(kRed);
    t2.SetTextColor(kBlue);

    t1.Draw("same");
    t2.Draw("same");
    
    h->GetYaxis()->SetRangeUser(10,5e5);
    h->SetLineColor(1);
    h->Draw("same");

}

