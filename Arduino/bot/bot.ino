#include<Wire.h>
#include<ZumoMotors.h>

const int MAX_SPEED = 400;
const byte LED = 13;
const int deadZone = 10;

ZumoMotors motors;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
}

void clear(int count) {
  for(int i = 0; i < count; i++) {
      Wire.read();
    }
}

void receiveEvent(int count) {
  int mode = Wire.read();
  count--;
  if (mode == 1) {
    if (count == 4) {

      int x = Wire.read();
      int x_neg = Wire.read();
      int y = Wire.read();
      int y_neg = Wire.read();
      count = count - 4;

      int throttle, direc = 0;

      if (x_neg == 1)
        direc = x * -1;
      else
        direc = x;
      direc = direc * -1;

      if (y_neg == 1)
        throttle = y * -1;
      else
        throttle = y;

      int leftMotor, rightMotor = 0;
      float leftMotorScale, rightMotorScale, maxMotorScale = 0;

      leftMotor = throttle + direc;
      rightMotor = throttle - direc;

      leftMotorScale = leftMotor / 400.0;
      leftMotorScale = abs(leftMotorScale);
      rightMotorScale = rightMotor / 400.0;
      rightMotorScale = abs(rightMotorScale);

      maxMotorScale = max(leftMotorScale, rightMotorScale);
      maxMotorScale = max(1, maxMotorScale);

      int leftMotorScaled = 0;
      int rightMotorScaled = 0;

      leftMotorScaled = constrain(leftMotor / maxMotorScale, -400, 400);
      rightMotorScaled = constrain(rightMotor / maxMotorScale, -400, 400);

      if (abs(leftMotorScaled) >= 245 && abs(rightMotorScaled) >= 245) {
        motors.setLeftSpeed(MAX_SPEED);
        motors.setRightSpeed(MAX_SPEED);
        Serial.println("max");
      } else {
        //Serial.println("left: " + String(leftMotorScaled) + " right: " + String(rightMotorScaled));
        if (abs(leftMotorScaled) > deadZone)
          motors.setLeftSpeed(leftMotorScaled);
        else
          motors.setLeftSpeed(0);

        if (abs(rightMotorScaled) > deadZone)
          motors.setRightSpeed(rightMotorScaled);
        else
          motors.setRightSpeed(0);
      }
      delay(10);
    }
  }
  clear(count);
}


