#ifndef RK_ImageConversion_Tools
#define RK_ImageConversion_Tools

#include <TH1F.h>
#include <TF1.h>
#include <TH2.h>
#include <TProfile.h>
#include "TASImage.h"
#include <TMath.h>
#include <TSystemDirectory.h>
#include <TList.h>


//To convert a 16 bits FITS image to a TH2 (PCO Edge et Dicam PRo)
TH2* FITS_to_TH2(string PathAndDatafilename){
    
    // Open primary HDU from file
    const char* cstr_inFile =PathAndDatafilename.c_str();
    TFITSHDU *hdu = new TFITSHDU(cstr_inFile);
    if (hdu == 0) {printf("ERROR: could not access the HDU\n"); return 0;}
    // hdu->Print("F+"); //To see all header datas
    printf("Exposure time = %s\n", hdu->GetKeywordValue("EXPTIME").Data());
    
    TMatrixD *mat = hdu->ReadAsMatrix(0);
    UInt_t xPixels = mat->GetNcols();
    UInt_t yPixels = mat->GetNrows();
    cout<<"Image format Horizontal x Vertical: "<<xPixels<<" x "<<yPixels<<endl;

    
    TH2D* hImage = new TH2D("hImage","hImage",xPixels,0,xPixels-1,yPixels,0,yPixels-1);
    for (UInt_t itX=0; itX<xPixels; ++itX) {
        for (UInt_t itY=0; itY<yPixels; ++itY) {
            hImage->SetBinContent(itX,itY,(*mat)[itY][itX]);//Attention la matrice est inversee entre x et y je corrige ca en inversant les deux
        }
    }
    delete mat;
    return hImage;
}

//To convert a 12 bits FITS image to a TH2 (Proxotronic camera cornell)
TH2* FITS_to_TH2_12BIT(string PathAndDatafilename){
    
    // Open primary HDU from file
    const char* cstr_inFile =PathAndDatafilename.c_str();
    TFITSHDU *hdu = new TFITSHDU(cstr_inFile);
    if (hdu == 0) {printf("ERROR: could not access the HDU\n"); return 0;}
    // hdu->Print("F+"); //To see all header datas
    printf("Exposure time = %s\n", hdu->GetKeywordValue("EXPTIME").Data());
    
    TMatrixD *mat = hdu->ReadAsMatrix(0);
    UInt_t xPixels = mat->GetNcols(); //1390
    UInt_t yPixels = mat->GetNrows(); //1038
    cout<<"Image format Horizontal x Vertical: "<<xPixels<<" x "<<yPixels<<endl; //1038x1390
    
    
    TH2D* hImage = new TH2D("hImage","hImage",xPixels,0,xPixels-1,yPixels,0,yPixels-1);
    for (UInt_t itX=0; itX<xPixels; ++itX) {
        for (UInt_t itY=0; itY<yPixels; ++itY) {
            //Here we mask the 4 higher bit and read only the first 12bits
            //We need to mask them since they are set to 1 by default !!
            UInt_t a= (*mat)[itY][itX]; //Attention la matrice est inversee entre x et y je corrige ca en inversant les deux
            a= a&0xFFF;

            hImage->SetBinContent(itX,itY,a);
        }
    }
    
    delete mat;
    return hImage;
}


void FITS_to_TH2_CleanPointer(TH2 *hist){
    hist->Delete();
    gDirectory->Delete("hImage");
}


void SumHistos(TH2 *histSum ,TH2 *hist ){
    int BinX, BinY;
    BinX=hist->GetXaxis()->GetNbins();
    BinY=hist->GetYaxis()->GetNbins();

    for (int itX=0; itX<BinX; ++itX) {
        for (int itY=0; itY<BinY; ++itY) {
            histSum->SetBinContent(itX,itY,(hist->GetBinContent(itX,itY)+histSum->GetBinContent(itX,itY)));
        }
    }
    return;
}



#endif
