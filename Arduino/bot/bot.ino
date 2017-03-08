#include <Wire.h>
#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <ZumoBuzzer.h>

const int MAX_SPEED = 400;
const byte LED = 13;

ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoBuzzer buzzer;

int leftMotorScaled = 0;
int rightMotorScaled = 0;
int deadZone = 10;
int lastError = 0;

byte state = 0; //0=off, 1=drive, 2=lineFollower

void setup() {
  reflectanceSensors.init();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  if (state == 1) {
    if (abs(leftMotorScaled) > deadZone)
      motors.setLeftSpeed(leftMotorScaled);
    else
      motors.setLeftSpeed(0);

    if (abs(rightMotorScaled) > deadZone)
      motors.setRightSpeed(rightMotorScaled);
    else
      motors.setRightSpeed(0);
    delay(10);
  } else if (state == 2) {
    unsigned int sensors[6];
    
    int position = reflectanceSensors.readLine(sensors);
    
    int error = position - 2500;
    
    int speedDifference = error / 4 + 6 * (error - lastError);

    lastError = error;
    
    int m1Speed = MAX_SPEED + speedDifference;
    int m2Speed = MAX_SPEED - speedDifference;
    
    if (m1Speed < 0)
      m1Speed = 0;
    if (m2Speed < 0)
      m2Speed = 0;
    if (m1Speed > MAX_SPEED)
      m1Speed = MAX_SPEED;
    if (m2Speed > MAX_SPEED)
      m2Speed = MAX_SPEED;

    motors.setSpeeds(m1Speed, m2Speed);
  }
}

void calibrationSensor() {
  delay(1000);
  int i;
  for (i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);
}

void receiveEvent(int count) {
  int mode = Wire.read();
  if (mode == 0) {
    if (count == 2) {
      buzzer.play("L16 cdegreg4");
      state = Wire.read();
      Serial.println(String(state));
      if (state == 2)
        calibrationSensor();
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
