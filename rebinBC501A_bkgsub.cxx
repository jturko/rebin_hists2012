
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TTree.h"
#include "TRandom3.h"

int main()
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
    //hists2012->Close();
   
    const int lowFile[nRuns] = { 0, 23, 51, 52 };
    const int upFile[nRuns] = { 22, 50, 51, 63 };
    const int nFiles[nRuns] = { 23, 28, 1, 12 };
 
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
        3500    ,4500    ,5650    ,6270    ,9130    ,9050    ,8900    ,8680    ,8400    ,8025    ,7590    ,7150    ,
        8445    ,8820    ,9140    ,9400    ,9630    ,9800    ,9935    ,10005   ,8025    ,7580    ,7100
    };    
    double tofCutHigh0[] = 
    {
        3800    ,4800    ,5940    ,6570    ,9210    ,9150    ,9015    ,8810    ,8545    ,8200    ,7800    ,7365    ,
        8600    ,8945    ,9260    ,9500    ,9720    ,9885    ,10000   ,10075   ,8165    ,7745    ,7270
    };

    double phCutLow0[] = 
    {
        57      ,57     ,72.5   ,72.5   ,72.5   ,72.5   ,72.5   ,72.5   ,72.5   ,
        72.5    ,72.5   ,72.5   ,72.5   ,72.5   ,72.5   ,72.5   ,72.5   ,200    ,
        200     ,200    ,100    ,100    ,100
    };
    double phCutHigh0[] = 
    {
        150     ,167    ,236 ,298 ,1040    ,970    ,1000    ,1000    ,800   ,
        700     ,550    ,450 ,800 ,1000    ,1100   ,1300    ,1500    ,1600  ,
        1800    ,1850   ,650 ,525 ,425
    };    

    double bkgCutLow0[] =
    {
        6000    ,7000    ,8000    ,8000    ,2000    ,2000    ,2000    ,2000    ,2000    ,1000    ,
        1000    ,1000    ,1000    ,1000    ,1000    ,1000    ,1000    ,1000    ,1000    ,1000    ,
        1000    ,1000    ,1000        
    };
    double bkgCutHigh0[] =
    {
        10000   ,12000   ,12000   ,12000   ,7000    ,7000    ,7000    ,7000    ,7000    ,7000    ,
        6000    ,6000    ,8000    ,8000    ,8000    ,8000    ,8000    ,8000    ,8000    ,9000    ,
        7000    ,6000    ,6000    
    };

 
    TH1F ** hists0 = (TH1F**)malloc(nFiles[0]*sizeof(TH1F*));
    TH1F ** hists0bkg = (TH1F**)malloc(nFiles[0]*sizeof(TH1F*));
    TH1F ** hists0bkgsub = (TH1F**)malloc(nFiles[0]*sizeof(TH1F*));
    int nEntries;
    int val[6];
    double slp, offset;
    double fillval;

    TRandom3 random;    
    TFile * outfile = TFile::Open("BC501A_bkgsub.root","RECREATE");
    
    slp = calArrayP[0]->GetParameter(0);
    offset = -1*slp*calArrayP[0]->GetParameter(1);        

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
        hists0bkg[i] = new TH1F(Form("hists0bkg_%d",i),Form("hists0bkg_%d",i),100,phCutLow0[i]-10,phCutHigh0[i]*1.2);
        //hists0bkgsub[i] = new TH1F(Form("hists0bkgsub_%d",i),Form("hists0bkgsub_%d",i),100,phCutLow0[i]-10,phCutHigh0[i]*1.2);
        
        for(int j=0; j<nEntries; j++) {
            treeArray_cal0[i]->GetEntry(j);
            
            //if(j<5) std::cout << "val[1] = " << val[1] << std::endl;
            //if(j<5) std::cout << "value = " << value << std::endl;
            
            fillval = slp*double(val[1])+offset;
            fillval = random.Gaus(fillval,hists0[i]->GetBinWidth(0)); 
            
            //if(val[5]>tofCutLow0[i] && val[5]<tofCutHigh0[i] && val[1]>10) hists0[i]->Fill(fillval);
            if(val[5]>tofCutLow0[i]+40 && val[5]<tofCutHigh0[i] && val[1]>10 && fillval<phCutHigh0[i]*1.19) hists0[i]->Fill(fillval);
            if(val[5]>bkgCutLow0[i] && val[5]<bkgCutHigh0[i] && fillval<phCutHigh0[i]*1.19) hists0bkg[i]->Fill(fillval);
        }   
        for(int k=0; k<hists0[i]->FindBin(1); k++) hists0[i]->SetBinContent(k,0);
        
        double scale = (tofCutHigh0[i]-tofCutLow0[i])/(bkgCutHigh0[i]-bkgCutLow0[i]);
        //hists0bkg[i]->Scale(scale);
        hists0bkg[i]->Sumw2(); hists0[i]->Sumw2();
        hists0bkgsub[i] = (TH1F*)hists0[i]->Clone();
        hists0bkgsub[i]->Add(hists0bkg[i],-1.0*scale);
        
        outfile->cd();
        hists0bkgsub[i]->Write(Form("ProtonCal%d",i));
        hists0[i]->Write(Form("ProtonCal_wbkg%d",i));
        hists0bkg[i]->Write(Form("BkgCal%d",i));
        std::cout << " \t written to ProtonCal" << i << " and BkgCal" << i << std::endl;
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
        7420    ,7890    ,8320    ,4860    ,5570    ,3225    ,6810    ,7300    ,7675    ,7970    ,
        8200    ,8335    ,4060    ,3290    ,2500    ,1740    ,4515    ,8140    ,7835    ,7350    ,
        6230    ,4520    ,2190    ,5740    ,5720    ,6400    ,6400    ,6000
    };
    double tofCutHigh1[] = 
    {
        7600    ,8060    ,8490    ,5220    ,5930    ,6560    ,7085    ,7535    ,7880    ,8150    ,
        8340    ,8450    ,4415    ,3620    ,2870    ,2060    ,4835    ,8390    ,8200    ,7790    ,
        6860    ,5335    ,3270    ,7100    ,7250    ,7840    ,8250    ,8000
    };
 
    double phCutLow1[] = 
    {
        17      ,17     ,20     ,8.5    ,8.5    ,15     ,15     ,15     ,15     ,15     ,15     ,15     ,8.5    ,
        8.5     ,8.5    ,8.5    ,8.5    ,8.5    ,8.5    ,8.5    ,8.5    ,8.5    ,
        8.5     ,8.5    ,8.5    ,8.5    ,8.5    ,8.5
    };
    double phCutHigh1[] =
    {
        425    ,540     ,600    ,200   ,240    ,300    ,350 ,
        420    ,500     ,550    ,600   ,620    ,160    ,130 ,
        110    ,95      ,75     ,165   ,155    ,130    ,110 ,
        75     ,55      ,45     ,35    ,30     ,25     ,25
    };
    
    double bkgCutLow1[] =
    {
        500     ,500     ,500     ,8000    ,8000    ,8000    ,8500    ,9000    ,
        9000    ,500     ,500     ,500     ,6000    ,5000    ,4000    ,3500    ,
        6000    ,500     ,500     ,500     ,8500    ,7000    ,6000    ,9500    ,
        10000   ,10000   ,11000   ,11000   
    };
    double bkgCutHigh1[] =
    {
        6500    ,7000    ,7500    ,15000   ,15000   ,15000   ,15000   ,15000   ,
        15000   ,7000    ,7000    ,7000    ,15000   ,15000   ,15000   ,15000   ,
        15000   ,7000    ,7000    ,6500    ,15000   ,15000   ,15000   ,15000   ,
        15000   ,15000   ,16000   ,16000   
    };

   
    TH1F ** hists1 = (TH1F**)malloc(nFiles[1]*sizeof(TH1F*));
    TH1F ** hists1bkg = (TH1F**)malloc(nFiles[1]*sizeof(TH1F*));
    TH1F ** hists1bkgsub = (TH1F**)malloc(nFiles[1]*sizeof(TH1F*));    

    slp = calArrayP[1]->GetParameter(0);
    offset = -1*slp*calArrayP[1]->GetParameter(1);        

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
        hists1bkg[i] = new TH1F(Form("hists1bkg_%d",i),Form("hists1bkg_%d",i),100,phCutLow1[i]-10,phCutHigh1[i]*1.2);
        
        for(int j=0; j<nEntries; j++) {
            treeArray_cal1[i]->GetEntry(j);
            
            //if(j<5) std::cout << "val[1] = " << val[1] << std::endl;
            //if(j<5) std::cout << "value = " << value << std::endl;
            
            fillval = slp*double(val[1])+offset;
            fillval = random.Gaus(fillval,hists1[i]->GetBinWidth(0)); 
            
            //if(val[5]>tofCutLow0[i]+40 && val[5]<tofCutHigh0[i] && val[1]>10 && fillval<phCutHigh0[i]*1.19) hists0[i]->Fill(fillval);
            if(val[5]>tofCutLow1[i] && val[5]<tofCutHigh1[i] && val[1]>10 && fillval<phCutHigh1[i]*1.19) hists1[i]->Fill(fillval);
            if(val[5]>bkgCutLow1[i] && val[5]<bkgCutHigh1[i] && fillval<phCutHigh1[i]*1.19) hists1bkg[i]->Fill(fillval);
        }   
        for(int k=0; k<hists1[i]->FindBin(1); k++) hists1[i]->SetBinContent(k,0);

        double scale = (tofCutHigh1[i]-tofCutLow1[i])/(bkgCutHigh1[i]-bkgCutLow1[i]);
        //hists1bkg[i]->Scale(scale);
        hists1bkg[i]->Sumw2(); hists1[i]->Sumw2();
        hists1bkgsub[i] = (TH1F*)hists1[i]->Clone();
        hists1bkgsub[i]->Add(hists1bkg[i],-1*scale);

        outfile->cd();
        hists1bkgsub[i]->Write(Form("ProtonCal%d",i+nFiles[0]));
        hists1[i]->Write(Form("ProtonCal_wbkg%d",i+nFiles[0]));
        hists1bkg[i]->Write(Form("BkgCal%d",i+nFiles[0]));
        std::cout << " \t written to ProtonCal" << i+nFiles[0] << " and BkgCal" << i+nFiles[0] << std::endl;
    }    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::string treeFiles2 = "0.7MeV_145_3";
    double tofCutLow2 = 5960;
    double tofCutHigh2 = 8080;
    double phCutLow2 = 8.5;
    double phCutHigh2 = 25;
    double bkgCutLow2 = 11000;
    double bkgCutHigh2 = 16000;
    

    TH1F ** hists2 = (TH1F**)malloc(nFiles[2]*sizeof(TH1F*));
    TH1F ** hists2bkg = (TH1F**)malloc(nFiles[2]*sizeof(TH1F*));
    TH1F ** hists2bkgsub = (TH1F**)malloc(nFiles[2]*sizeof(TH1F*));
    
    slp = calArrayP[2]->GetParameter(0);
    offset = -1*slp*calArrayP[2]->GetParameter(1);        

    std::cout << " slp2 = " << slp << " offset2 = " << offset << std::endl;    

    path = "~/data/testcan_neutron_raw/" + treeFiles2 + ".root";
    tmpFile = TFile::Open(path.c_str());
    treeArray_cal2[0] = (TTree*)tmpFile->Get("tree")->Clone();
    //tmpFile->Close();            

    std::cout << "sorting " << path << "\t";
    nEntries = treeArray_cal2[0]->GetEntries();
    treeArray_cal2[0]->SetBranchAddress("value",&val);
    
    hists2[0] = new TH1F(Form("hists2_%d",0),Form("hists2_%d",0),100,phCutLow2-10,phCutHigh2*1.2);
    hists2bkg[0] = new TH1F(Form("hists2bkg_%d",0),Form("hists2bkg_%d",0),100,phCutLow2-10,phCutHigh2*1.2);
    
    for(int j=0; j<nEntries; j++) {
        treeArray_cal2[0]->GetEntry(j);
        
        //if(j<5) std::cout << "val[1] = " << val[1] << std::endl;
        //if(j<5) std::cout << "value = " << value << std::endl;
        
        fillval = slp*double(val[1])+offset;
        fillval = random.Gaus(fillval,hists2[0]->GetBinWidth(0)); 
        
        //if(val[5]>tofCutLow0[i]+40 && val[5]<tofCutHigh0[i] && val[1]>10 && fillval<phCutHigh0[i]*1.19) hists0[i]->Fill(fillval);
        if(val[5]>tofCutLow2 && val[5]<tofCutHigh2 && val[1]>0 && fillval > phCutLow2 && fillval<phCutHigh2*1.19) hists2[0]->Fill(fillval);
        if(val[5]>bkgCutLow2 && val[5]<bkgCutHigh2 && fillval<phCutHigh2*1.19) hists2bkg[0]->Fill(fillval);
    }   
    for(int k=0; k<hists2[0]->FindBin(1); k++) hists2[0]->SetBinContent(k,0);
    
    double scale = (tofCutHigh2-tofCutLow2)/(bkgCutHigh2-bkgCutLow2);
    //hists2bkg[0]->Scale(scale);
    hists2bkg[0]->Sumw2(); hists2[0]->Sumw2();
    hists2bkgsub[0] = (TH1F*)hists2[0]->Clone();
    hists2bkgsub[0]->Add(hists2bkg[0],-1*scale);    

    outfile->cd();
    hists2bkgsub[0]->Write(Form("ProtonCal%d",nFiles[0]+nFiles[1]));
    hists2[0]->Write(Form("ProtonCal_wbkg%d",nFiles[0]+nFiles[1]));
    hists2bkg[0]->Write(Form("BkgCal%d",nFiles[0]+nFiles[1]));
    std::cout << " \t written to ProtonCal" << nFiles[0]+nFiles[1] << " and BkgCal" << nFiles[0]+nFiles[1] << std::endl;

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
        10940   ,10885   ,10785   ,10660   ,10475   ,10315   ,10065   ,9780    ,9470    ,9130    ,8775    ,8455
    };
    double tofCutHigh3[] = 
    {
        11010   ,10955   ,10870   ,10740   ,10585   ,10400   ,10145   ,9880    ,9590    ,9255    ,8920    ,8590
    };
 
    double phCutLow3[] = 
    {
        500    ,500    ,
        200     ,200    ,200    ,200    ,200    ,200    ,200    ,150    ,150    ,
        150
    };
    double phCutHigh3[] =
    {
        4150    ,4000  ,
        3650    ,3200   ,2800,2500,2050    ,1750   ,1400    ,1100    ,900   ,
        750
    };
    double bkgCutLow3[] = 
    {
        500 ,500 ,500 ,500 ,500 ,500 ,500 ,500 ,500 ,500 ,500 ,500
    };
    double bkgCutHigh3[] = 
    {
        5400    ,5400    ,5400    ,5400    ,5400    ,5400    ,
        5400    ,5400    ,5400    ,5400    ,5400    ,5400    
    };

   
    TH1F ** hists3 = (TH1F**)malloc(nFiles[3]*sizeof(TH1F*));
    TH1F ** hists3bkg = (TH1F**)malloc(nFiles[3]*sizeof(TH1F*));
    TH1F ** hists3bkgsub = (TH1F**)malloc(nFiles[3]*sizeof(TH1F*));
    
    slp = calArrayP[3]->GetParameter(0);
    offset = -1*slp*calArrayP[3]->GetParameter(1);        

    std::cout << " slp3 = " << slp << " offset3 = " << offset << std::endl;    

    //for(int i=0; i<=nFiles[3]; i++) {
    for(int i=0; i<nFiles[3]; i++) {
        path = "~/data/testcan_neutron_raw/" + treeFiles3[i] + ".root";
        tmpFile = TFile::Open(path.c_str());
        treeArray_cal3[i] = (TTree*)tmpFile->Get("tree")->Clone();
        //tmpFile->Close();            

        std::cout << "sorting " << path << "\t";
        nEntries = treeArray_cal3[i]->GetEntries();
        treeArray_cal3[i]->SetBranchAddress("value",&val);
        
        hists3[i] = new TH1F(Form("hists3_%d",i),Form("hists3_%d",i),100,phCutLow3[i]-10,phCutHigh3[i]*1.2);
        hists3bkg[i] = new TH1F(Form("hists3bkg_%d",i),Form("hists3bkg_%d",i),100,phCutLow3[i]-10,phCutHigh3[i]*1.2);
        //hists3[i] = new TH1F(Form("hists3_%d",i),Form("hists3_%d",i),5000,0,5000);
        
        for(int j=0; j<nEntries; j++) {
            treeArray_cal3[i]->GetEntry(j);
            
            //if(j<5) std::cout << "val[1] = " << val[1] << std::endl;
            //if(j<5) std::cout << "value = " << value << std::endl;
            
            fillval = slp*double(val[1])+offset;
            fillval = random.Gaus(fillval,hists3[i]->GetBinWidth(0)); 
            
            if(val[5]>tofCutLow3[i] && val[5]<tofCutHigh3[i] && val[1]>10 && fillval<phCutHigh3[i]*1.19) hists3[i]->Fill(fillval);
            if(val[5]>bkgCutLow3[i] && val[5]<bkgCutHigh3[i] && fillval<phCutHigh3[i]*1.19) hists3bkg[i]->Fill(fillval);
            //if(val[5]>tofCutLow3[i] && val[5]<tofCutHigh3[i] && val[1]>10) hists3[i]->Fill(fillval);
        }   
        for(int k=0; k<hists3[i]->FindBin(1); k++) hists3[i]->SetBinContent(k,0);
        
        double scale = (tofCutHigh3[i]-tofCutLow3[i])/(bkgCutHigh3[i]-bkgCutLow3[i]);
        //hists3bkg[i]->Scale(scale);
        hists3bkg[i]->Sumw2(); hists3[i]->Sumw2();
        hists3bkgsub[i] = (TH1F*)hists3[i]->Clone();
        hists3bkgsub[i]->Add(hists3bkg[i],-1*scale);    
        
        outfile->cd();
        hists3bkgsub[i]->Write(Form("ProtonCal%d",i+nFiles[0]+nFiles[1]+nFiles[2]));
        hists3[i]->Write(Form("ProtonCal_wbkg%d",i+nFiles[0]+nFiles[1]+nFiles[2]));
        hists3bkg[i]->Write(Form("BkgCal%d",i+nFiles[0]+nFiles[1]+nFiles[2]));
        std::cout << " \t written to ProtonCal" << nFiles[0]+nFiles[1]+nFiles[2]+i << " and BkgCal" << nFiles[0]+nFiles[1]+nFiles[2]+i << std::endl;
    }    

    for(int i=0; i<nFiles[0]; i++) { delete hists0[i]; delete hists0bkg[i]; delete hists0bkgsub[i]; delete treeArray_cal0[i]; hists0[i] = hists0bkg[i] = hists0bkgsub[i] = NULL; treeArray_cal0[i] = NULL;}
    delete[] hists0; delete[] hists0bkg; delete[] hists0bkgsub; delete[] treeArray_cal0; hists0 = hists0bkg = hists0bkgsub = NULL; treeArray_cal0 = NULL;

    for(int i=0; i<nFiles[1]; i++) { delete hists1[i]; delete hists1bkg[i]; delete hists1bkgsub[i]; delete treeArray_cal1[i]; hists1[i] = hists1bkg[i] = hists1bkgsub[i] = NULL; treeArray_cal1[i] = NULL;}
    delete[] hists1; delete[] hists1bkg; delete[] hists1bkgsub; delete[] treeArray_cal1; hists1 = hists1bkg = hists1bkgsub = NULL; treeArray_cal1 = NULL;

    for(int i=0; i<nFiles[2]; i++) { delete hists2[i]; delete hists2bkg[i]; delete hists2bkgsub[i]; delete treeArray_cal2[i]; hists2[i] = hists2bkg[i] = hists2bkgsub[i] = NULL; treeArray_cal2[i] = NULL;}
    delete[] hists2; delete[] hists2bkg; delete[] hists2bkgsub; delete[] treeArray_cal2; hists2 = hists2bkg = hists2bkgsub = NULL; treeArray_cal2 = NULL;

    for(int i=0; i<nFiles[3]; i++) { delete hists3[i]; delete hists3bkg[i]; delete hists3bkgsub[i]; delete treeArray_cal3[i]; hists3[i] = hists3bkg[i] = hists3bkgsub[i] = NULL; treeArray_cal3[i] = NULL;}
    delete[] hists3; delete[] hists3bkg; delete[] hists3bkgsub; delete[] treeArray_cal3; hists3 = hists3bkg = hists3bkgsub = NULL; treeArray_cal3 = NULL;

    //outfile->Close();
    

    //return treeArray_cal0;
    //return calArrayD;    
    //return hists3;

    return 0;

}
