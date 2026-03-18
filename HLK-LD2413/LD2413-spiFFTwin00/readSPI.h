#include <ESP32DMASPISlave.h>
ESP32DMASPI::Slave slave;
static constexpr size_t BUFFER_SIZE=8208; //2*(4+4100)=2*(4+4*1025)
uint hBufferSize=4104; 
uint8_t *dma_rx_buf;
uint8_t rx_buf[BUFFER_SIZE];

union buf4byte{ //reverse order Hi,Lo
  struct {int16_t Q, I;}; //normal order I,Q
  struct {uint8_t B[4];};
};//union

void setupSPI(){
  dma_rx_buf=slave.allocDMABuffer(BUFFER_SIZE);
  slave.setDataMode(SPI_MODE0);   // default: SPI_MODE0 =0..3 polarity & phase
  slave.setMaxTransferSize(BUFFER_SIZE);  // default: 4092 bytes
  slave.setQueueSize(1); // default: 1
  slave.begin(HSPI, 4, 5, 6, SS_PIN);  // spi_bus, sck, miso, mosi, ss
  if (LogEnable) Serial.println("start spi slave");
}//setupSPI

uint readSPI(){
  digitalWrite(LED,LOW);
  slave.queue(NULL,dma_rx_buf,BUFFER_SIZE);
  const std::vector<size_t> received_bytes=slave.wait();
  memcpy(rx_buf,dma_rx_buf,BUFFER_SIZE);
  digitalWrite(LED,HIGH);
  return (received_bytes[0]);
}//readSPI

void printSPI(int spiLen){
  int offset;
  char format;
  buf4byte t;
  if (spiLen==0) return;

  digitalWrite(LED,LOW);
  Serial.print("** "); Serial.print(" "); 
  Serial.println(spiLen);
  for (int j=0; j<2; j++) { offset=j*hBufferSize;
    for (int i=0; i<hBufferSize; i+=4){ 
      for (int k=0; k<4; k++) t.B[3-k]=rx_buf[offset+i+k]; //copy memory reverse order
      if ((i==0)||(i==4100)) {Serial.print("* "); format=HEX;} else format=DEC; 
      Serial.print(t.I,format); Serial.print(" ");
      Serial.print(t.Q,format); Serial.println();
    }//for i
  }//for j
  digitalWrite(LED,HIGH);
}//printSPI