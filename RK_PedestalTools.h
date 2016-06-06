#ifndef RK_PedestalTools
#define RK_PedestalTools

#include <TH1D.h>
#include <TH2.h>

TH1D* DynamicRange(TH2* h, int MaxDynRange = 65535){
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    TH1D* hDynamicRange = new TH1D("hDynamicRange","Dynamic Range",MaxDynRange,0,MaxDynRange-1);
    for(int i =0; i<BinX; i++){
        for(int j =0; j<BinY; j++){
            if(h->GetBinContent(i,j)!=0)
                hDynamicRange->Fill(h->GetBinContent(i,j));
        }
    }
    return hDynamicRange;
}
void DynamicRange_CleanPointer(TH1D *hist){
    hist->Delete();
    gDirectory->Delete("hDynamicRange");
}


void TH2_PedestalSubstraction_FitDynRange(TH2* h, int Sigmas = 5){
    cout<<"--------------------------------------"<<endl;
    cout<<"PEDESTAL SUBSTRACTION "<<Sigmas<<" SIGMA"<<endl;
    
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    //Fill dynamic range
    TH1D* hDynamicRange;
    int MaxDynRange = pow(2, 16);
    hDynamicRange = DynamicRange(h, MaxDynRange);
    
    int MaxBin = hDynamicRange->GetMaximumBin();
    TF1 *fitGaus = new TF1("fitGaus","gaus",0,1.5*MaxBin);
    hDynamicRange->Fit("fitGaus","R");
    double Mean = fitGaus->GetParameter(1);
    double Sigma = fitGaus->GetParameter(2);
    double Threshold = Mean + Sigma*Sigmas;
    cout<<"Mean: "<<Mean<<endl;
    cout<<"Sigma: "<<Sigma<<endl;
    cout<<"Threshold: "<<Threshold<<endl;
    
#ifdef CONTROL_PLOTS
    TCanvas* canvDynRange= new TCanvas("canvDynRange","canvDynRange",800,600,800,600);
    canvDynRange->cd();
    hDynamicRange->Draw();
    canvDynRange->SetLogy();
    hDynamicRange->GetXaxis()->SetRangeUser(0,200);
    canvDynRange->Print("./Control_DynamicRange.png");
#endif
    DynamicRange_CleanPointer(hDynamicRange);

    //Apply pedestal substraction
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            int newvalue = h->GetBinContent(itX,itY)-Threshold;
            if(newvalue<0) newvalue=0;
            h->SetBinContent(itX,itY,newvalue);
        }
    }
    return;
}


void TH2_PedestalSubstraction_Manual(TH2* h, int Threshold = 0){
    cout<<"--------------------------------------"<<endl;
    cout<<"PEDESTAL SUBSTRACTION Manual THreshold: "<<Threshold<<endl;
    
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    //Fill dynamic range
    TH1D* hDynamicRange;
    int MaxDynRange = pow(2, 16);
    hDynamicRange = DynamicRange(h, MaxDynRange);
    
#ifdef CONTROL_PLOTS
    TCanvas* canvDynRange= new TCanvas("canvDynRange","canvDynRange",800,600,800,600);
    canvDynRange->cd();
    hDynamicRange->Draw();
    canvDynRange->SetLogy();
    hDynamicRange->GetXaxis()->SetRangeUser(0,200);
    canvDynRange->Print("./Control_DynamicRange.png");
#endif
    DynamicRange_CleanPointer(hDynamicRange);
    
    //Apply pedestal substraction
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            int newvalue = h->GetBinContent(itX,itY)-Threshold;
            //cout<<"New value: "<<newvalue<<endl;
            if(newvalue<0) newvalue=0;
            h->SetBinContent(itX,itY,newvalue);
        }
    }
    return;
}


#endif




