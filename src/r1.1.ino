#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

// Pin Definitions
#define SERVO_PIN 13
#define BUTTON_PIN 22
#define IN3 42
#define IN4 43
#define ENB 5
#define INITIAL_ANGLE 99
#define trigPinl 2
#define echoPinl 3
#define trigPinr 4
#define echoPinr 9
#define trigPinf 6
#define echoPinf 7

MPU6050 mpu;
Servo myServo;
int16_t gz;
long previous_time;
float yaw_angle = 0;
float gyro_z_offset = 0;
int calibration_count = 1000; // Number of samples for calibration
int currentAngle = INITIAL_ANGLE;
unsigned long lastTime = 0;
int i = 0; // Declare variable 'i'
int targetYawAngle = 0;
float ANGLE_CORRECTION_FACTOR = 1;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Ultrasonic sensor setup
  pinMode(trigPinl, OUTPUT);
  pinMode(echoPinl, INPUT);
  pinMode(trigPinr, OUTPUT);
  pinMode(echoPinr, INPUT);
  pinMode(trigPinf, OUTPUT);
  pinMode(echoPinf, INPUT);

  // Check if MPU6050 is connected properly
  Serial.println(mpu.testConnection() ? "MPU6050 connected" : "MPU6050 connection failed");
  
  // Calibrate the gyro to calculate drift
  Serial.println("Calibrating gyro...");
  calculateGyroDrift();
  Serial.print("Gyro Z Offset: ");
  Serial.println(gyro_z_offset);

  // Servo setup
  myServo.attach(SERVO_PIN);
  Serial.print("Initial angle set to: ");
  Serial.println(currentAngle);
  myServo.write(INITIAL_ANGLE);

  // Motor setup
  digitalWrite(IN3, HIGH);  // Set direction
  digitalWrite(IN4, LOW);   // Set direction
  analogWrite(ENB, 0); // Set initial speed (0-255)

  previous_time = millis(); // Record the start time

  while (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println( usLeft() );
  }

  analogWrite(ENB, 255);  

  while (usForward() > 35) {
    MoveFWwithGyro();
  }
  
  stopMotor();
  delay(100);

  if (usLeft() > usRight()) {
    i = 1;
  } else if (usLeft() < usRight()) {
    i = 2;
  } else {
    while (1) {}
  }
  
  analogWrite(ENB, 255);

  while (i == 1) {
    MoveFWwithGyro();
    if (usForward() < 35 && usLeft() > 70) {
      targetYawAngle += 90;
      while (usForward() < 100) {
        MoveFWwithGyro();
      }
    }
  }
  
  while (i == 2) {
    MoveFWwithGyro();
    if (usForward() < 35 && usRight() > 70) {
      targetYawAngle -= 90;
      while (usForward() < 100) {
        MoveFWwithGyro();
      }
    }
  }
}