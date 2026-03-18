// WebSocketsServer needs Hash
#include <WebSocketsServer.h>
WebSocketsServer ws=WebSocketsServer(81);

String serverIP;
IPAddress clientIP; //maybe Array?
bool onClient[WEBSOCKETS_SERVER_CLIENT_MAX];
bool wsConnected=false;
void decodeWS(String s);

void wsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  int clients;
  Serial.print("Event on "); Serial.print(num); Serial.print(":");
  switch(type) {
    case WStype_ERROR:
      if (LogEnable) Serial.println("Error!!!");
      break;          
    case WStype_DISCONNECTED: 
      onClient[num]=false;  
      if (LogEnable) Serial.println("Disconnected!");
      onClient[num]=false;  
      break;
    case WStype_CONNECTED: wsConnected=true; clientIP= ws.remoteIP(num);
      onClient[num]=true;  
      if (LogEnable) {
        Serial.print(" Connected to ");
        for (int i=0; i<4; i++){ Serial.print(clientIP[i],DEC); Serial.print("."); } 
        Serial.println();
      }//if Log   
      break;
    case WStype_TEXT:
      String s=String((char*)payload);
      decodeWS(s);
      if (LogEnable) {
        Serial.print(num); Serial.print(":"); Serial.print(s); Serial.println("."); 
      }//if Log   
      break;
  }//switch
}//