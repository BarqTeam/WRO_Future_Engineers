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

# First Solution: Using the TCS3200 Color Sensor
* Color Detection: We initially programmed the TCS3200 sensor to identify specific colors on the track. An orange line signaled that the car should turn clockwise, while a blue line indicated a counterclockwise turn.

* Controlled Turning: Upon detecting a color, the car was instructed to turn at a specific angle. This angle was precisely controlled using an Inertial Measurement Unit (IMU), ensuring the car made accurate and consistent turns, regardless of external factors like speed or track conditions.

* Code Implementation: The process was automated through predefined functions such as detectOrangeLine and detectBlueLine, which triggered the appropriate turning actions based on the color detected.

This approach provided a straightforward method for determining the car's direction based on visual cues from the track, with the IMU ensuring precise turning control. However, this solution proved suboptimal due to the car's high speed and the limitations of the TCS3200, which was not the best choice for color detection in this context.







## Barq team's social accounts

## Overview
Our car is built on an RC structure featuring four wheels and a rear-wheel drive system. It is powered by a DC motor that controls the wheels and a servo motor for steering. The car is equipped with three ultrasonic sensors , gyroscope and a RGB color sensor to ensure it stays on the right path.

On the second level of the structure, we have placed the Arduino controller and the Raspberry Pi, along with a 12-volt battery down in the first floor, a camera, and other components that will be detailed in the parts section.



## How to prepare the repo based on the template

_Remove this section before the first commit to the repository_

1. Clone this repo by using the `git clone` functionality.
2. Remove `.git` directory
3. [Initialize a new public repository on GitHub](https://github.com/new) by following instructions from "create a new repository on the command line" section (appeared after pressing "Create repository" button).
