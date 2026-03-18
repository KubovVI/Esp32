// float *Rdat    [in, out] - Real part of Input and Output Data 
// float *Idat    [in, out] - Imaginary part of Input and Output Data 
// int    N       [in]      - Input and Output Data length 
//          N    = 4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536;
//          Lg2N = 2,3, 4, 5, 6,  7,  8,  9,  10,  11,  12,  13,   14,   15,   16;

#define maxLg2N 15
float Rcoef[maxLg2N]; float Icoef[maxLg2N];
int Lg2N;

void initFFT(int N){ 
  int i,j,n;
  float Fi=M_PI;
  for (i=0; i<maxLg2N; i++) { Icoef[i]=-sin(Fi); Rcoef[i]=cos(Fi); 
    Fi=Fi/2; 
  }//for i 
  j=N; for (i=0; i<maxLg2N; i++){ j>>=1; if (j==0) break; }//Lg2N
  Lg2N=i;
}//initFFT

void doFFT(float *Rdat, float *Idat, int N) { // time=41ms for N=1024
  int i,j,n,k,io,ie,in,nn;
  float ru,iu,rtp,itp,rtq,itq,rw,iw,sr;
  nn=N>>1; ie=N;
  for(n=1; n<=Lg2N; n++) {
    rw=Rcoef[Lg2N-n]; iw=Icoef[Lg2N-n];
    in=ie>>1; ru=1.0F; iu=0.0F;
    for(j=0; j<in; j++) {
      for(i=j; i<N; i+=ie) {
        io=i+in;
        rtp=Rdat[i]+Rdat[io]; itp=Idat[i]+Idat[io];
        rtq=Rdat[i]-Rdat[io]; itq=Idat[i]-Idat[io];
        Rdat[io]=rtq*ru-itq*iu; Idat[io]=itq*ru+rtq*iu;
        Rdat[i]=rtp; Idat[i]=itp;
      }//for i
      sr=ru; ru=ru*rw-iu*iw; iu=iu*rw+sr*iw;
    }//for j
    ie>>=1;
  }//for n
  for(j=i=1; i<N; i++) {
    if(i<j) {
      io=i-1; in=j-1;
      rtp=Rdat[in]; itp=Idat[in];
      Rdat[in]=Rdat[io]; Idat[in]=Idat[io];
      Rdat[io]=rtp; Idat[io]=itp;
    }//if i
    k=nn;
    while(k<j) { j=j-k; k>>=1; }
    j=j+k;
  }//for i
}//doFFT
