#ifndef RK_FitFunctions
#define RK_FitFunctions

Double_t SingleGaussPlusConstant(Double_t* x, Double_t* params)
{
    Double_t T_a = (x[0]-params[1])/params[2];
    Double_t G_a =params[3]+(params[0]*exp(-0.5*( T_a *T_a)));
    return G_a;
}


Double_t ConstantMinusGauss(Double_t* x, Double_t* params)
{
    Double_t T_a = (x[0]-params[1])/params[2];
    
    Double_t G_a =params[0]*(1-exp(-0.5*( T_a *T_a)))   + params[3]; //Param 3 is a constant for minima (visibility minima)
    return G_a;
}



#endif




