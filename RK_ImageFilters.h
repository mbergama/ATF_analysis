#ifndef RK_ImageFilters
#define RK_ImageFilters

#include <TH1D.h>
#include <TH2.h>

 void Kill_Particles(TH2* h,double ratio){ //TRy to remove the saturated pixels dues to particles crossing the CCD
 int BinX, BinY;
 BinX=h->GetXaxis()->GetNbins();
 BinY=h->GetYaxis()->GetNbins();
 unsigned int MaxValue = h->GetMaximum();
 
 int tempVal =0;
 int ScanDistance =3;
     
 for (int itX=ScanDistance; itX<(BinX-ScanDistance); ++itX) {
     for (int itY=ScanDistance; itY<(BinY-ScanDistance); ++itY) {
         tempVal = h->GetBinContent(itX,itY);
 
         //if(tempVal > (MaxValue/2)){//Premier securité pour gagner du temps
         
         //Test les 4 bandes a distance 10 pixels (ajouter securité pour  pas depasser le range de l'histo)
         bool TopTest =false;
         for (int itXLoc=itX-ScanDistance; itXLoc<itX+ScanDistance; ++itXLoc) {
             if((tempVal/ratio) > h->GetBinContent(itXLoc,itY+ScanDistance)) TopTest = true;
         }
         bool BotTest =false;
         for (int itXLoc=itX-ScanDistance; itXLoc<itX+ScanDistance; ++itXLoc) {
             if((tempVal/ratio) > h->GetBinContent(itXLoc,itY-ScanDistance)) BotTest = true;
         }
         
         bool LeftTest =false;
         for (int itYLoc=itY-ScanDistance; itYLoc<itY+ScanDistance; ++itYLoc) {
             if((tempVal/ratio) > h->GetBinContent(itX-ScanDistance,itYLoc)) LeftTest = true;
         }
         bool RightTest =false;
         for (int itYLoc=itY-ScanDistance; itYLoc<itY+ScanDistance; ++itYLoc) {
             if((tempVal/ratio) > h->GetBinContent(itX+ScanDistance,itYLoc)) RightTest = true;
         }
         
         if(TopTest && BotTest && LeftTest && RightTest){
             //Clean Pixel
             h->SetBinContent(itX,itY, 0);
             
             //Clean whole area (too violent)
             //for (int itXrm=(itX-ScanDistance); itXrm<(itX+ScanDistance); ++itXrm) {
             //    for (int itYrm=(itY-ScanDistance); itYrm<(itY+ScanDistance); ++itYrm) {
             //        h->SetBinContent(itXrm,itYrm, 0);
             //    }
             //}
         }
     }
 }
 return;
 
 }

//Marche mieux
void Kill_ParticlesAverage(TH2* h,double ratio){ //TRy to remove the saturated pixels dues to particles crossing the CCD
    int BinX, BinY;
    BinX=h->GetXaxis()->GetNbins();
    BinY=h->GetYaxis()->GetNbins();
    unsigned int MaxValue = h->GetMaximum();
    
    int tempVal =0;
    int ScanDistance =5;
    
    for (int itX=ScanDistance; itX<(BinX-ScanDistance); ++itX) {
        for (int itY=ScanDistance; itY<(BinY-ScanDistance); ++itY) {
            
            
            //if(tempVal > (MaxValue/2)){//Premier securité pour gagner du temps
            
            //Test les 4 bandes a distance 10 pixels (ajouter securité pour  pas depasser le range de l'histo)
            double cntVal =0;
            double SumVal = 0;
            
            for (int itXLoc=itX-ScanDistance; itXLoc<itX+ScanDistance; ++itXLoc) {
                SumVal+=h->GetBinContent(itXLoc,itY+ScanDistance);
                cntVal++;
            }
            for (int itXLoc=itX-ScanDistance; itXLoc<itX+ScanDistance; ++itXLoc) {
                SumVal+=h->GetBinContent(itXLoc,itY-ScanDistance);
                cntVal++;
            }
            for (int itYLoc=itY-ScanDistance; itYLoc<itY+ScanDistance; ++itYLoc) {
                SumVal+=h->GetBinContent(itX-ScanDistance,itYLoc);
                cntVal++;
            }

            for (int itYLoc=itY-ScanDistance; itYLoc<itY+ScanDistance; ++itYLoc) {
                SumVal+=h->GetBinContent(itX+ScanDistance,itYLoc);
                cntVal++;
            }
            
            SumVal = SumVal/cntVal;//Average
            tempVal = (h->GetBinContent(itX,itY) / ratio);
            
            if(tempVal>SumVal){
                //Clean Pixel
               //h->SetBinContent(itX,itY, 0);
                
                //Clean whole area (too violent)
                for (int itXrm=(itX-ScanDistance); itXrm<(itX+ScanDistance); ++itXrm) {
                    for (int itYrm=(itY-ScanDistance); itYrm<(itY+ScanDistance); ++itYrm) {
                        h->SetBinContent(itXrm,itYrm, 0);
                    }
                }
            }
        }
    }
    return;
    
}

#endif




