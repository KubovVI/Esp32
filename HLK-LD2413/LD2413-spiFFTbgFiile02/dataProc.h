#define FFTsize 1024 
#define distSize 512
#define numbBGavrg 10

float AmpRe[FFTsize]; float AmpIm[FFTsize];
float bgRe[FFTsize]; float bgIm[FFTsize];
float Window[FFTsize];
float Amp2[FFTsize]; 
float AmpdB[FFTsize];
int8_t AdB[FFTsize+distSize];

void initBg(int N){
  for (int n=0; n<N; n++){ bgRe[n]=0.0; bgIm[n]=0.0; }  
}//initBg

void initWindow(int winSize){
  int i;
  float step=M_PI/(winSize-1);
  if (LogPreFFTenable) Serial.println("Window");
  for (i=0; i<winSize; i++) { Window[i]=sin(i*step);
    if (LogPreFFTenable) {Serial.print(i); Serial.print("\t"); Serial.println(Window[i],2);}
  }//for i
}//initWindow

void PowFFT(float *Rdat,float *Idat,int N, float *Pdat, int D){ // time=3ms for N=1024
  float norm=4.0/(N*N);
  for (int i=0; i<D; i++){ Pdat[i]=(Rdat[i]*Rdat[i]+Idat[i]*Idat[i])*norm; }
}//PowFFT

float dBrough2(float alpha){ //Max Error -0.023dB time=9ms for N=1024
  if (alpha<=0.0f) return(-9999.99F);
  float a=alpha; int exponent;
  float value=frexpf(a, &exponent);
  float y=(value-1.0F)*(2.0F-1.386F*(value-0.5F))+exponent; //0.5<value<1.0
  return y*3.010299956639812f;
}//dBrough2

float dBrough1(float alpha){ //Max Error -0.26dB time=9ms for N=1024
  if (alpha<=0.0f) return(-9999.99F);
  float a=alpha; int exponent;
  float value=frexpf(a, &exponent);
  float y=value*2-2.0f+exponent; //0.5<value<1.0
  return y*3.010299956639812f;
}//dBrough1

void dB_FFT8(float *Rdat,float *Idat,int N, int8_t *AmpdB8, int D, int D2 ){ // time=7ms for N=1024 Error=0.25dB
  float norm=dBrough1(4.0/(N*2));
  float x;
  int i,j;
  for (i=0; i<D; i++){ 
    x=dBrough1(Rdat[i]*Rdat[i]+Idat[i]*Idat[i])+norm;
    if (x<-128) x=-128; if (x>127) x=127;
      AmpdB8[i]=(int8_t)x;
  }//for i
  norm=dBrough1(2.0/(N*2)); // 1/2
  for (i=0; i<D2; i++){j=D-1-i;
    x=Rdat[i]*Rdat[i]+Idat[i]*Idat[i];
    x+=Rdat[j]*Rdat[j]+Idat[j]*Idat[j]; //mirrored from end
    x=dBrough1(x)+norm;
    if (x<-128) x=-128; if (x>127) x=127;
      AmpdB8[D+i]=(int8_t)x;
  }//for i
}//dB_FFT

void preFFT(uint8_t *SPIbuf,int bufSize, float *Ar,float *Ai,int N, bool WinOn){
  buf4byte t0,t1;
  int i0,i1;
  int blockSize=bufSize/2;
  if (blockSize<(N+2)*4) {Serial.println("preFFT Error"); return;}
  i0=4; i1=i0+hBufferSize;
  if (LogPreFFTenable) Serial.println("winData");
  for (int n=0; n<N; n++){
    for (int k=0; k<4; k++) { t0.B[3-k]=SPIbuf[i0+k]; t1.B[3-k]=SPIbuf[i1+k]; }
    Ar[n]=t0.Q+t1.Q; Ai[n]=t0.I+t1.I;
    if (WinOn) { Ar[n]=Ar[n]*Window[n]; Ai[n]=Ai[n]*Window[n];}
    i0+=4; i1+=4;
    if (LogPreFFTenable) { Serial.print(n); Serial.print("\t"); 
      Serial.print(Ar[n],0); Serial.print("\t"); Serial.println(Ai[n],0);
    }//if LogFFTenable
  }//for n
}//preFFT

void getAiq(uint8_t *SPIbuf,int bufSize, float *Ar,float *Ai,int N){
 buf4byte t0,t1;
  int i0,i1;
  int blockSize=bufSize/2;
  if (blockSize<(N+2)*4) {Serial.println("getAiq Error"); return;}
  i0=4; i1=i0+hBufferSize;
  for (int n=0; n<N; n++){
    for (int k=0; k<4; k++) { t0.B[3-k]=SPIbuf[i0+k]; t1.B[3-k]=SPIbuf[i1+k]; }
    Ar[n]=t0.Q+t1.Q; Ai[n]=t0.I+t1.I;
    i0+=4; i1+=4;
  }//for n
}//getAiq

void removeBg(float *Ar,float *Ai,int N){
  for (int n=0; n<N; n++){ Ar[n]-=bgRe[n]; Ai[n]-=bgIm[n]; }
}//removeBg

void winAiq(float *Ar,float *Ai,int N){
  for (int n=0; n<N; n++){ Ar[n]*=Window[n]; Ai[n]*=Window[n]; }
}//winAiq

void printFFTdB(float *AdB, int N){
  for (int n=0; n<N; n++){
    Serial.print(n); Serial.print("\t"); Serial.println(AdB[n],0); 
  }//for n
}//printFFTdB

void printFFTdB8(int8_t *AmpdB8,  int N){
  for (int n=0; n<N; n++){
    Serial.print(n); Serial.print("\t"); Serial.println(AmpdB8[n]); 
  }//for n
}//printFFTdB8

void printBG(float *Ar,float *Ai,int N){
  Serial.println("--- BG data ---");
  for (int n=0; n<N; n++){
    Serial.print(n); Serial.print("\t"); Serial.print(Ar[n],0); 
    Serial.print("\t"); Serial.print(Ai[n],0); Serial.println();
  }//for n
}//printBG

int NumbSets=0;
void writeDataFile(const char *path, float *Re,float *Im,int N);

void procAiqFile(int Navrg){
  int n;
  if (NumbSets==0) {
    if (LogBGenable) Serial.println("*** start collect BG data ***");
    for (n=0; n<FFTsize; n++){ bgRe[n]=AmpRe[n]; bgIm[n]=AmpIm[n]; }   
  } else {
    for (n=0; n<FFTsize; n++){ bgRe[n]+=AmpRe[n]; bgIm[n]+=AmpIm[n]; } 
    if (LogBGenable) {Serial.print("*** data #"); Serial.println(NumbSets);} 
  }// else  
  NumbSets++;
  if (NumbSets>=Navrg) {
    float invN=1.0/NumbSets;
    for (n=0; n<FFTsize; n++){ bgRe[n]*=invN; bgIm[n]*=invN; }
    if (LogBGenable) Serial.println("*** stop collect BG data ***");    
    NumbSets=0;
    saveAiqFile=0; //stop
    if (LogBGenable) printBG(bgRe,bgIm,FFTsize); 
    writeDataFile(FileName, bgRe,bgIm,FFTsize); 
  }//if NumbSets>Navrg
}//procAiqFile
