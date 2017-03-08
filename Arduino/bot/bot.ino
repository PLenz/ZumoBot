#include <Wire.h>
#include <ZumoMotors.h>

ZumoMotors motors;

int leftMotorScaled = 0;
int rightMotorScaled = 0;
int deadZone = 10;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
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

void receiveEvent(int count) {
  int mode = Wire.read();
  if (mode == 1) {
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
