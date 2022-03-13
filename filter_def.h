#include "coef_tharaak.h"

#define buffers 512
#define taps_filter 120

float firP45(float NewSample){
  static float x[buffers/2]; float y = 0;  int k;
  for (k = buffers/2 - 1; k>0; k--)  x[k] = x[k - 1];
  x[0] = NewSample; 
  for (k = 0; k<taps_filter; k++)   
  y +=  tharaak_p45_65tap[k] * x[k]; 
   //y +=  tharaak_p45_100tap[k] * x[k]; 
  //  y +=  tharaak_p45_120tap[k] * x[k]; 
   //y +=  tharaak_p45_160tap[k] * x[k]; 
  return y;
}
float firM45(float NewSample){
  static float x[buffers/2]; float y = 0;  int n;
  for (n = buffers/2 - 1; n>0; n--)  x[n] = x[n - 1];
  x[0] = NewSample; 
  for (n = 0; n<taps_filter; n++)  
  y +=  tharaak_m45_65tap[n] * x[n]; 
  //y +=  tharaak_m45_100tap[n] * x[n];
  //  y +=  tharaak_m45_120tap[n] * x[n]; 
  //y +=  tharaak_m45_160tap[n] * x[n];
  return y;
}


/*---------------------------------------------------Low Pas Filter --------------------------------------------*/
#define NCoeflp  5
float iirlp(float NewSample) {
    float ACoef[NCoeflp+1] = {
        0.00006248961583259117,
        0.00031244807916295581,
        0.00062489615832591163,
        0.00062489615832591163,
        0.00031244807916295581,
        0.00006248961583259117
    };

    float BCoef[NCoeflp+1] = {
        1.00000000000000000000,
        -3.98454311961233690000,
        6.43486709027586820000,
        -5.25361517035226730000,
        2.16513290972413230000,
        -0.35992824506355597000
    };

    static float y[NCoeflp+1]; //output samples
    static float x[NCoeflp+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoeflp; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoeflp; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}
/*---------------------------------------------------Band Pas Filter --------------------------------------------*/
#define NCoef   4 
float iirbp1(float NewSample) {
    float ACoef[NCoef+1] = {
        0.00106356936315145090,
        0.00000000000000000000,
        -0.00212713872630290170,
        0.00000000000000000000,
        0.00106356936315145090
    };

    float BCoef[NCoef+1] = {
        1.00000000000000000000,
        -3.88130634755370620000,
        5.68704929289846020000,
        -3.72794504902690130000,
        0.92256587664469880000
    };

    static float y[NCoef+1]; //output samples
    static float x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}

float iirbp2(float NewSample) {
    float ACoef[NCoef+1] = {
        0.00061855045155176781,
        0.00000000000000000000,
        -0.00123710090310353560,
        0.00000000000000000000,
        0.00061855045155176781
    };

    float BCoef[NCoef+1] = {
        1.00000000000000000000,
        -3.86780446043318360000,
        5.68037714277528400000,
        -3.75262811572661410000,
        0.94134329942994244000
    };

    static float y[NCoef+1]; //output samples
    static float x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}

float iirbp3(float NewSample) {
    float ACoef[NCoef+1] = {
        0.00052900752511731472,
        0.00000000000000000000,
        -0.00105801505023462940,
        0.00000000000000000000,
        0.00052900752511731472
    };

    float BCoef[NCoef+1] = {
        1.00000000000000000000,
        -3.83208548192352220000,
        5.61160384925482260000,
        -3.71797278492666420000,
        0.94134329942636996000
    };

    static float y[NCoef+1]; //output samples
    static float x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}

float iirbp4(float NewSample) {
    float ACoef[NCoef+1] = {
        0.00058902808395192713,
        0.00000000000000000000,
        -0.00117805616790385430,
        0.00000000000000000000,
        0.00058902808395192713
    };

    float BCoef[NCoef+1] = {
        1.00000000000000000000,
        -3.92358443470576250000,
        5.78905209715058520000,
        -3.80674706148852640000,
        0.94134329943810735000
    };

    static float y[NCoef+1]; //output samples
    static float x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}
float iirbp5(float NewSample) {
    float ACoef[NCoef+1] = {
        0.00050774254988756760,
        0.00000000000000000000,
        -0.00101548509977513520,
        0.00000000000000000000,
        0.00050774254988756760
    };

    float BCoef[NCoef+1] = {
        1.00000000000000000000,
        -3.93237393168444880000,
        5.80631840457837530000,
        -3.81527482286820390000,
        0.94134329944021655000
    };

    static float y[NCoef+1]; //output samples
    static float x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}
float iirbp6(float NewSample) {
    float ACoef[NCoef+1] = {
        0.00056348399201553670,
        0.00000000000000000000,
        -0.00112696798403107340,
        0.00000000000000000000,
        0.00056348399201553670
    };

    float BCoef[NCoef+1] = {
        1.00000000000000000000,
        -3.93797065618559160000,
        5.81733288338746220000,
        -3.82070488686086350000,
        0.94134329944177586000
    };

    static float y[NCoef+1]; //output samples
    static float x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];
    
    return y[0];
}
