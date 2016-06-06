#ifndef RK_TH2_Profiles_Tools
#define RK_TH2_Profiles_Tools

#include <TH1D.h>
#include <TH2.h>


TH1D* VerticalProfileFull(TH2* h){ //Attention la normalisation fait que c'est tres moche il faut surement l'enlever a tester sur le slice aussi
    cout<<"--------------------------------------"<<endl;
    cout<<"VERTICAL PROFILE FULL "<<endl;
    TH1D* hProfileYslice = new TH1D("hProfileYslice","SliceY_Image",h->GetYaxis()->GetNbins(),h->GetYaxis()->GetXmin(),h->GetYaxis()->GetXmax());
    for(int y = 0; y< h->GetYaxis()->GetNbins(); y++){
        int SumValue =0;
        for(int x = 0; x<h->GetXaxis()->GetNbins(); x++){
            SumValue+= h->GetBinContent(x,y);
        }
       // SumValue = SumValue / h->GetXaxis()->GetNbins(); //Normalisation du profile
        hProfileYslice->SetBinContent(y,SumValue);
    }
    return hProfileYslice;
}

TH1D* HorizontalProfileFull(TH2* h){
    cout<<"--------------------------------------"<<endl;
    cout<<"HORIZONTAL PROFILE FULL"<<endl;
    TH1D* hProfileXslice = new TH1D("hProfileXslice","SliceX_Image",h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    
    for(int x = 0; x< h->GetXaxis()->GetNbins(); x++){
        int SumValue =0;
        for(int y = 0; y<h->GetYaxis()->GetNbins(); y++){
            SumValue+= h->GetBinContent(x,y);
        }
       // SumValue = SumValue/h->GetYaxis()->GetNbins(); //Normalisation du profile
        hProfileXslice->SetBinContent(x,SumValue);
    }
    return hProfileXslice;
}


TH1D* VerticalProfileSlice(TH2* h, int width =2, int X_center_position = 0){
    cout<<"--------------------------------------"<<endl;
    cout<<"VERTICAL PROFILE"<<endl;
    cout<<"X_center_position: "<<X_center_position<<endl;
    cout<<"Integration width: "<<width<<endl;
    
    TH1D* hProfileYslice = new TH1D("hProfileYslice","SliceY_Image",h->GetYaxis()->GetNbins(),h->GetYaxis()->GetXmin(),h->GetYaxis()->GetXmax());
    int BinXmin_VerticalProfile= h->GetXaxis()->FindBin(X_center_position-(width/2));
    int BinXmax_VerticalProfile= h->GetXaxis()->FindBin(X_center_position+(width/2));
    
    for(int y = 0; y< h->GetYaxis()->GetNbins(); y++){
        int SumValue =0;
        for(int x = BinXmin_VerticalProfile; x<BinXmax_VerticalProfile; x++){
           SumValue+= h->GetBinContent(x,y);
        }
        //SumValue = SumValue/(BinXmax_VerticalProfile - BinXmin_VerticalProfile); //Normalisation du profile
        hProfileYslice->SetBinContent(y,SumValue);
    }

    return hProfileYslice;
}

TH1D* HorizontalProfileSlice(TH2* h, int width =2, int Y_center_position = 0){
    //If not defined use center of the 2D histo
    //if(Y_center_position == 0){Y_center_position = (h->GetYaxis()->GetNbins())/2;}
    
    cout<<"--------------------------------------"<<endl;
    cout<<"HORIZONTAL PROFILE"<<endl;
    cout<<"Y_center_position: "<<Y_center_position<<endl;
    cout<<"Integration width: "<<width<<endl;
    
    TH1D* hProfileXslice = new TH1D("hProfileXslice","SliceX_Image",h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    int BinYmin_HorizontalProfile= h->GetYaxis()->FindBin(Y_center_position-(width/2));
    int BinYmax_HorizontalProfile= h->GetYaxis()->FindBin(Y_center_position+(width/2));
    
    for(int x = 0; x< h->GetXaxis()->GetNbins(); x++){
        int SumValue =0;
        for(int y = BinYmin_HorizontalProfile; y<BinYmax_HorizontalProfile; y++){
            SumValue+= h->GetBinContent(x,y);
        }
        //SumValue = SumValue/(BinYmax_HorizontalProfile - BinYmin_HorizontalProfile); //Normalisation du profile
        hProfileXslice->SetBinContent(x,SumValue);
    }

    return hProfileXslice;
}

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
}


#endif




