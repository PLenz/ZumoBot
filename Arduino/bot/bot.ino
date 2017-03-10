#include<Wire.h>
#include<ZumoMotors.h>
#include <ZumoBuzzer.h>

const int MAX_SPEED = 400;
const byte LED = 13;
const int deadZone = 10;

int STATE = 0;

ZumoMotors motors;
ZumoBuzzer buzzer;

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
  for (int i = 0; i < count; i++) {
    Wire.read();
  }
}

void receiveEvent(int count) {
  if (count == 0)
    return;

  int mode = Wire.read();
  count--;

  if (mode == 0) {
    if (count == 1) {
      int state = Wire.read();
      count--;
      switch (state)
      {
        case 0:
          motors.setLeftSpeed(0);
          motors.setRightSpeed(0);
          break;
        case 1:
          buzzer.play("L16 cdegreg4");
          break;
        case 2:
          break;
      }

      STATE = state;
    }
  } else if (mode == 1 && STATE == 1) {
    if (count == 4) {

      int x = Wire.read();
      count--;
      int x_neg = Wire.read();
      count--;
      int y = Wire.read();
      count--;
      int y_neg = Wire.read();
      count--;

      drive(x, x_neg, y, y_neg);
    }
  }
  if (count > 0)
    clear(count);
}

void drive(int x, int x_neg, int y, int y_neg) {
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

  if (leftMotorScaled >= 240 && rightMotorScaled >= 240) {
    motors.setLeftSpeed(MAX_SPEED);
    motors.setRightSpeed(MAX_SPEED);
  } else {
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
