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
    analogWrite(ENB, 0);      // Set initial speed (0-255)

    previous_time = millis(); // Record the start time

    while (digitalRead(BUTTON_PIN) == LOW) {
        // Serial.println();
    }

    return_front = usForward();
    return_right = usRight();
    return_left = usLeft();

    analogWrite(ENB, 255);

    while (usForward() > 45) {
        MoveFWwithGyro();
        stopfromcrashing_andreturn();
    }

    stopMotor();

    if (usLeft() > usRight()) {
        i = 1;
    } else if (usLeft() < usRight()) {
        i = 2;
    } else {
        while (1) {}
    }

    if (i == 1) {
        if (return_left > return_right) {
            section = 2;
        }
        if (return_left < return_right) {
            section = 1;
        }
    }

    if (i == 2) {
        if (return_left > return_right) {
            section = 1;
        }
        if (return_left < return_right) {
            section = 2;
        }
    }

    if (145 <= return_front && return_front <= 180) {
        place = 1;
    }
    if (95 <= return_front && return_front <= 130) {
        place = 2;
    }
    analogWrite(ENB, 255);
    if (i == 1) {
        MoveFWwithGyro();
        stopfromcrashing_andreturn();
        targetYawAngle += 90;
        while (first_time < 300) {
            first_time += 1;  
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
        }
    }
    if (i == 2) {
        MoveFWwithGyro();
        stopfromcrashing_andreturn();
        targetYawAngle -= 90;
        while (first_time < 300) {
            first_time += 1;   
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
        }
    }
}

void loop() {
    while (i == 1 && counter < 10) {
        MoveFWwithGyro();
        stopfromcrashing_andreturn();
        if (65 <= usForward() && usForward() <= 95 && usLeft() >= 70) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            turn = 1;
        }
        if (65 <= usForward() && usForward() <= 95 && usLeft() <= 70) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            turn = 2;
        }
        if (turn == 1 && usForward() <= 60) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            targetYawAngle += 90;
            counter += 1;
            while (turning_counter < 300) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                turning_counter += 1;
            }
            turning_counter = 0;
            turn =  0;
        }   
        if (turn == 2 && usForward() <= 35) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            targetYawAngle += 90;
            counter += 1;
            while (turning_counter < 300) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                turning_counter += 1;
            }
            turning_counter = 0;
            turn = 0;
        }
    }

    while (i == 2 && counter <= 10) {
        MoveFWwithGyro();
        stopfromcrashing_andreturn();
        if (65 <= usForward() && usForward() <= 95 && usRight() >= 70) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            turn = 1;
        }
        if (65 <= usForward() && usForward() <= 95 && usRight() <= 70) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            turn = 2;
        }
        if (turn == 1 && usForward() <= 60) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            targetYawAngle -= 90;
            counter += 1;
            while (turning_counter < 300) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                turning_counter += 1;
            }
            turning_counter = 0;
            turn = 0;
        }
        if (turn == 2 && usForward() <= 35) {
            MoveFWwithGyro();
            stopfromcrashing_andreturn();
            targetYawAngle -= 90;
            counter += 1;
            while (turning_counter < 300) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                turning_counter += 1;
            }
            turning_counter = 0;
            turn = 0;
        }
    }

    while (counter > 10) {
        if (i == 1) {
            if (section == 1 && usForward() <= 60) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                targetYawAngle += 90;
                while (turning_counter < 300) {
                    MoveFWwithGyro();
                    stopfromcrashing_andreturn();
                    turning_counter += 1;
                }
                turning_counter = 0; 
                if (place == 1) {
                    while (usForward() > 150) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                } else if (place == 2) {
                    while (usForward() > 110) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                }
            }
            if (section == 2 && usForward() <= 35) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                targetYawAngle += 90;
                while (turning_counter < 300) {
                    MoveFWwithGyro();
                    stopfromcrashing_andreturn();
                    turning_counter += 1;
                }
                turning_counter = 0; 
                if (place == 1) {
                    while (usForward() > 150) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                } else if (place == 2) {
                    while (usForward() > 110) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                }
            }
        }
        if (i == 2) {
            if (section == 1 && usForward() <= 60) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                targetYawAngle -= 90;
                while (turning_counter < 300) {
                    MoveFWwithGyro();
                    stopfromcrashing_andreturn();
                    turning_counter += 1;
                }
                turning_counter = 0; 
                if (place == 1) {
                    while (usForward() > 150) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                } else if (place == 2) {
                    while (usForward() > 110) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                }
            }
            if (section == 2 && usForward() <= 35) {
                MoveFWwithGyro();
                stopfromcrashing_andreturn();
                targetYawAngle -= 90;
                while (turning_counter < 300) {
                    MoveFWwithGyro();
                    stopfromcrashing_andreturn();
                    turning_counter += 1;
                }
                turning_counter = 0; 
                if (place == 1) {
                    while (usForward() > 150) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                } else if (place == 2) {
                    while (usForward() > 110) {
                        MoveFWwithGyro();
                        stopfromcrashing_andreturn();
                    }
                    stopMotor();
                }
            }
        }
    }
}

// Function Definitions

void calculateGyroDrift() {
    gyro_z_offset = 0;  // Reset the offset
    long temp_offset = 0;  // Temporary variable to store the sum
    int validReadings = 0;

    for (int i = 0; i < calibration_count; i++) {
        gz = mpu.getRotationZ();

        // Exclude any extreme outliers
        if (gz > -32760 && gz < 32760) {
            temp_offset += gz;
            validReadings++;
        }

        delay(3);  // Short delay between readings to stabilize
    }

    // Average the valid readings to get the drift (offset) value
    if (validReadings > 0) {
        gyro_z_offset = temp_offset / validReadings;
    }
}

void MoveFWwithGyro() {
    long current_time = millis();
    float elapsed_time = (current_time - previous_time) / 1000.0;  // Calculate elapsed time in seconds
    previous_time = current_time;

    // Get Z-axis rotation value
    gz = mpu.getRotationZ();

    // Apply drift correction
    gz -= gyro_z_offset;

    // Convert gyroscope reading to degrees per second
    float gyro_z = ((float)gz / 131.0) + 0.001517;

    // Calculate yaw angle
    yaw_angle += gyro_z * elapsed_time;

    // Calculate error
    float error = targetYawAngle - yaw_angle;
    Serial.println(yaw_angle);
    Serial.println(targetYawAngle);
    // PID calculations
    float proportional = error;
    integral += error * elapsed_time;
    float derivative = (error - previousError) / elapsed_time;
    float output = Kp * proportional + Ki * integral + Kd * derivative;

    previousError = error;

    // Update current angle
    currentAngle += output * ANGLE_CORRECTION_FACTOR;

    // Constrain the angle to servo limits
    currentAngle = constrain(currentAngle, 0, 180);

    myServo.write(currentAngle);
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

    // PID calculations
    float error = targetYawAngle - yaw_angle;
    integral += error * elapsed_time;
    float derivative = (error - previousError) / elapsed_time;
    float output = Kp * error + Ki * integral + Kd * derivative;
    previousError = error;

    // Adjust the servo angle based on PID output
    float t = output + INITIAL_ANGLE;
    t = constrain(t, 60, 140); // Keep the angle within bounds
    myServo.write(int(t));

    // Debugging information
    Serial.print("Yaw Angle: "); Serial.println(yaw_angle);
    Serial.print("Target Yaw Angle: "); Serial.println(targetYawAngle);
    Serial.print("Servo Angle: "); Serial.println(t);
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
    if (usForward() < 15) {
        analogWrite(ENB, 0);
        delay(100);
        digitalWrite(IN3, LOW);  
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, 255);
        while (fixing_time < 100) {
            fixing_time += 1;
            MoveFWwithGyro_Reverse();
        }
        analogWrite(ENB, 0);
        delay(100);
        digitalWrite(IN3, HIGH);  
        digitalWrite(IN4, LOW);
        analogWrite(ENB, 255);
        fixing_time = 0;
    }
}