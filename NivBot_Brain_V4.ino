#include "define.h"

////////////
//  Setup //
////////////
void setup() {
  pinMode(LED, OUTPUT);
  mySerial.begin(115200);           // BLuetooth
  Serial.begin(BAUDRATE);          //TGAM
  brainAcc.begin();                 //ADXL345
  ADXL345_Init();
}

/////////////
//MAIN LOOP//
/////////////
void loop() {
 
  CheckBrain();
  //ReadOneByte();
}


