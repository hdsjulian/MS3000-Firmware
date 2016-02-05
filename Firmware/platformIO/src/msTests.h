

void testAccelerometer()
{
  Serial.println("trying accel!");

  int d = 500;
  int b = 20;
  while (1)
  {
    if (msSystem.msSensorOK) 
    {
      msSystem.msLEDs.fillPixels(0, b, 0, 0xff);
    }
    else
    {
     msSystem.msLEDs.fillPixels(b, 0, 0, 0xff);
   }
   msSystem.msLEDs.updatePixels();
   delay(d);

   msSystem.msLEDs.fillPixels(b, b, b, 0xff);
   msSystem.msLEDs.updatePixels();
   delay(d);
 }
}


void testButtonForBOM_X()
{
  if (msGlobals.ggCurrentFrame % 1000 == 0)
  {
    msSystem.logln("_");
  }

  if (msSystem.msButtons.msBtnPwrLongHit)
  {
    msSystem.msLEDs.setPixel(1, 0, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnPwrHit)
  {
    msSystem.msLEDs.setPixel(1, 20, 20, 0, 15);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnALongHit)
  {
    msSystem.msLEDs.setPixel(0, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnAHit)
  {
    msSystem.msLEDs.setPixel(0, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnBLongHit)
  {
    msSystem.msLEDs.setPixel(2, 20, 0, 20, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
  if (msSystem.msButtons.msBtnBHit)
  {
    msSystem.msLEDs.setPixel(2, 20, 20, 0, 20);
    msSystem.msLEDs.updatePixels();
    delay(200);
  }
}


void testSimpleButtons()
{

  double avgVal = 800;
  double avgF = 0.01;

  int cnt = 0;

  int minV = 1000;
  int maxV = 0;

  cnt++;

  int adVal = msSystem.getADValue();

  avgVal = avgF * adVal + (1-avgF) * avgVal; 

  if (minV > adVal) minV = adVal;
  if (maxV < adVal) maxV = adVal;

  if (adVal > 950 )
  {
    msSystem.msLEDs.setPixel(7, 0, 10, 0, msGlobals.ggBrightness);
    msSystem.msLEDs.setPixel(8, 0, 10, 0, msGlobals.ggBrightness);
  }
  else
  {
    msSystem.msLEDs.setPixel(7, 10, 0, 0, msGlobals.ggBrightness);
    msSystem.msLEDs.setPixel(8, 10, 0, 0, msGlobals.ggBrightness);
  }

  if (!digitalRead(PIN_BUTTON_A))
  {
    msSystem.msLEDs.setPixel(9, 0, 10, 0, msGlobals.ggBrightness);
  }
  else
  {
    msSystem.msLEDs.setPixel(9, 10, 0, 0, msGlobals.ggBrightness);
  }

  if (!digitalRead(PIN_BUTTON_B))
  {
    msSystem.msLEDs.setPixel(6, 0, 10, 0, msGlobals.ggBrightness);
  }
  else
  {
    msSystem.msLEDs.setPixel(6, 10, 0, 0, msGlobals.ggBrightness);
  }
  if (cnt % 300 == 550)
  {
    minV++;
    maxV--;
  }

  if (cnt%40 == 0)
  {
    Serial.print(minV);
    Serial.print(":");
    Serial.print(maxV);
    Serial.print("|");
    Serial.print(adVal);
    Serial.print("/");
    Serial.println(avgVal);
  }
  msSystem.msLEDs.updatePixels();
  delay(10);
  //return;
}

void logButtons()
{
  msSystem.log("msBtnA:"); msSystem.logln(String(msSystem.msButtons.msBtnAPressTime));
  msSystem.log("msBtnPwr:"); msSystem.logln(String(msSystem.msButtons.msBtnPwrPressTime));
  msSystem.log("msBtnB:"); msSystem.logln(String(msSystem.msButtons.msBtnBPressTime));
}

void doTests()
{

  // do some tests
  // testAccelerometer();
  // testButtonForBOM_X();
  // testSimpleButtons();
  // logButtons();
}
