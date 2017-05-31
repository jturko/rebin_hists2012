
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

        std::cout << "sorting " << path << "\t";
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
        std::cout << " \t written to ScionixCal" << i << std::endl;
    }    

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::string treeFiles1[] = 
    {
        "4.35MeV_110_2",
        "4.35MeV_100_2",
        "4.35MeV_90_2",
        "2.0MeV_90",
        "2.0MeV_80",
        "2.0MeV_70",
        "2.0MeV_60",
        "2.0MeV_50",
        "2.0MeV_40",
        "2.0MeV_30",
        "2.0MeV_20",
        "2.0MeV_10",
        "2.0MeV_100",
        "2.0MeV_110",
        "2.0MeV_120",
        "2.0MeV_130",
        "2.0MeV_145_2",
        "0.7MeV_10",
        "0.7MeV_20",
        "0.7MeV_30",
        "0.7MeV_45",
        "0.7MeV_60",
        "0.7MeV_75",
        "0.7MeV_90_2",
        "0.7MeV_105_2",
        "0.7MeV_115",
        "0.7MeV_130_3",
        "0.7MeV_145_2"
    };

    double tofCutLow1[] = 
    {
        7435    ,7855    ,8320    ,4800    ,5770    ,6633    ,7225    ,7733    ,8100    ,8370    ,
        8590    ,8700    ,4330    ,3560    ,2760    ,1940    ,4825    ,8525    ,8230    ,7750    ,
        6610    ,4820    ,2450    ,6060    ,6100    ,6650    ,7000    ,6660    
    };
    double tofCutHigh1[] = 
    {
        7700    ,8150    ,8575    ,5475    ,6450    ,6945    ,7490    ,7960    ,8315    ,8550    ,
        8760    ,8860    ,4775    ,3990    ,3175    ,2400    ,5225    ,8880    ,8640    ,8260    ,
        7280    ,5730    ,3620    ,7600    ,8030    ,8570    ,9150    ,8700    
    };
 
    double phCutLow1[] = 
    {
        10  ,10  ,10  ,10  ,10  ,10  ,10  ,10  ,10  ,10  ,
        10  ,10  ,10  ,10  ,10  ,10  ,10  ,10  ,10  ,10  ,
        10  ,10  ,10  ,10  ,10  ,10  ,9   ,9   
    };
    double phCutHigh1[] =
    {
        320 ,400 ,480 ,160 ,180 ,220 ,260 ,310 ,360 ,410 ,
        440 ,480 ,110 ,100 ,90  ,80  ,70  ,130 ,120 ,110 ,
        90  ,70  ,55  ,44  ,34  ,32  ,26  ,26  
    };

   
    TH1F ** hists1 = (TH1F**)malloc(nFiles[1]*sizeof(TH1F*));
    
    slp = calArrayD[1]->GetParameter(0);
    offset = -1*slp*calArrayD[1]->GetParameter(1);        

    std::cout << " slp1 = " << slp << " offset1 = " << offset << std::endl;    

    for(int i=0; i<nFiles[1]; i++) {
        path = "~/data/testcan_neutron_raw/" + treeFiles1[i] + ".root";
        tmpFile = TFile::Open(path.c_str());
        treeArray_cal1[i] = (TTree*)tmpFile->Get("tree")->Clone();
        //tmpFile->Close();            

        std::cout << "sorting " << path << "\t";
        nEntries = treeArray_cal1[i]->GetEntries();
        treeArray_cal1[i]->SetBranchAddress("value",&val);
        
        //hists1[i] = new TH1F(Form("hists1_%d",i),Form("hists1_%d",i),20000,0,2000);
        hists1[i] = new TH1F(Form("hists1_%d",i),Form("hists1_%d",i),100,phCutLow1[i]-10,phCutHigh1[i]*1.2);
        
        for(int j=0; j<nEntries; j++) {
            treeArray_cal1[i]->GetEntry(j);
            
            //if(j<5) std::cout << "val[0] = " << val[0] << std::endl;
            //if(j<5) std::cout << "value = " << value << std::endl;
            
            fillval = slp*double(val[0])+offset;
            fillval = random.Gaus(fillval,hists1[i]->GetBinWidth(0)); 
            
            //if(val[4]>tofCutLow0[i]+40 && val[4]<tofCutHigh0[i] && val[0]>10 && fillval<phCutHigh0[i]*1.19) hists0[i]->Fill(fillval);
            if(val[4]>tofCutLow1[i] && val[4]<tofCutHigh1[i] && val[0]>10 && fillval<phCutHigh1[i]*1.19) hists1[i]->Fill(fillval);
        }   
        for(int k=0; k<hists1[i]->FindBin(1); k++) hists1[i]->SetBinContent(k,0);
        outfile->cd();
        hists1[i]->Write(Form("ScionixCal%d",i+nFiles[0]));
        std::cout << " \t written to ScionixCal" << i+nFiles[0] << std::endl;
    }    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::string treeFiles2 = "0.7MeV_145_3";
    double tofCutLow2 = 6020;
    double tofCutHigh2 = 9140;
    double phCutLow2 = 3;
    double phCutHigh2 = 22;

    TH1F ** hists2 = (TH1F**)malloc(nFiles[2]*sizeof(TH1F*));
    
    slp = calArrayD[2]->GetParameter(0);
    offset = -1*slp*calArrayD[2]->GetParameter(1);        

    std::cout << " slp2 = " << slp << " offset2 = " << offset << std::endl;    

    path = "~/data/testcan_neutron_raw/" + treeFiles2 + ".root";
    tmpFile = TFile::Open(path.c_str());
    treeArray_cal2[0] = (TTree*)tmpFile->Get("tree")->Clone();
    //tmpFile->Close();            

    std::cout << "sorting " << path << "\t";
    nEntries = treeArray_cal2[0]->GetEntries();
    treeArray_cal2[0]->SetBranchAddress("value",&val);
    
    hists2[0] = new TH1F(Form("hists2_%d",0),Form("hists2_%d",0),100,phCutLow2-10,phCutHigh2*1.2);
    
    for(int j=0; j<nEntries; j++) {
        treeArray_cal2[0]->GetEntry(j);
        
        //if(j<5) std::cout << "val[0] = " << val[0] << std::endl;
        //if(j<5) std::cout << "value = " << value << std::endl;
        
        fillval = slp*double(val[0])+offset;
        fillval = random.Gaus(fillval,hists2[0]->GetBinWidth(0)); 
        
        //if(val[4]>tofCutLow0[i]+40 && val[4]<tofCutHigh0[i] && val[0]>10 && fillval<phCutHigh0[i]*1.19) hists0[i]->Fill(fillval);
        if(val[4]>tofCutLow2 && val[4]<tofCutHigh2 && val[0]>0 && fillval > phCutLow2 && fillval<phCutHigh2*1.19) hists2[0]->Fill(fillval);
    }   
    for(int k=0; k<hists2[0]->FindBin(1); k++) hists2[0]->SetBinContent(k,0);
    outfile->cd();
    hists2[0]->Write(Form("ScionixCal%d",nFiles[0]+nFiles[1]));
    std::cout << " \t written to ScionixCal" << nFiles[0]+nFiles[1] << std::endl;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::string treeFiles3[] = 
    {
        "8.0MeV_10",
        "8.0MeV_20",
        "8.0MeV_30",
        "8.0MeV_40",
        "8.0MeV_50",
        "8.0MeV_60",
        "8.0MeV_70",
        "8.0MeV_80",
        "8.0MeV_90",
        "8.0MeV_100",
        "8.0MeV_110",
        "8.0MeV_120",
    };

    double tofCutLow3[] = 
    {
        11530,  11470,  11360,  11260,  11080,  10915,  10645,  10335,  10025,  9680,   9335,   8970
    };
    double tofCutHigh3[] = 
    {
        11650,  11600,  11520,  11380,  11220,  11030,  10790,  10500,  10210,  9855 ,  9525,   9200    
    };
 
    double phCutLow3[] = 
    {
        150,120,100,80,70,80,60,60,55,50,50,50
    };
    double phCutHigh3[] =
    {
        3100,3000,2680,2500,2170,1850,1560,1250,1050,825,650,575
    };

   
    TH1F ** hists3 = (TH1F**)malloc(nFiles[3]*sizeof(TH1F*));
    
    slp = calArrayD[3]->GetParameter(0);
    offset = -1*slp*calArrayD[3]->GetParameter(1);        

    std::cout << " slp3 = " << slp << " offset3 = " << offset << std::endl;    

    for(int i=0; i<=nFiles[3]; i++) {
        path = "~/data/testcan_neutron_raw/" + treeFiles3[i] + ".root";
        tmpFile = TFile::Open(path.c_str());
        treeArray_cal3[i] = (TTree*)tmpFile->Get("tree")->Clone();
        //tmpFile->Close();            

        std::cout << "sorting " << path << "\t";
        nEntries = treeArray_cal3[i]->GetEntries();
        treeArray_cal3[i]->SetBranchAddress("value",&val);
        
        hists3[i] = new TH1F(Form("hists3_%d",i),Form("hists3_%d",i),100,phCutLow3[i]-10,phCutHigh3[i]*1.2);
        //hists3[i] = new TH1F(Form("hists3_%d",i),Form("hists3_%d",i),5000,0,5000);
        
        for(int j=0; j<nEntries; j++) {
            treeArray_cal3[i]->GetEntry(j);
            
            //if(j<5) std::cout << "val[0] = " << val[0] << std::endl;
            //if(j<5) std::cout << "value = " << value << std::endl;
            
            fillval = slp*double(val[0])+offset;
            fillval = random.Gaus(fillval,hists3[i]->GetBinWidth(0)); 
            
            if(val[4]>tofCutLow3[i] && val[4]<tofCutHigh3[i] && val[0]>10 && fillval<phCutHigh3[i]*1.19) hists3[i]->Fill(fillval);
            //if(val[4]>tofCutLow3[i] && val[4]<tofCutHigh3[i] && val[0]>10) hists3[i]->Fill(fillval);
        }   
        for(int k=0; k<hists3[i]->FindBin(1); k++) hists3[i]->SetBinContent(k,0);
        outfile->cd();
        hists3[i]->Write(Form("ScionixCal%d",i+nFiles[0]+nFiles[1]+nFiles[2]));
        std::cout << " \t written to ScionixCal" << i+nFiles[0]+nFiles[1]+nFiles[2] << std::endl;
    }    

    //return treeArray_cal0;
    //return calArrayD;    
    return hists3;

}
