// Nologo ESP32C3 Super Mini
// 24GHz radar HLK-LD2413 Hi-Link HLK24R50K SPI Slave, UART
/*
  Board  | SPI      | MOSI| MISO| SCK | SS | Default
  esp32c3| HSPI/FSPI|  6  |  5  |  4  |  7 | Not                
*/
#define SS_PIN 7

// Serial - USB. hSerial - Rx-21; Tx-20
HardwareSerial hSerial(0);
#define RX 21
#define TX 20

#include <WiFi.h>
#include <WebServer.h>
//http://192.168.4.1
WebServer server(80);

bool LogEnable, LogSPIenable, LogPreFFTenable, LogFFTenable;
bool winFFTenable;
bool wsSPIenable, wsFFTenable;

#include "webSocProc.h"

#define ssid "LD2413"
#define pass "123456789"
#define LED 8

#define _HTMLsize 3000
String HTMLstring="";
float Distance=0.0;
String configStr="";

#include "DistanceHTML.h"
#include "readUART.h"
#include "readSPI.h"
#include "SPIdataHTML.h"
#include "FFT1.h"
#include "dataProc.h"
#include "FFTdataHTML.h"
#include "graphHTML.h"
#include "mapHTML.h"
#include "ctlHTML.h"
#include "rawHTML.h"

void handleRoot(){
  if (LogEnable) Serial.println("handleRoot");
  handleDist();
}//handleRoot

void handleDist() {
  digitalWrite(LED,0);
  DistanceHTML(); 
  HTMLstring.replace("#####.##",String(Distance,2));
  server.send(200,"text/html",HTMLstring);
  if (LogEnable) Serial.println("handleDist");
  digitalWrite(LED,1);
}//handlDist

void handleSPI() {
  digitalWrite(LED,0);
  SPIdataHTML(); 
  server.send(200,"text/html",HTMLstring);
  if (LogEnable) Serial.println("handleSPI");
  digitalWrite(LED,1);
}//handlSPI

void handleRAW() {
  digitalWrite(LED,0);
  rawHTML(); 
  server.send(200,"text/html",HTMLstring);
  if (LogEnable) Serial.println("handleRAW");
  digitalWrite(LED,1);
}//handlRAW

void handleFFT() {
  digitalWrite(LED,0);
  FFTdataHTML(); 
  server.send(200,"text/html",HTMLstring);
  if (LogEnable) Serial.println("handleFFT");
  digitalWrite(LED,1);
}//handleFFT

void handleGraph() {
  digitalWrite(LED,0);
  graphHTML(); 
  server.send(200,"text/html",HTMLstring);
  if (LogEnable) Serial.println("handleGraph");
  digitalWrite(LED,1);
}//handlGraph

void handleMap() {
  digitalWrite(LED,0);
  mapHTML(); 
  server.send(200,"text/html",HTMLstring);
  if (LogEnable) Serial.println("handleMap");
  digitalWrite(LED,1);
}//handlGraph

void handleCtl() {
  digitalWrite(LED,0);
  ctlHTML(); 
  server.send(200,"text/html",HTMLstring);
  if (LogEnable) Serial.println("handleCtl");
  digitalWrite(LED,1);
}//handlGraph

void handleNotFound() {
  digitalWrite(LED,0);
  server.send(404,"text/plain","Error: 404");
  if (LogEnable) Serial.println("handleNotFound");
  digitalWrite(LED,1);
}//handleNotFound

void decodeWS(String s){
  String mask="#w=";
  int p=s.indexOf(mask);
  if (p<0) return;
  s=s.substring(mask.length());
  if (s.charAt(0)=='0') winFFTenable=0; else winFFTenable=1;
}//decodeWS

void setup(void) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED,1);
  Serial.begin(115200);
  hSerial.begin(115200,SERIAL_8N1,RX,TX);
  //This needs only at first time to disable Station mode
  //WiFi.mode(WIFI_STA); WiFi.disconnect(); //Disable Station Mode
  delay(100);
  WiFi.softAP(ssid, pass); // set password if needs
 
  server.on("/", handleRoot);
  server.on("/d", handleDist);
  server.on("/spi", handleSPI);
  server.on("/raw", handleRAW);
  server.on("/fft", handleFFT);
  server.on("/gr", handleGraph);
  server.on("/map", handleMap);
  server.on("/ctl", handleCtl);
  server.onNotFound(handleNotFound);

  server.begin();
  ws.begin();
  ws.onEvent(wsEvent);
  setupSPI();
  HTMLstring.reserve(_HTMLsize);

  LogEnable=1; LogSPIenable=0; LogPreFFTenable=0; LogFFTenable=0;//0/1;
  winFFTenable=1;
  wsSPIenable=1; wsFFTenable=1;
  
  delay(2000);
  if (LogEnable) Serial.println("HTTP server started");
  initFFT(FFTsize);
  initWindow(FFTsize);
}//setup

void loop(void) {
  //uint8_t num=0; //single client
  unsigned char b;
  int spiLen;
  bool wsOk;
  server.handleClient();
  ws.loop();

  while(digitalRead(SS_PIN)==LOW){ //Wait for end of current block to start SPI read from begin
    while (hSerial.available()) b=hSerial.read(); //clear RxUART buffer
  }//while SS_PIN

  spiLen=readSPI();
  Distance=readDistance();

  if (LogEnable) Serial.println(spiLen);
  if (LogSPIenable) printSPI(spiLen);
   
  //wsOk=ws.sendBIN(num,(uint8_t*) &Distance,4); //Errors on disconnection
  wsOk=ws.broadcastBIN((uint8_t*) &Distance,4);
  if (!wsOk) {if (LogEnable) Serial.println("ws Dist Error!");}//if Ok 

  if (wsSPIenable){
    wsOk=ws.broadcastBIN((uint8_t*) rx_buf, BUFFER_SIZE);
    if (!wsOk) {if (LogEnable) Serial.println("ws SPI Error!");}//if Ok 
  }//if wsSPIenable
  
  configStr="win: "; if (winFFTenable) configStr+="Sine"; else configStr+="None";
  preFFT(rx_buf,BUFFER_SIZE, AmpRe,AmpIm,FFTsize, winFFTenable); //set data for FFT
  doFFT(AmpRe,AmpIm, FFTsize); // do FFT
  dB_FFT8(AmpRe,AmpIm,FFTsize, AdB,FFTsize,distSize);// convert FFT data to dB
  if (wsFFTenable){
    wsOk=ws.broadcastBIN((uint8_t*) AdB, FFTsize+distSize) &&
         ws.broadcastTXT(configStr); 
    if (!wsOk) {if (LogEnable) Serial.println("ws FFT Error!");}//if Ok 
  }//if wsFFTenable  

  if(LogFFTenable) printFFTdB8(AdB, FFTsize+distSize); 

}//loop
