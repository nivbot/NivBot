void ADXL345_Init()
{
  brainAcc.setRange(ADXL345_RANGE_16G);
  // Set tap detection on Z-Axis
  brainAcc.setTapDetectionX(0);       // Don't check tap on X-Axis
  brainAcc.setTapDetectionY(0);       // Don't check tap on Y-Axis
  brainAcc.setTapDetectionZ(1);       // Check tap on Z-Axis

  brainAcc.setTapThreshold(2.5);      // Recommended 2.5 g
  brainAcc.setTapDuration(0.02);      // Recommended 0.02 s
  brainAcc.setDoubleTapLatency(0.10); // Recommended 0.10 s
  brainAcc.setDoubleTapWindow(0.30);  // Recommended 0.30 s
  // Select INT 1 for get activities
  brainAcc.useInterrupt(ADXL345_INT1);

  for (unsigned char i = 0; i < 3; i++ )
  {
    Vector raw = brainAcc.readRaw();
    X_3I += raw.XAxis;
    Y_3I += raw.YAxis;
    Z_3I += raw.ZAxis;
    delay(100);
  }
  X_Init = X_3I / 3;
  Y_Init = Y_3I / 3;
  Z_Init = Z_3I / 3;
}

void checkSetup()
{
  mySerial.print("Look tap on axis = "); 
  if (brainAcc.getTapDetectionX()) { mySerial.print(" X "); }
  if (brainAcc.getTapDetectionY()) { mySerial.print(" Y "); }
  if (brainAcc.getTapDetectionZ()) { mySerial.print(" Z "); }
  mySerial.println();

  mySerial.print("Tap Threshold = "); mySerial.println(brainAcc.getTapThreshold());
  mySerial.print("Tap Duration = "); mySerial.println(brainAcc.getTapDuration());
  mySerial.print("Double Tap Latency = "); mySerial.println(brainAcc.getDoubleTapLatency());
  mySerial.print("Double Tap Window = "); mySerial.println(brainAcc.getDoubleTapWindow());
}

