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
#define trigPinr 11
#define echoPinr 10
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

// PID variables
float Kp = 1.0; // Proportional gain
float Ki = 0.0; // Integral gain
float Kd = 0.0; // Derivative gain
float previousError = 0;
float integral = 0;
float input = 0;
int return_front = 0;
int return_right = 0;
int return_left = 0;
int section = 0;
int counter = 0;
int place = 0;
int fixing_time = 0;
int turn = 0;
int first_time = 0;
int turning_counter = 0;

// Function declarations
void calculateGyroDrift();
void MoveFWwithGyro();
void MoveFWwithGyro_Reverse();
int usForward();
int usRight();
int usLeft();
void stopMotor();
void stopfromcrashing_andreturn();

void setup() {
    // Serial.begin(250000);
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
    // Serial.println(mpu.testConnection() ? "MPU6050 connected" : "MPU6050 connection failed");

    // Calibrate the gyro to calculate drift
    // Serial.println("Calibrating gyro...");
    calculateGyroDrift();
    // Serial.print("Gyro Z Offset: ");
    // Serial.println(gyro_z_offset);

    // Servo setup
    myServo.attach(SERVO_PIN);
    // Serial.print("Initial angle set to: ");
    // Serial.println(currentAngle);
    myServo.write(INITIAL_ANGLE);

    // Motor setup
    digitalWrite(IN3, HIGH);  // Set direction
    digitalWrite(IN4, LOW);   // Set direction
    analogWrite(ENB, 0);      // Set initial speed (0-255)

    previous_time = millis(); // Record the start time

    while (digitalRead(BUTTON_PIN) == LOW) {
        // Serial.println(usLeft());
    }

    return_front = usForward();
    return_right = usRight();
    return_left = usLeft();

    analogWrite(ENB, 210);

    while (usForward() > 60) {
        MoveFWwithGyro();
        // stopfromcrashing_andreturn();
    }

    if (usLeft() > usRight() ) {
        i = 1;
    } else if (usLeft() < usRight() ) {
        i = 2;
    } else {
        while (1) {}
    }
    

    while ( counter <= 11){
      Green_Turn();
      Red_Turn();
      // Serial.print(counter , ENB);
      // Serial.print("\n");
      while (i == 1 && counter <= 11 ) {
        Green_Turn();
        Red_Turn();
        // Serial.print(counter);
        // Serial.print("\n");
        MoveFWwithGyro();
        if (usForward() < 50 && usLeft() > 70) {
          targetYawAngle += 90;
          counter += 1;
          while (turning_counter < 1000) {
            MoveFWwithGyro();
            // Serial.print(counter);
            // Serial.print("\n");
            turning_counter += 1;
          }
          turning_counter = 0;

        }
      }
  
      while (i == 2 && counter <= 11) {
        Green_Turn();
        Red_Turn();
        // Serial.print(counter);
        // Serial.print("\n");
        MoveFWwithGyro();
        // Serial.print(counter);
        // Serial.print("\n");
        if (usForward() < 50 && usRight() > 70) {
          // Serial.print(counter);
          // Serial.print("\n");
          targetYawAngle -= 90;
          counter += 1;
          while (turning_counter < 1000) {
            // Serial.print(counter);
            // Serial.print("\n");
            MoveFWwithGyro();
            turning_counter += 1;
          }
          turning_counter = 0;
        }
      }
    }
    analogWrite(ENB, 0);
    while (usForward() > 140) {
        // Serial.print(counter);
        // Serial.print("\n");
        MoveFWwithGyro();
        
        // stopfromcrashing_andreturn();
    }
    while (usForward() < 140){

  
    stopMotor();
    // Serial.print("nigga");
    // Serial.print("\n");
    }
}
    

void loop() {
   
}

// Function Definitions

void MoveFWwithGyro() {
  long current_time = millis();
  float elapsed_time = (current_time - previous_time) / 1000.0; // Calculate elapsed time in seconds
  previous_time = current_time;

  // Get Z-axis rotation value
  gz = mpu.getRotationZ();

  // Apply drift correction
  gz -= gyro_z_offset;

  // Convert gyroscope reading to degrees per second
  float gyro_z = (gz / 131.0) + 0.004354;

  // Calculate the yaw angle by integrating the angular velocity over time
  yaw_angle += gyro_z * elapsed_time;

  // Apply the angle correction factor and constrain the servo angle
  float angleError = (yaw_angle - targetYawAngle) * ANGLE_CORRECTION_FACTOR;
  float t = angleError + INITIAL_ANGLE;
  t = constrain(t, 70, 130); // Keep the angle within bounds
  myServo.write(int(t));

  // Debugging information
  // Serial.print("Yaw Angle: "); Serial.println(yaw_angle);
  // Serial.print("Target Yaw Angle: "); Serial.println(targetYawAngle);
  // Serial.print("Servo Angle: "); Serial.println(t);
}

void calculateGyroDrift() {
  gyro_z_offset = 0; // Reset the offset
  long temp_offset = 0; // Temporary variable to store the sum
  int validReadings = 0;

  for (int i = 0; i < calibration_count; i++) {
    gz = mpu.getRotationZ();

    // Exclude any extreme outliers
    if (gz > -32760 && gz < 32760) {
      temp_offset += gz;
      validReadings++;
    }

    delay(3); // Short delay between readings to stabilize
  }

  // Average the valid readings to get the drift (offset) value
  if (validReadings > 0) {
    gyro_z_offset = temp_offset / validReadings;
  }
}

void MoveFWwithGyro_Reverse() {
  long current_time = millis();
  float elapsed_time = (current_time - previous_time) / 1000.0; // Calculate elapsed time in seconds
  previous_time = current_time;

  // Get Z-axis rotation value
  gz = mpu.getRotationZ();

  // Apply drift correction
  gz -= gyro_z_offset;

  // Convert gyroscope reading to degrees per second
  float gyro_z = (gz / 131.0) + 0.001517;

  // Calculate the yaw angle by integrating the angular velocity over time
  yaw_angle += gyro_z * elapsed_time;

  // Apply the angle correction factor, invert the direction, and constrain the servo angle
  float angleError = (targetYawAngle - yaw_angle) * ANGLE_CORRECTION_FACTOR; // Invert the angle calculation
  float t = angleError + INITIAL_ANGLE;
  t = constrain(t, 60, 140); // Keep the angle within bounds
  myServo.write(int(t));

  // Debugging information
  // Serial.print("Yaw Angle: "); Serial.println(yaw_angle);
  // Serial.print("Target Yaw Angle: "); Serial.println(targetYawAngle);
  // Serial.print("Servo Angle: "); Serial.println(t);
}


int usLeft() {
    digitalWrite(trigPinl, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinl, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinl, LOW);
    long duration = pulseIn(echoPinl, HIGH, 30000); // Timeout after 30ms
    if (duration == 0) return 200; // If no echo, return a large distance
    return duration * 0.034 / 2;
}

int usRight() {
    digitalWrite(trigPinr, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinr, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinr, LOW);
    long duration = pulseIn(echoPinr, HIGH, 30000); // Timeout after 30ms
    if (duration == 0) return 200; // If no echo, return a large distance
    return duration * 0.034 / 2;
}

int usForward() {
    digitalWrite(trigPinf, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinf, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinf, LOW);
    long duration = pulseIn(echoPinf, HIGH, 30000); // Timeout after 30ms
    if (duration == 0) return 200; // If no echo, return a large distance
    return duration * 0.034 / 2;
}

void stopMotor() {
    analogWrite(ENB, 0);
}

void stopfromcrashing_andreturn() {
    if (usForward() < 10) {
        analogWrite(ENB, 0);
        delay(100);
        digitalWrite(IN3, LOW);  
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, 210);
        while (fixing_time < 300) {
            fixing_time += 1;
            MoveFWwithGyro_Reverse();
        }
        analogWrite(ENB, 0);
        delay(100);
        digitalWrite(IN3, HIGH);  
        digitalWrite(IN4, LOW);
        analogWrite(ENB, 210);
        fixing_time = 0;
    }
}
void Green_Turn() {
  while (GREEN()) {
    if (green_x() >= 640) {
      MoveFWwithGyro();
    }
    if (green_x() <= 640) {
      if (green_size() > 10000) { 
        targetYawAngle -= 30;
        // setpoint = targetYawAngle; // Update PID setpoint
        while (turning_counter < 80) {
          turning_counter += 1;
          MoveFWwithGyro();
        }
        targetYawAngle += 30;
        // setpoint = targetYawAngle; // Reset PID setpoint
        turning_counter = 0;
      }
    }
  }
}
void Red_Turn() {
  while (RED) {
    if (red_x() <= 500) {
      MoveFWwithGyro();
    }
    if (red_x() >= 500) {
      if (green_size() > 10000) { 
        targetYawAngle += 30;
        // setpoint = targetYawAngle; // Update PID setpoint
        while (turning_counter < 80) {
          turning_counter += 1;
          MoveFWwithGyro();
        }
        targetYawAngle -= 30;
        // setpoint = targetYawAngle; // Reset PID setpoint
        turning_counter = 0;
      }
    }
  }
}
bool RED() {
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    
    if (receivedData.length() == 21) {
      int objectType = receivedData.charAt(0) - '0';
      if (objectType == 1) {
        int xPos = receivedData.substring(1, 6).toInt();
        int yPos = receivedData.substring(6, 11).toInt();
        int width = receivedData.substring(11, 16).toInt();
        int height = receivedData.substring(16, 21).toInt();
        
        // Serial.print("Received Data: ");
        // Serial.print("Type: ");
        // Serial.print(objectType);
        // Serial.print(", X: ");
        // Serial.print(xPos);
        // Serial.print(", Y: ");
        // Serial.print(yPos);
        // Serial.print(", Width: ");
        // Serial.print(width);
        // Serial.print(", Height: ");
        // Serial.println(height);

        return true;
      }
    }
  }
  return false;
}

// Function to handle GREEN object detection (type 2)
bool GREEN() {
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    
    if (receivedData.length() == 21) {
      int objectType = receivedData.charAt(0) - '0';
      if (objectType == 2) {
        int xPos = receivedData.substring(1, 6).toInt();
        int yPos = receivedData.substring(6, 11).toInt();
        int width = receivedData.substring(11, 16).toInt();
        int height = receivedData.substring(16, 21).toInt();
        
        // Serial.print("Received Data: ");
        // Serial.print("Type: ");
        // Serial.print(objectType);
        // Serial.print(", X: ");
        // Serial.print(xPos);
        // Serial.print(", Y: ");
        // Serial.print(yPos);
        // Serial.print(", Width: ");
        // Serial.print(width);
        // Serial.print(", Height: ");
        // Serial.println(height);

        return true;
      }
    }
  }
  return false;
}

// Function to get the X value for a RED object (type 1)
int red_x() {
  if (RED()) {
    String receivedData = Serial.readStringUntil('\n');
    return receivedData.substring(1, 6).toInt();
  }
  return -1; // Return -1 if no RED object is detected
}

// Function to get the X value for a GREEN object (type 2)
int green_x() {
  if (GREEN()) {
    String receivedData = Serial.readStringUntil('\n');
    return receivedData.substring(1, 6).toInt();
  }
  return -1; // Return -1 if no GREEN object is detected
}

// Function to calculate the size (width * height) for a RED object (type 1)
int red_size() {
  if (RED()) {
    String receivedData = Serial.readStringUntil('\n');
    int width = receivedData.substring(11, 16).toInt();
    int height = receivedData.substring(16, 21).toInt();
    return width * height;
  }
  return -1; // Return -1 if no RED object is detected
}

// Function to calculate the size (width * height) for a GREEN object (type 2)
int green_size() {
  if (GREEN()) {
    String receivedData = Serial.readStringUntil('\n');
    int width = receivedData.substring(11, 16).toInt();
    int height = receivedData.substring(16, 21).toInt();
    return width * height;
  }
  return -1; // Return -1 if no GREEN object is detected
}
