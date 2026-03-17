#define WaitUART 10 //ms
#define bufRxSize 14

float readDistance(){
  unsigned char b;
  uint8_t bufRx[bufRxSize];
  union buf{ //reverse order Hi,Lo
    float D;
    struct {uint8_t B[4];};
  };//union
  buf t;
  for (int i=0; i<WaitUART; i++) {
    if (hSerial.available()==bufRxSize) break;
    delayMicroseconds(1000); //1ms
  }//for i
  for (int r=0; r<bufRxSize; r++) bufRx[r]=hSerial.read();
  //for (int r=0; r<bufRxSize; r++) {Serial. print(bufRx[r],HEX); Serial. print(" ");} // Debug
  for (int i=0; i<4; i++) t.B[i]=bufRx[6+i];
  if (LogEnable) {
    Serial.print("D= "); Serial.print(t.D); 
    Serial.println();
  }//if Log  
  return (t.D);
}//readDistance