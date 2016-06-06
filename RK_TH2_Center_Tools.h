#ifndef RK_TH2_Center_Tools
#define RK_TH2_Center_Tools

#include <TH1D.h>
#include <TH2.h>

void TH2_MoveOriginToCenter(TH2* h){
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    cout<<"--------------------------------------"<<endl;
    cout<<"RECENTERING IMAGE TO ZERO ZERO"<<endl;
    
    //Change histogram range to see it centered in zero
    h->GetXaxis()->Set(BinX,-BinX/2, BinX/2 -1);
    h->GetYaxis()->Set(BinY,-BinY/2, BinY/2 -1);
    
    return;
}

//Common methode to recenter
void TH2_RecenterImage(TH2* h, int OffsetX, int OffsetY){ //attention ici on specifie juste une translation en pixel
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    cout<<"--------------------------------------"<<endl;
    cout<<"RECENTERING IMAGE"<<endl;
    cout<<"Recenter applied offset (x,y): ("<< OffsetX <<" , "<<OffsetY<<")"<<endl;
    
    //Copy histogram
    TH2D* tempHist = new TH2D("tempHist","tempHist",BinX,0,BinX-1, BinY, 0, BinY-1);
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            tempHist->SetBinContent(itX,itY,h->GetBinContent(itX,itY));
        }
    }
    
    //Erase and fill with 0 to avaoid ghost after translation
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            h->SetBinContent(itX ,itY, 0);
        }
    }
    
    //Apply translation
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            //The if to avoid using out of range values
            if(((itX-OffsetX)>=0) && ((itX-OffsetX)<BinX) && ((itY - OffsetY)>=0) && ((itY - OffsetY)<BinY)){
                h->SetBinContent(itX - OffsetX ,itY - OffsetY, tempHist->GetBinContent(itX,itY));
            }
        }
    }
    
    tempHist->Delete();
    gDirectory->Delete("tempHist");
    
    return;
}

//Not very accurate
void TH2_RecenterImageByMean(TH2* h){
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    int MeanX = h->GetMean(1);
    int MeanY = h->GetMean(2);
    
    int MeanXbin = h->GetXaxis()->FindBin(MeanX);
    int MeanYbin = h->GetYaxis()->FindBin(MeanY);
    
    int OffsetX = MeanXbin - (BinX/2);
    int OffsetY = MeanYbin - (BinY/2);
    
    cout<<"MeanX: "<<MeanX<<endl;
    cout<<"MeanY: "<<MeanY<<endl;
    
    TH2_RecenterImage(h, OffsetX, OffsetY);
    return;
}


int FindCenterBinByMinimumBetweenPeaks(TH1D* h){
    int Maximum = h->GetMaximum();
    int BinCrossing[4];
    int CrossNumber =0;
    int BinX=h->GetXaxis()->GetNbins();
    
    bool FirstToPositive = false;
    bool FirstToNegative = false;
    bool SecondToPositive = false;
    bool SecondToNegative = false;
    
    for(int it =0; it<BinX; it++){
        if(!FirstToPositive && (h->GetBinContent(it)>(Maximum*0.5)) ){
            BinCrossing[CrossNumber]=it;
            FirstToPositive = true;
            CrossNumber++;
        }
        else if(FirstToPositive && !FirstToNegative && ( h->GetBinContent(it) < (Maximum*0.2)) ){
            BinCrossing[CrossNumber]=it;
            FirstToNegative = true;
            CrossNumber++;
        }
        else if(FirstToPositive && FirstToNegative && !SecondToPositive &&( h->GetBinContent(it) > (Maximum*0.5)) ){
            BinCrossing[CrossNumber]=it;
            SecondToPositive = true;
            CrossNumber++;
        }
        
    }
    
    cout<<"Crossings: "<<BinCrossing[1]<<" "<<BinCrossing[2]<<endl<<endl;
    
    int Minima = Maximum;
    int BinMinimum =0;
    for(int it =BinCrossing[1]; it<BinCrossing[2]; it++){
        if(h->GetBinContent(it) < Minima){
            Minima = h->GetBinContent(it);
            BinMinimum = it;
        }
    }
    
    cout<<"Minimium "<<Minima<<" found at bin: "<<BinMinimum<<endl;
    return BinMinimum;
}



int FindCenterBinByMinimumBetweenPeaksAndFit(TH1D* h){
    int Maximum = h->GetMaximum();
    int BinCrossing[4];
    int CrossNumber =0;
    int BinX=h->GetXaxis()->GetNbins();
    
    bool FirstToPositive = false;
    bool FirstToNegative = false;
    bool SecondToPositive = false;
    bool SecondToNegative = false;
    
    for(int it =0; it<BinX; it++){
        if(!FirstToPositive && (h->GetBinContent(it)>(Maximum*0.5)) ){
            BinCrossing[CrossNumber]=it;
            FirstToPositive = true;
            CrossNumber++;
        }
        else if(FirstToPositive && !FirstToNegative && ( h->GetBinContent(it) < (Maximum*0.2)) ){
            BinCrossing[CrossNumber]=it;
            FirstToNegative = true;
            CrossNumber++;
        }
        else if(FirstToPositive && FirstToNegative && !SecondToPositive &&( h->GetBinContent(it) > (Maximum*0.5)) ){
            BinCrossing[CrossNumber]=it;
            SecondToPositive = true;
            CrossNumber++;
        }
        
    }
    
    cout<<"Crossings bin: "<<BinCrossing[1]<<" "<<BinCrossing[2]<<endl<<endl;
    
    int Minima = Maximum;
    int BinMinimum =0;
    for(int it =BinCrossing[1]; it<BinCrossing[2]; it++){
        if(h->GetBinContent(it) < Minima){
            Minima = h->GetBinContent(it);
            BinMinimum = it;
        }
    }
    
    //Fit
    
    cout<<"Crossings value: "<<h->GetBinCenter(BinCrossing[1])<<" "<<h->GetBinCenter(BinCrossing[2])<<endl<<endl;
    float range = h->GetBinCenter(BinCrossing[2]) - h->GetBinCenter(BinCrossing[1]);
    float center = h->GetBinCenter(BinMinimum);
    
    cout<<"range: "<<range<<endl;
    cout<<"center: "<<center<<endl;
    
    TF1 *fitGaussX = new TF1("fitGaussip",ConstantMinusGauss,center - range/2. , center + range/2.,4);
    fitGaussX->SetParLimits (0,h->GetMaximum()/2. , h->GetMaximum()); //Max de la gaussienne
    fitGaussX->SetParLimits (1,center - range/4. , center + range/4.); //Mean position
    fitGaussX->SetParLimits (2,0, range); //Sigma
    fitGaussX->SetParLimits (3,0, h->GetMaximum()/2.); //Sigma
    
    fitGaussX->SetParameters(h->GetMaximum(),center,range/2,0);
    h->Fit(fitGaussX,"","",center - range/2. , center + range/2.);
    cout<<"Minimium gauss: "<<fitGaussX->GetParameter(1)<<endl;
    cout<<"Minimium basic: "<<h->GetBinCenter(BinMinimum)<<endl;
    
    cout<<"Minimium "<<Minima<<" found at bin: "<<BinMinimum<<endl;
    return BinMinimum;
}



int FindCenterBin_GaussianPlusConstantFit(TH1D* h, float center = 0., float range =0.){
    
    if((center == 0) && (range ==0)){
        range = h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin();
        center = h->GetXaxis()->GetXmin() + range/2;
    }
    
    cout<<"Range: "<<range<<endl;
    cout<<"center: "<<center<<endl;
    
    TF1 *fitGaussX = new TF1("fitGaussX",SingleGaussPlusConstant, center - (range/2.), center + (range/2.), 4);
    
    cout<<"Max: "<< h->GetMaximum()<<endl;
    fitGaussX->SetParLimits (0,h->GetMaximum()/2., h->GetMaximum()); //Amplitude gauss
    fitGaussX->SetParLimits (1,center - (range/4.), center + (range/4.)); //Mean position
    fitGaussX->SetParLimits (2,0, range/2.); //Sigma
    fitGaussX->SetParLimits (3,0 , h->GetMaximum()); //Constant baseline
    
    fitGaussX->SetParameters(h->GetMaximum(),center,range/10.,0);
    h->Fit(fitGaussX,"","",center-range/2.,center+range/2.);
    
    double centerBin =fitGaussX->GetParameter(1);
    
    centerBin = h->GetXaxis()->FindBin(centerBin);
    
    cout<<"Center found at bin: "<<centerBin<<endl;
    fitGaussX->Delete();
    
    return (int) centerBin;
}




#endif
