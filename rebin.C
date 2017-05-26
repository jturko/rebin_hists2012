
TH1F ** rebin()
{
    
    //TTree ** treeArray = (TTree**)malloc(nfiles*sizeof(TTree*));
    TFile * tmpFile;
    std::string path;

    
    TFile * hists2012 = TFile::Open("~/data/hists2012.root");
        
    const int nRuns = 4;
    
    TF1 ** calArrayP = (TF1**)malloc(nRuns*sizeof(TF1*));
    TF1 ** calArrayD = (TF1**)malloc(nRuns*sizeof(TF1*));
    for(int i=0; i<nRuns; i++) {
        calArrayP[i] = (TF1*)hists2012->Get(Form("LinearCalibration%dProton",i));
        calArrayD[i] = (TF1*)hists2012->Get(Form("LinearCalibration%dScionix",i));
    }    
    hists2012->Close();
   
    const int lowFile[nRuns] = { 0, 23, 51, 52 };
    const int upFile[nRuns] = { 22, 50, 51, 63 };
    const int nFiles[nRuns] = { 23, 28, 1, 11 };
 
    TTree ** treeArray_cal0 = (TTree**)malloc(nFiles[0]*sizeof(TTree*));
    TTree ** treeArray_cal1 = (TTree**)malloc(nFiles[1]*sizeof(TTree*));
    TTree ** treeArray_cal2 = (TTree**)malloc(nFiles[2]*sizeof(TTree*));
    TTree ** treeArray_cal3 = (TTree**)malloc(nFiles[3]*sizeof(TTree*));
        
    std::string treeFiles0[] = 
    {
        "3.0MeV_145" ,
        "3.0MeV_125" ,
        "3.0MeV_105" ,
        "3.0MeV_95"  ,
        "3.0MeV_10"  ,
        "3.0MeV_20"  ,
        "3.0MeV_30"  ,
        "3.0MeV_40"  ,
        "3.0MeV_50"  ,
        "3.0MeV_60"  ,
        "3.0MeV_70"  ,
        "3.0MeV_80"  ,
        "4.35MeV_80" ,
        "4.35MeV_70" ,
        "4.35MeV_60" ,
        "4.35MeV_50" ,
        "4.35MeV_40" ,
        "4.35MeV_30" ,
        "4.35MeV_20" ,
        "4.35MeV_10" ,
        "4.35MeV_90" ,
        "4.35MeV_100",
        "4.35MeV_110" 
    };

    double tofCutLow0[] = 
    {
        3806    ,4783    ,5910    ,6508    ,9247    ,9163    ,9059    ,8843    ,8575    ,8212    ,7795    ,7346    ,
        8614    ,8979    ,9295    ,9553    ,9742    ,9889    ,10021   ,10072   ,8188    ,7766    ,7303    
    };    
    double tofCutHigh0[] = 
    {
        4039    ,5036    ,6151    ,6751    ,9383    ,9317    ,9187    ,8988    ,8721    ,8375    ,7983    ,7556    ,
        8765    ,9113    ,9421    ,9670    ,9876    ,10046   ,10172   ,10238   ,8347    ,7929    ,7471    
    };

    double phCutLow0[] = 
    {
        40  ,40  ,50  ,50  ,50  ,50  ,50  ,50  ,50  ,50  ,50  ,50  ,
        50  ,50  ,50  ,50  ,50  ,50  ,50  ,50  ,50  ,50  ,50  
    };
    double phCutHigh0[] = 
    {
        110 ,130 ,180 ,230 ,800 ,700 ,650 ,600 ,500 ,430 ,370 ,300 ,
        500 ,620 ,750 ,880 ,1000,1100,1200,1300,430 ,360 ,300 
    };    

    TH1F ** hists0 = (TH1F**)malloc(nFiles[0]*sizeof(TH1F*));
    int nEntries;
    int val[6];
    double slp, offset;
    double fillval;

    TRandom3 random;    
    TFile * outfile = TFile::Open("outfile.root","RECREATE");
    
    slp = calArrayD[0]->GetParameter(0);
    offset = -1*slp*calArrayD[0]->GetParameter(1);        

    std::cout << " slp0 = " << slp << " offset0 = " << offset << std::endl;    

    for(int i=0; i<nFiles[0]; i++) {
        path = "~/data/testcan_neutron_raw/" + treeFiles0[i] + ".root";
        tmpFile = TFile::Open(path.c_str());
        treeArray_cal0[i] = (TTree*)tmpFile->Get("tree")->Clone();
        //tmpFile->Close();            

        std::cout << "sorting " << path << std::endl;
        nEntries = treeArray_cal0[i]->GetEntries();
        treeArray_cal0[i]->SetBranchAddress("value",&val);
        
        //hists0[i] = new TH1F(Form("hists0_%d",i),Form("hists0_%d",i),10000,0,10000);
        //hists0[i] = new TH1F(Form("hists0_%d",i),Form("hists0_%d",i),50100,-10,5000);
        hists0[i] = new TH1F(Form("hists0_%d",i),Form("hists0_%d",i),100,phCutLow0[i]-10,phCutHigh0[i]*1.2);
        
        for(int j=0; j<nEntries; j++) {
            treeArray_cal0[i]->GetEntry(j);
            
            //if(j<5) std::cout << "val[0] = " << val[0] << std::endl;
            //if(j<5) std::cout << "value = " << value << std::endl;
            
            fillval = slp*double(val[0])+offset;
            fillval = random.Gaus(fillval,hists0[i]->GetBinWidth(0)); 
            
            //if(val[4]>tofCutLow0[i] && val[4]<tofCutHigh0[i] && val[0]>10) hists0[i]->Fill(fillval);
            if(val[4]>tofCutLow0[i]+40 && val[4]<tofCutHigh0[i] && val[0]>10 && fillval<phCutHigh0[i]*1.19) hists0[i]->Fill(fillval);
        }   
        for(int k=0; k<hists0[i]->FindBin(1); k++) hists0[i]->SetBinContent(k,0);
        outfile->cd();
        hists0[i]->Write(Form("ScionixCal%d",i));
    }    

    //return treeArray_cal0;
    //return calArrayD;    
    return hists0;

}
