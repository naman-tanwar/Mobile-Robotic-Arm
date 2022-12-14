# Mobile Robotic Arm
A 3D-printed Robotic arm mounted on Aluminium 2020 chassis which can be controlled over the internet from anywhere in the world. Ideal for remote exploration.
## Abstract
Exploration of various unreachable/hazardous places is a key to scientifically enrich our understanding of this universe. And remotely performing operations on these locations opens more aspects of not even exploration but also providing medical aid where a real human intervention is not possible. This project aims to serve this goal of remote exploration and operation.

## Methodology
The working of the project can be defined as:
- 2 Ultrasonic sensors are used to measure the distance from any obstacle in any of the four directions.
- One of the Ultrasonic sensors is mounted on a servo motor which will help to measure the distance from a obstacle in 3 directions using only one sensor
- Wheels and the robot arms are controlled using ESP32 microcontroller
- A live camera is setup using Raspberry Pi which can be accessed remotely from any location. This makes the Robot perfect for operations in remote locations.
- The robot can be controlled remotely using blynk IoT Platform

## Mechanical Design and Implementaion
![N|Solid](https://github.com/namanteg/Mobile-Robotic-Arm/blob/main/design.png?raw=true)
## Circuit Diagram
![N|Solid](https://github.com/namanteg/Mobile-Robotic-Arm/blob/main/CIRCUIT.png?raw=true)

### Power Supply
A 12V 30A rated SMPS is used to supply power to the entire system. Sisce this power supply is temporary, It can be replaced with a suitable LiPo battery (preferably, 3S 35C 3500MAh) for mobility.
The Volatage is then step down to 5v using Buck Covertors so that it can be used to power other components.
The servo (Tower Pro MG995) can pull upto 1.5A to 2A of current under stall condition. So all the 6 servers can pull upto 12A of current. Hence, a separate buck converter (rated 20A) is used to power servos alone. The remaining buck convertor (rated 10A) is used to power rest of the circuit.

### ESP32 and connected devices
ESP32 is used as a microcontroller as it has WiFi functionality which can be used to connect to the internet.
Six Tower Pro MG995 servos are connected to the digital pins of ESP32 Microcontroller (D2, D4, D5, D15, D18, D19). L298N Motor Driver is used to control two 200 RPM geared DC motor. The inputs are connected to D12, D13, D14, D27 while enable is connected to D23 and D22. Two HCSRO4 Ultrasonic senosrs are used to measure distance in all four direction. Digital Pins D25, D26, D32, D33 are used for connections of ultrasonic sensors.
### Raspberry Pi
A Raspberry Pi camera is connected to Raspberry Pi 3B+ which can be used to view camera preview remotely.
### Circuit Assembly
The circuit is made on a PCB board. Male header pins are used to connect the servo motors, Ultrasonic sensors, Motor Driver input and power input for Raspberry Pi. Femal headers are used to mount ESP 32 on the PCB.
In the top left, two Buck convertors can be seen mounted with some screws on the PCB.In the bottom, L298N motor driver is mounted with some screws.
