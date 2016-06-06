#include <TH1F.h>
#include <TF1.h>
#include <TH2.h>
#include <TProfile.h>
#include "TASImage.h"
#include <TMath.h>
#include <TSystemDirectory.h>
#include <TList.h>


void TH2_Print_Pedestal_Maximum_RangeXY(TH2* h){
    cout<<"--------------------------------------"<<endl;
    cout<<"PRINT Pedestal and XYRange "<<endl;
    
    int BinX, BinY, MaxValue;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    cout<<"Binx "<<BinX<<endl;
    cout<<"Biny "<<BinY<<endl;
    
    MaxValue = h->GetMaximum();
    cout<<"Max before pedestal: "<<MaxValue<<endl;
    
    
    
    int PedestalPeakPosition = 0;
    TH1D* hDynRange = new TH1D("hDynRangePed","Dynamic Range",h->GetMaximum(),0,h->GetMaximum()-1);
    //Dynamic range
    for(int i =0; i<BinX; i++){
        for(int j =0; j<BinY; j++){
            hDynRange->Fill(h->GetBinContent(i,j));
        }
    }
    PedestalPeakPosition = hDynRange->GetMaximumBin();
    hDynRange->Delete();
    
    cout<<"Pedestal peak position "<<PedestalPeakPosition<<endl;
    
    return;
}



void TH2_RotateImage(TH2* h,  float thetaRad= 0){
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    //Copy histogram
    TH2D* tempHist = new TH2D("tempHistRot","tempHistRot",BinX,0,BinX-1, BinY, 0, BinY-1);
    for (int row=0; row<BinX; ++row) {
        for (int col=0; col<BinY; ++col) {
            tempHist->SetBinContent(row,col,h->GetBinContent(row,col));
        }
    }
    //Apply rotation
    for (int row=0; row<BinX; ++row) {
        for (int col=0; col<BinY; ++col) {
            int binRelativeToCenterX = row - BinX/2;
            int binRelativeToCenterY = col - BinY/2;
            int oldbin_X = (float)binRelativeToCenterX*cos(thetaRad) + (float)binRelativeToCenterY*sin(thetaRad);
            int oldbin_Y = -binRelativeToCenterX*sin(thetaRad) + (float)binRelativeToCenterY*cos(thetaRad);
            h->SetBinContent(row, col, tempHist->GetBinContent(oldbin_X + BinX/2, oldbin_Y + BinY/2));
        }
    }
    tempHist->Delete();
    return;
}

void TH2_RotateImage_StdDev_X_Minimisation(TH2* h, float angleSpanDeg = 10, float angleStepsDeg = 0.1){
    
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    
    float BestStdDevX = h->GetStdDev(1);
    float BestIteration = 0;
    
    //Loop on deltaAngledeg to minimize the standard deviation
    float DegToRad = (TMath::Pi()/180.);
    
    //TH2_RotateImage(tempHist, - ((testRangeSteps/2)*deltaAngleRad) ); //Pour commencer a l'angle negatif
    
    int Min_angleDeg = -angleSpanDeg/2;
    int Max_angleDeg = angleSpanDeg/2;
    
    
    
    for(float angleDeg = Min_angleDeg ; angleDeg <Max_angleDeg; angleDeg = angleDeg + angleStepsDeg){
        
        //Copy histogram
        TH2D* tempHist = new TH2D("tempHistRotOptim","tempHistRotOptim",BinX,0,BinX-1, BinY, 0, BinY-1);
        for (int row=0; row<BinX; ++row) {
            for (int col=0; col<BinY; ++col) {
                tempHist->SetBinContent(row,col,h->GetBinContent(row,col));
            }
        }
        
        TH2_RotateImage(tempHist, (angleDeg) * DegToRad);
        if(tempHist->GetStdDev(1) < BestStdDevX){
            BestStdDevX = tempHist->GetStdDev(1);
            BestIteration = angleDeg;
        }
        tempHist->Delete();
        
    }
    cout<<"--------------------------------------"<<endl;
    cout<<"ROTATION OPIMISATION"<<endl;
    cout<<"Min , max: "<<Min_angleDeg<<" , "<<Max_angleDeg<<endl;
    cout<<"Steps: "<<  angleStepsDeg <<" deg"<<endl;
    cout<<"Initial Std Dev : "<<h->GetStdDev(1)<<endl;
    
    TH2_RotateImage(h, (BestIteration) * DegToRad);
    
    cout<<"Optimized Std Dev : "<<h->GetStdDev(1)<<endl;
    cout<<"Rotation correction : "<<BestIteration<<" deg"<<endl;
    
    return;
}

/*
TH1D* HorizontalProfileOfTheMaximums(TH2* h, int width =2, int Y_center_position = 0){
    //This function is supposed to project only the maximum height of bin found in the range
    //Using that I can know wich dynamic range was used but it's NOT and integrated profile (only max)
    
    //If not defined use center of the 2D histo
    //if(Y_center_position == 0){Y_center_position = (h->GetYaxis()->GetNbins())/2;}
    
    cout<<"--------------------------------------"<<endl;
    cout<<"HORIZONTAL PROFILE"<<endl;
    cout<<"Y_center_position: "<<Y_center_position<<endl;
    cout<<"Integration width: "<<width<<endl;
    
    TH1D* hProfileHorizontal = new TH1D("hProfileHorizontal","hProfileHorizontal",h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    
    int BinYmin_HorizontalProfile= h->GetYaxis()->FindBin(Y_center_position-(width/2));
    int BinYmax_HorizontalProfile= h->GetYaxis()->FindBin(Y_center_position+(width/2));
    
    for(int x = 0; x< h->GetXaxis()->GetNbins(); x++){
        int MaxValue =0;
        for(int y = BinYmin_HorizontalProfile; y<BinYmax_HorizontalProfile; y++){
            if(MaxValue< h->GetBinContent(x,y))MaxValue= h->GetBinContent(x,y);
        }
        hProfileHorizontal->SetBinContent(x,MaxValue);
    }
    return hProfileHorizontal;
}*/



void FitMaxAndBaseline(TH1D* h, FILE* theResultsFile = NULL){
    
    TF1 *fitGaussX = new TF1("fitGaussX",SingleGaussPlusConstant,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax(),4);
    fitGaussX->SetParLimits (0,0 , h->GetMaximum());
    
    float range = h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin();
    float center = range/2;
    
    fitGaussX->SetParLimits (1,center - range/10. , center + range/10.); //Mean position
    fitGaussX->SetParLimits (2,0, range/6.); //Sigma
    fitGaussX->SetParLimits (3,0 , h->GetMaximum());
    
    
    fitGaussX->SetParameters(10,center,h->GetRMS(),0);
    h->Fit(fitGaussX);
  
    double XMean =fitGaussX->GetParameter(1);
    double XSigma =fitGaussX->GetParameter(2);
    cout<<"Mean value: "<<XMean<<"+-"<<XSigma<<endl;
    cout<<"Max value: "<<fitGaussX->GetParameter(0)<<endl;
    cout<<"Constant: "<<fitGaussX->GetParameter(3)<<endl;
    cout<<"Max +cst value: "<<fitGaussX->GetParameter(0)+fitGaussX->GetParameter(3)<<endl;
    
    //theResultsFile
    //Baseline Max_with_baseline_included
    fprintf (theResultsFile,"%i \t %i \t %f \t", (int)(fitGaussX->GetParameter(3)), (int)(fitGaussX->GetParameter(0)+fitGaussX->GetParameter(3)),(float) fitGaussX->GetParameter(0)/fitGaussX->GetParameter(3));
    
    fitGaussX->Delete();

    
}

int list_files(const char *dirname="./", const char *ext=".fit", string* fList =NULL)
{
    int Nfiles =0;
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();
    if (files) {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        
        while ((file=(TSystemFile*)next())) {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.EndsWith(ext)) {
                //cout << fname.Data() << endl;
                fList[Nfiles] = fname.Data();
                (Nfiles)++;
            }
            
        }
    }
    return Nfiles;
    
}


int FindCenterBinByGaussianFit(TH1D* h){
    float range = h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin();
    cout<<"Range: "<<range<<endl;
    
    float center = h->GetXaxis()->GetXmin() + range/2;
    cout<<"center: "<<center<<endl;
    
    TF1 *fitGaussX = new TF1("fitGaussX",SingleGaussPlusConstant,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax(),4);
    
    cout<<"Max: "<< h->GetMaximum()<<endl;
    fitGaussX->SetParLimits (0,h->GetMaximum()/2., h->GetMaximum());
    fitGaussX->SetParLimits (1,h->GetXaxis()->GetXmin()/2. , h->GetXaxis()->GetXmax()/2.); //Mean position
    fitGaussX->SetParLimits (2,0, range/2.); //Sigma
    fitGaussX->SetParLimits (3,0 , (h->GetMaximum()/20.));
    
    fitGaussX->SetParameters(h->GetMaximum(),h->GetXaxis()->GetXmin()+center,range/10.,0);
    h->Fit(fitGaussX,"","",center-range/4.,center+range/4.);
    
    double centerBin =fitGaussX->GetParameter(1);
    
    centerBin = h->GetXaxis()->FindBin(centerBin);
    
    cout<<"Center found at bin: "<<centerBin<<endl;
    fitGaussX->Delete();

    return (int) centerBin;
}

int MaxOfTheGaussianFit(TH1D* h){
    TF1 *fitGaussX = new TF1("fitGaussX",SingleGaussPlusConstant,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax(),4);
    fitGaussX->SetParLimits (0,h->GetMaximum()/10., h->GetMaximum());
    
    float range = h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin();
    float center = range/2;
    fitGaussX->SetParLimits (1,h->GetXaxis()->GetXmin()/2. , h->GetXaxis()->GetXmax()/2.); //Mean position
    fitGaussX->SetParLimits (2,0, range/6.); //Sigma
    fitGaussX->SetParLimits (3,0 , h->GetMaximum()/10);
    fitGaussX->SetParameters(10,center,h->GetRMS(),0);
    h->Fit(fitGaussX);
    double MaxGaussian =fitGaussX->GetParameter(0);
    cout<<"Max Gaussian value : "<<MaxGaussian<<endl;
    fitGaussX->Delete();
    
    return (int) MaxGaussian;
}


void OptimiseDynamicRange_TopValue(TH2* h){ //TRy to remove the saturated pixels dues to particles crossing the CCD
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    //Take the profiles
    TH1D* hProfileHorizontal = new TH1D("hProfileHorizontaln","SliceX_Image",h->GetXaxis()->GetNbins(),0,h->GetXaxis()->GetNbins()-1);
    hProfileHorizontal = HorizontalProfileOfTheMaximums(h,200,0);
    int MaxTheshold = MaxOfTheGaussianFit(hProfileHorizontal) * 1.1 ; //Add 10% security
    delete hProfileHorizontal;
    
    //Copy hogram
    TH2D* tempHist = new TH2D("tempHist","tempHist",BinX,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax(), BinY, h->GetYaxis()->GetXmin(),h->GetYaxis()->GetXmax());
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            tempHist->SetBinContent(itX,itY,h->GetBinContent(itX,itY));
        }
    }
    //Clean histogram
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            h->SetBinContent(itX,itY, 0);
        }
    }
    //Remove the huges particles crossing
    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            if(tempHist->GetBinContent(itX,itY) < MaxTheshold){
                h->SetBinContent(itX ,itY , tempHist->GetBinContent(itX,itY));
            }
            else{
                h->SetBinContent(itX ,itY , 0);
            }
        }
    }
    tempHist->Delete();
    return;
    
}



void RemoveBackgroundUsingExternalFrame(TH2* h, int Xwidth, int Ywidth, double NSigma){
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    
    //Cadre
    int XLow = BinX/2 - Xwidth/2;
    int Xhigh = BinX/2 + Xwidth/2;
    int YLow = BinY/2 - Ywidth/2;
    int Yhigh = BinY/2 + Ywidth/2;
    
    //Take the profiles
    TH1D* hDynamicRange = new TH1D("hDynamicRange","hDynamicRange",pow(2,16),0,pow(2,16)-1); //16 bit maximum

    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            if(((itX<XLow)||(itX>Xhigh))&&((itY<YLow)||(itY>Yhigh))){ //Etre en dehors du cadre
                hDynamicRange->Fill(h->GetBinContent(itX,itY));
            }
        }
    }
    int CutSIgma = hDynamicRange->GetMaximumBin(); //Valeur ADC la plus souvent trouvée  pic de poisson
    int LowTheshold = CutSIgma * NSigma;
    
    cout<<"Pedestal found at : "<<CutSIgma<<endl;
    cout<<"Threshold set "<<NSigma<<" sigma at : "<<LowTheshold<<" DAC"<<endl;
    delete hDynamicRange;


    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            if((h->GetBinContent(itX,itY) - CutSIgma) >0){
                h->SetBinContent(itX,itY, h->GetBinContent(itX,itY)-CutSIgma);
            }
            else{h->SetBinContent(itX,itY, 0);}//evite de passer en negatif
        }
    }
   
    return;

    

}

//Recupere les profil speciaux avec le max trouvé sur chaque ligne chez X par exemple,
//fit gauss
//Trouve la valeur max de l'amplitude de la gaussienne
//Coupe tout les pixels qui sont superieurs au somet de la gaussienne + 5%





