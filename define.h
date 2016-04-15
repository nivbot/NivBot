#include <Wire.h>
#include <ADXL345.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); //RX TX
ADXL345 brainAcc;

#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 1
//the Send Data Register each bit Setting information
#define Data_Tap2  11
#define Data_Tap1  10
#define Data_Dir2   9
#define Data_Dir1   8
#define Data_Q      7
#define Data_SYN    240

int X_3I, Y_3I, Z_3I;
int X_Init, Y_Init, Z_Init;
unsigned int SendData = 0b1000000000000; //this guy hold all data from the headset and used for send out to control NIVBOT
byte Data_High, Data_Low;

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0;
int payloadLength = 0;
unsigned int payloadData[256] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

boolean hasPower = false;
unsigned long eegPower[8];

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte() {
  byte ByteRead;

  while (!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT
  mySerial.write(ByteRead);   // echo the same byte out the USB serial (for debug purposes)
  //delay(10);
#endif

  return ByteRead;
}


