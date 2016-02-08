#ifndef _MAGNETOMETER_MODE_H
#define _MAGNETOMETER_MODE_H

//
// render the magnetometer sensor data
// 

class MagicMagnetMode : public MagicShifterBaseMode 
{

private:
  int frame = 0;
  int lMode = 0;

  public:
    
    const char *modeName="MagicMagnet";

    void start()
    {
    }

    void stop(void)
    {
    }

    bool step()
    {
      static  int autoCalResetCounter = 0;

      frame++;

      // AccelPoll();
      if (msSystem.msButtons.msBtnAHit) {
        lMode--;
        msSystem.msButtons.msBtnAHit = false;
      }
      if (msSystem.msButtons.msBtnBHit) {
        lMode++;
        msSystem.msButtons.msBtnBHit = false;
      }
      if (lMode < 0) lMode = 2;
      if (lMode > 2) lMode = 0;

      msSystem.msSensor.readMagnetometerData(msGlobals.ggMagnet);

      if ((msSystem.msSensor.readRegister(0x5E) & 0x02)) {
        msSystem.logln("magnetometer:go!");
        autoCalResetCounter++;
        if (autoCalResetCounter > 10) {
          msSystem.logln("magnetometer:reset");
          // M_CTRL_REG2: Hybrid auto increment, Magnetic measurement min/max detection function reset
          msSystem.msSensor.writeRegister(0x5C, 0x24);
          autoCalResetCounter = 0;
        }
      }
      else {
        autoCalResetCounter = 0;
      }

      //    msSystem.msLEDs.fillLEDs(frame % 300 == 0 ? v : 0, frame % 300 == 100 ? v : 0, frame % 300 == 200 ? v : 0);
      //    msSystem.msLEDs.setLED(15, centerBtnPressed ? 100 : 0, powerBtnPressed ? 100 : 0, 0);
      //    updateLedsWithBlank();

      // if (!MagicShifter_Poll()) break;
      
      int degrees = int(atan2(msGlobals.ggMagnet[YAXIS], -msGlobals.ggMagnet[XAXIS]) * 180 / M_PI);
      msSystem.msLEDs.fillLEDs(0,0,0);
      int lednr = map(abs(degrees), 0, 180, 0, 15);

      if (lMode <= 1) {
        for (int lC=0;lC<lednr;lC++) 
          msSystem.msLEDs.setLED(lC, 0, 255, 0, msGlobals.ggBrightness); // !J! hack

        for (int lC=lednr+1;lC<MAX_LEDS;lC++) 
          msSystem.msLEDs.setLED(lC, 255, 0, 0, msGlobals.ggBrightness); // !J! hack
      }

      if ((lMode == 0) || (lMode == 2))
        msSystem.msLEDs.setLED(lednr, 0, 255, 0, msGlobals.ggBrightness);

      if (lMode == 1)
        msSystem.msLEDs.setLED(lednr, 0, 0, 255, msGlobals.ggBrightness);

      msSystem.msLEDs.updateLEDs();

      delay(35);

    }

  void update() {};
  void reset() {};

};

#endif