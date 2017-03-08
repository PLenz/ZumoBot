#include <Wire.h>
#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <ZumoBuzzer.h>

byte LED = 13;

ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoBuzzer buzzer;

int leftMotorScaled = 0;
int rightMotorScaled = 0;
int deadZone = 10;

int state = 1; //0=off, 1=drive, 2=lineFollower

void setup() {
  reflectanceSensors.init();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  buzzer.play("L16 cdegreg4");
}

void loop() {
  if (state == 1) {

  } else if (state == 2) {
    if (abs(leftMotorScaled) > deadZone)
      motors.setLeftSpeed(leftMotorScaled);
    else
      motors.setLeftSpeed(0);

    if (abs(rightMotorScaled) > deadZone)
      motors.setRightSpeed(rightMotorScaled);
    else
      motors.setRightSpeed(0);
    delay(10);
  }
}

void receiveEvent(int count) {
  int mode = Wire.read();
  if (mode == 0) {
    if (count == 2) {
      state = Wire.read();
    }
  } else if (mode == 1) {
    
    if (count == 5) {
      int throttle, direc = 0;

      int x = Wire.read();
      int x_neg = Wire.read();

      if (x_neg == 1)
        direc = x * -1;
      else
        direc = x;

      direc = direc * -1;

      int y = Wire.read();
      int y_neg = Wire.read();

      if (y_neg == 0)
        throttle = y * -1;
      else
        throttle = y;

      int leftMotor, rightMotor = 0;
      float leftMotorScale, rightMotorScale, maxMotorScale = 0;

      leftMotor = throttle + direc;
      rightMotor = throttle - direc;

      leftMotorScale =  leftMotor / 400.0;
      leftMotorScale = abs(leftMotorScale);
      rightMotorScale =  rightMotor / 400.0;
      rightMotorScale = abs(rightMotorScale);

      maxMotorScale = max(leftMotorScale, rightMotorScale);
      maxMotorScale = max(1, maxMotorScale);

      leftMotorScaled = constrain(leftMotor / maxMotorScale, -400, 400);
      rightMotorScaled = constrain(rightMotor / maxMotorScale, -400, 400);
    }
  }
}
