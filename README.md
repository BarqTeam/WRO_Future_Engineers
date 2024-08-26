Engineering materials
====

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Future Engineers competition in the season 2022.

## Content

* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members)
* `v-photos` contains 6 photos of the vehicle (from every side, from top and bottom)
* `video` contains the video.md file with the link to a video where driving demonstration exists
* `schemes` contains one or several schematic diagrams in form of JPEG, PNG or PDF of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition
* `models` is for the files for models used by 3D printers, laser cutting machines and CNC machines to produce the vehicle elements. If there is nothing to add to this location, the directory can be removed.
* `other` is for other files which can be used to understand how to prepare the vehicle for the competition. It may include documentation how to connect to a SBC/SBM and upload files there, datasets, hardware specifications, communication protocols descriptions etc. If there is nothing to add to this location, the directory can be removed.

## Introduction

Hello everyone,

We are the [Barq Engineers] from Palestine. We are a group of three ambitious young men: Amjad Ghannam, Rayan Farid, and Qusay Laila , with our car {BURAQ} represents not only our drive for technological advancement but also a symbol of hope and aspiration for the people of Palestine. Burag reflects the speed and brilliance of this legendary creature, embodying our ambition to overcome obstacles and make a mark on the world stage. Through Burag, we aim to demonstrate the remarkable achievements that Palestinian youth can accomplish, even in the face of adversity. .

In this competition, our goal is to showcase to the world what Palestinian youth can achieve, even under the harsh conditions of occupation. Through our efforts, we aim to send a powerful message about resilience, creativity, and the limitless potential that exists within every Palestinian boy.

## Project Overview

We began working on the WRO Future Engineers 2024 competition nearly three months ago. You can find more details about the competition in the official rules.

In summary, the challenge requires us to design and build a self-driving car capable of navigating a specific path set by the judges. The car must be equipped with the appropriate components to successfully complete the course.

In this repository, we've documented our entire journey over the past three months, capturing every step, challenge, and achievement along the way.

## 1.1 Team Management

We are deeply grateful to everyone who contributed to our work. The success of this project is a testament to the dedication and collaboration of our incredible team, which consists of:

* Amjad Ghannam:
Amjad is responsible for managing our social media accounts, including GitHub. He is the creative force behind our design concepts, overseeing the development of the car's body, its mechanical systems, and other related components. Additionally, Amjad assists with coding.
`Contact Information:`
                    _ Email: amjadgh72@gmail.com
                    _ WhatsApp: +972594387565

* Qusay Laila:
Qusay is our team leader and the mastermind behind problem-solving. He excels in CAD design and manages the main code, assembling all the functions like pieces of a Lego set.
`Contact Information:`
                   _ Email: qusaylaila.work@gmail.com
                    _WhatsApp: +972592509054

* Rayan Farid:
Rayan is our designer for 3D parts and CNC components. He also manages serial communication and contributes to the code alongside Qusay. Additionally, Rayan serves as a GitHub editor.
`Contact Information:`
                    _ Email: rayanplaystation180@gmail.com
                     _WhatsApp: +972568997345

*The Coach: Eng. Mohammad Muamar
Eng. Mohammad Muamar, a Palestinian engineer and graduate of Palestine Polytechnic University in Hebron, has been our steadfast guide throughout this journey. His unwavering support, especially during challenging times, has been invaluable.
`Contact Information:`
Email: moh.mummar@gmail.com
Phone: +970594958003
Palestine Polytechnic University

We extend our heartfelt gratitude to our coach and all the team members who have played a crucial role in bringing this project to life.

## 1.2 Vehicle Strengths
*  Chassis and Mechanism : Our vehicle was meticulously built from scratch, utilizing CNC-machined and 3D-printed parts. We also incorporated components from the EV3 Lego kit for critical mechanisms, such as the steering system and differential.

* Speed: Powered by a 24-volt lithium battery, our car achieves impressive speeds, giving it a competitive edge in performance.

* Mobility and Flexibility: The differential system significantly enhances the vehicle's ability to maneuver, providing smooth and flexible turns.

* Acceleration: The car boasts excellent acceleration, thanks to a well-calibrated gear ratio of 1.4:1 and a high-torque gear system that delivers powerful performance.

 ## 2. Programming Languages
We utilized C++ for coding the Arduino, ensuring efficient and reliable control of the hardware. Additionally, we employed Python to code the Raspberry Pi, particularly for implementing OpenCV with the Pi Camera.

## 3. The First Chassis
Initially, we decided to modify an RC car by adding the necessary components and creating a second layer to put them on. However, we encountered several challenges. The first issue arose when trying to create proper holes for mounting the servo motor. This led to further complications when attempting to implement a rack and pinion steering system (you can view the FreeCAD models for the rack and pinion in the "Models" section of this repository) using an MG699R servo motor.

Due to the high power and torque of the servo, it couldn't stay securely in place and kept shifting, which compromised the stability of the steering system. As a result, we decided to move on to our next plan: building a new chassis from scratch.

## 4. Programming Challenges
##Detecting Turns and Direction
In the WRO Future Engineers competition, the car's direction isn't fixed—it can move either clockwise or counterclockwise, adding complexity to navigation. This unpredictability is particularly challenging when the car needs to execute precise U-turns at specific points on the track.

### First Solution: Using the TCS3200 Color Sensor
* Color Detection: We initially programmed the TCS3200 sensor to identify specific colors on the track. An orange line signaled that the car should turn clockwise, while a blue line indicated a counterclockwise turn.

* Controlled Turning: Upon detecting a color, the car was instructed to turn at a specific angle. This angle was precisely controlled using an Inertial Measurement Unit (IMU), ensuring the car made accurate and consistent turns, regardless of external factors like speed or track conditions.

* Code Implementation: The process was automated through predefined functions such as detectOrangeLine and detectBlueLine, which triggered the appropriate turning actions based on the color detected.

This approach provided a straightforward method for determining the car's direction based on visual cues from the track, with the IMU ensuring precise turning control. However, this solution proved suboptimal due to the car's high speed and the limitations of the TCS3200, which was not the best choice for color detection in this context.

### Second Solution: Using Three Ultrasonic Sensors
* Direction Determination: We implemented three ultrasonic sensors to guide the car's movement based on real-time distance measurements. These sensors were strategically positioned to monitor distances on the left, right, and front of the car. When a turn was required, the direction was determined by comparing the distances detected by the sensors:

* If the right sensor measured a greater distance than the left sensor, the car would turn clockwise.
* Conversely, if the left sensor detected a greater distance, the car would turn counterclockwise.
#### Front Sensor Activation:
The front ultrasonic sensor played a critical role in detecting when the car was approximately 50 cm away from an obstacle or a turning point. This detection served as a trigger for the car to initiate the turn.

This approach emphasized spatial awareness and precise distance measurement, allowing the car to make informed directional decisions based on its environment rather than relying on visual cues. Ultimately, we decided that this solution was the most effective for our needs.

## Lap Counting Mechanism:
#### Loop-Based Counting with Ultrasonic Sensor and IMU

##### Overview:
This method utilizes a software-based loop counter to track lap completion. The robot employs an ultrasonic sensor to detect when a lap is completed. If the sensor measures a distance of less than 50 cm to a specified point or object, the system registers the start of a new lap. The robot's steering is precisely managed by an Inertial Measurement Unit (IMU), ensuring accurate navigation during turns.

## IMU-Based Steering
#### Overview: 
In our autonomous vehicle, precise steering control is essential for accurate navigation. We achieve this through an IMU-based steering mechanism. The Inertial Measurement Unit (IMU) supplies real-time data on the vehicle’s angular velocity, which is crucial for calculating the yaw angle (the vehicle's turning angle) and ensuring stable and accurate steering.

#### Understanding the IMU:
An IMU (Inertial Measurement Unit) integrates several sensors, typically including a 3-axis accelerometer and a 3-axis gyroscope. These components work together to monitor the vehicle's movement and orientation in three-dimensional space.

* Accelerometer: Measures acceleration along the X, Y, and Z axes, which helps determine the vehicle’s tilt and linear movement.
* Gyroscope: Measures angular velocity (the rate of rotation) around the X, Y, and Z axes, providing critical information for understanding rotational motion, especially the yaw angle.
#### The X, Y, and Z Axes: 
Understanding these axes is fundamental for interpreting IMU data:

* X-Axis: Represents roll, the tilting motion of the vehicle from side to side.
* Y-Axis: Represents pitch, the tilting motion of the vehicle from front to back.
* Z-Axis: Represents yaw, the rotational movement around the vertical axis, crucial for steering as it defines the vehicle’s direction of travel.
#### Calculating the Yaw Angle: 
The yaw angle (
�
�
θ 
z
​
 ) quantifies the vehicle's rotation around the Z-axis. The IMU’s gyroscope provides angular velocity data along this axis, indicating the rate of rotation.

To determine the vehicle's heading, we calculate the yaw angle by integrating the angular velocity over time. This calculation updates continuously to maintain precise tracking of the vehicle’s direction.

The yaw angle is computed using the following equation:

\theta_z = \theta_{z, \text{previous}} + \left( \frac{\text{gyro}_z - \text{gyro_z_offset}}{131.0} \right) \times \Delta t

Where:

�
�
θ 
z
​
  is the current yaw angle.
�
�
,
previous
θ 
z,previous
​
  is the yaw angle from the previous time step.
gyro
�
gyro 
z
​
  is the angular velocity around the Z-axis, provided by the gyroscope.
\text{gyro_z_offset} is the gyroscope offset, determined during calibration to correct for any drift.
Δ
�
Δt is the time elapsed between the current and previous readings.
### Importance of Gyro Offset:
The gyroscope offset (\text{gyro_z_offset}) is vital as gyroscopes can experience slight errors or biases over time, known as drift. By calculating and subtracting this offset, we ensure that the yaw angle calculation remains accurate, preventing gradual deviations from the true value.

## 6. Obstacle Avoidance Round Challenge
In this round we included the pi camera for  this round

## 7.Power 
Lithium Polymer (LiPo) batteries are highly favored in robotics due to their superior energy density, lightweight construction, and capability to deliver high currents. These attributes make them exceptionally suited for applications that demand both substantial power and agility, such as our autonomous robot.
Advantages of LiPo Batteries:

1- High Energy Density:

2- High Discharge Rate:

3- Lightweight and Compact:

4- Stable Voltage Output:

### Why a 24V Battery?
#### Voltage Compatibility:

The 24V output is ideal for many high-power robotics components, such as motors and motor drivers, which are designed to operate efficiently at this higher voltage. This reduces the need for additional voltage conversion and simplifies the power distribution system, making it more efficient for demanding applications.

#### Power Sufficiency:

A 24V battery provides ample power for the entire robot, ensuring that high-demand components like motors receive sufficient voltage for optimal performance. This higher voltage allows for efficient voltage regulation to lower voltages (e.g., 5V or 3.3V) for other electronics, while maintaining overall power efficiency.

Key Characteristics of the 24V LiPo Battery:

* Voltage: 24V nominal, ideal for direct use with high-power motors and for stepping down to lower voltages.
* Capacity: Typically ranges from 2000mAh to 6000mAh, influencing how long the robot can operate before requiring a recharge.
* Discharge Rate: High C-ratings (e.g., 20C) ensure the battery can deliver the necessary current for motors and other components during peak usage.
* Weight: Lightweight design, generally around 300g for a 2200mAh battery, important for maintaining the robot’s agility and maneuverability.

  ## 8.components
| Quantity | Status                             | Description                                                                                                                                             |
| ---------| ---------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1        | Raspberry Pi 4 Model B             |https://www.raspberrypi.com/products/raspberry-pi-4-model-b/                   |
| 1        | *Arduino Mega:          | (https://store.arduino.cc/products/arduino-mega-2560-rev3)            |
| 1        | The L298N Motor Driver:                     | https://components101.com/modules/l293n-motor-driver-module                         |
| 1       | Servo motor MG996r          |https://www.jsumo.com/mg996r-servo-motor-digital |
| 1 | pi camera module 2| https://www.raspberrypi.com/products/camera-module-v2/ |
| 1      | MPU-9250       |https://learn.sparkfun.com/tutorials/mpu-9250-hookup-guide/all         |
| 3        | *HC-SR04 Ultrasonic Sensor  | https://www.sparkfun.com/products/15569 |
|1      | TCS3200 Sensor        | https://www.adafruit.com/product/1334   |
|1      | lithum Battery 24v     | "We have connected two 12-volt batteries in series using a 6S Battery Management System (BMS) to ensure balanced charging and efficient power management."   |
|1      | HW-083       | https://www.elektormagazine.com/labs/hw-083-hack-evolution-charger   |









## Barq team's social accounts

## Overview
Our car is built on an RC structure featuring four wheels and a rear-wheel drive system. It is powered by a DC motor that controls the wheels and a servo motor for steering. The car is equipped with three ultrasonic sensors , gyroscope and a RGB color sensor to ensure it stays on the right path.

On the second level of the structure, we have placed the Arduino controller and the Raspberry Pi, along with a 12-volt battery down in the first floor, a camera, and other components that will be detailed in the parts section.



## How to prepare the repo based on the template

_Remove this section before the first commit to the repository_

1. Clone this repo by using the `git clone` functionality.
2. Remove `.git` directory
3. [Initialize a new public repository on GitHub](https://github.com/new) by following instructions from "create a new repository on the command line" section (appeared after pressing "Create repository" button).
