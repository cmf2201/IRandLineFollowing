#include <Arduino.h>
#include <QTRSensors.h>
#include <IRdecoder.h>
#include <RemoteConstants.h>
#include <Chassis.h>


QTRSensors qtr;
uint16_t position;

IRDecoder decoder(14);

Chassis chassis;

bool lineFollowOn = false;

int effort = 100;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];


void lineFollow() {
  position = qtr.readLineBlack(sensorValues);
  if(position < 2700) {
    chassis.setMotorEfforts(effort - 20,effort);
  } else if ( position > 3300) {
    chassis.setMotorEfforts(effort,effort - 20);
  } else {
    chassis.setMotorEfforts(effort,effort);
  }
}


void checkRemote() {
  int16_t code = decoder.getKeyCode();
  switch (code) {

    case remote1:
      Serial.println("on");
      lineFollowOn = true;
      break;

    case remote2:
      lineFollowOn = false;
      chassis.setMotorEfforts(0,0);
      break;

    case remoteLeft:
      Serial.print("Position: ");
      position = qtr.readLineBlack(sensorValues);

      Serial.println(position);
      //Do something fancy here, or set a specific state!
      //Other fancy things too!
      //Adding
      //New line
      break;

    case remoteRight:
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

    //   analogRead() takes about 0.1 ms on an AVR.
      // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
      // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
      // Call calibrate() 400 times to make calibration take about 10 seconds.
      qtr.resetCalibration();
      for (uint16_t i = 0; i < 400; i++)
      {
        qtr.calibrate();
      }
      digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

      break;
    case remoteDown:
    Serial.println("DOWN");
    //New line
    break;
    case remoteUp:
    Serial.println("UP");
    //more changes
    break;
    
  }
}



void setup()
{
  Serial.begin(9600);

  delay(1000);

  //configure the chassis 
  chassis.init();

  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A7, A6, A0, A2, A3, A4}, SensorCount);
//  qtr.setEmitterPin(2);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

//   analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

  decoder.init();
}

void loop()
{
  
  checkRemote();

  if(lineFollowOn)
  {
    lineFollow();
  }
    // delay(100);
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
}





