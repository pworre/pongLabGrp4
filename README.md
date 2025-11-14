# TTK4155 – Embedded and Industrial Computer Systems

##  Project Overview
This repository contains the project developed as part of the NTNU course **TTK4155 – Embedded and Industrial Computer Systems**, at the **Department of Engineering Cybernetics**.  
The course focuses on the integration of **embedded hardware** and **real-time software**, requiring precision, collaboration, and a deep understanding of both hardware and software systems.

Our project explores the design and implementation of an embedded system combining sensors, actuators, and real-time software to achieve robust and reliable control performance.

---

## Node 1: Atmega 162
This node is responsible for the game-controller side and the main game-logic. It establish **SPI** for the OLED-display, io-controller and for the CAN-controller *MCP2515*. We also use UART with RS232. An SRAM is connected to the atmega for more storage. **CAN-bus** is used for connection with Node 2. 

## Node 2: Arduinoe Due
This node is connected to the pong-board and is responsible driving the servo- and DC-motor, and to detect goals when the ball is passing through the line between the IR-emitter and photo-diode. 

---

##  Objectives
- Develop a functional embedded system integrating hardware and software components  
- Implement real-time control and communication  
- Gain hands-on experience with low-level programming and hardware debugging  

---

##  About the Course
**Course:** TTK4155 – Embedded and Industrial Computer Systems  
**Institution:** Norwegian University of Science and Technology (NTNU)  
**Department:** Engineering Cybernetics (ITK)  
**Semester:** Fall 2025  

---

##  Project Team
| Name | Study Program |
|------|----------------|
| **Marie Anne Dam-Nielsen** | Cybernetics and Robotics |
| **Paul Eirik Worre** | Cybernetics and Robotics |
| **Øystein Martinsen** | Cybernetics and Robotics |

---

##  Technologies and Tools
- **Microcontroller platform:** Atmega 162, sam3x
- **Programming languages:** C
- **Communication protocols:** UART, SPI, CAN
- **Version control:** Git & GitHub  

