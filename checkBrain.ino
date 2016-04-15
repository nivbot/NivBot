void CheckBrain()
{
  // Look for sync bytes
  if (ReadOneByte() == 170) {
    if (ReadOneByte() == 170) {
      payloadLength = ReadOneByte();
      if (payloadLength > 169)                     //Payload length can not be greater than 169
        return;

      generatedChecksum = 0;
      for (int i = 0; i < payloadLength; i++) {
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }

      checksum = ReadOneByte();                      //Read checksum byte from stream
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

      if (checksum == generatedChecksum) {
        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for (int i = 0; i < payloadLength; i++) {   // Parse the payload
          switch (payloadData[i]) {
            case 2:
              i++;
              poorQuality = payloadData[i];
              bigPacket = true;
              break;
            case 4:
              i++;
              attention = payloadData[i];
              break;
            case 5:
              i++;
              meditation = payloadData[i];
              break;
            case 0x80:
              i = i + 3;
              break;
            case 0x83:
              // i = i + 25;
              i = i + 25;
              break;
            default:
              break;
          } // switch
        } // for loop to parse the Payload
        // *** Add your code here ***
        #if !DEBUGOUTPUT
        if (bigPacket) {

          myAction();

        }
        bigPacket = false;
        #endif
      }//end if Checksum Sucess
      else {// Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
}

void myAction()
{
  SendData = SendData | ( 1 << 12);
  // Read normalized values
  Vector raw = brainAcc.readRaw();
  Activites activ = brainAcc.readActivites();

  if (activ.isDoubleTap)      SendData = SendData | ( 1 << Data_Tap2 );              // Data Register Tap: 10 // Single Tap
  else if (activ.isTap)       SendData = SendData | ( 1 << Data_Tap1 );              // Data Register Tap: 01 // Double Tap
  else SendData = SendData & 0b1001111111111;                                        // Data Register Tap: 00 // No Tap

  if ( (raw.ZAxis - Z_Init) > 50)      SendData = SendData | ( 1 << Data_Dir2 );                       // Data Register Dir: 10 //Left
  else if ((raw.ZAxis - Z_Init) < -50)  SendData = SendData | ( 1 << Data_Dir2 ) | ( 1 << Data_Dir1 ); // Data Register Dir: 11 //Right
  else if ((raw.XAxis - X_Init) > 50 )  SendData = SendData | ( 1 << Data_Dir1 );                      // Data Register Dir: 01 //Forward
  else if ((raw.XAxis - X_Init) < -50)  SendData = SendData & 0b110011111111;                          // Data Register Dir: 00 //Stop
  else SendData = SendData & 0b1110011111111;

  if (poorQuality == 0)  { //good Quality and catch the brain signal
    SendData = SendData + (unsigned int)attention;
    Data_High = (SendData >> 8);
    Data_Low  = (SendData & 0b01111111);
    mySerial.write(Data_SYN);
   // mySerial.flush();
    delay(1);
    mySerial.write(Data_High);
    delay(1);
    mySerial.write(Data_Low);
    SendData = SendData & 0;
  }
  else if (poorQuality == 200) { //poor Quality , just show out acc sensor data
    SendData = SendData | 0b1000011110000;
    Data_High = (SendData >> 8);
    Data_Low  = (SendData & 0b11111111);
    mySerial.write(Data_SYN);
    delay(1);
    mySerial.write(Data_High);
    delay(1);
    mySerial.write(Data_Low);
    SendData = SendData & 0;
  }
  else {   // have been touch but not good Quality,show out acc sensor data and set the lower 7 bit high to let me know the statues
    SendData = SendData | 0b1000001111111;
    //    mySerial.print("don't just touch me!!  ");
    Data_High = (SendData >> 8);
    Data_Low  = (SendData & 0b11111111);
    mySerial.write(Data_SYN);
    delay(1);
    mySerial.write(Data_High);
    delay(1);
    mySerial.write(Data_Low);
    SendData = SendData & 0;
  }

  //  mySerial.print("PoorQuality: ");
  //  mySerial.print(poorQuality, DEC);
  //  mySerial.print(" Attention: ");
  //  mySerial.print(attention, DEC);
  //  mySerial.print(" Meditation: ");
  //  mySerial.print(meditation, DEC);
  //  mySerial.print(" Time since last packet: ");
  //  mySerial.print(millis() - lastReceivedPacket, DEC);
  //  lastReceivedPacket = millis();

  //  mySerial.print("\n");

}

