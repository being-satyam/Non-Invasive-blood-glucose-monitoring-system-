Non-Invasive Blood Glucose Monitoring System
This project presents a non-invasive blood glucose monitoring system using near-infrared (NIR) technology. The system integrates hardware, firmware, and software components to provide a pain-free, cost-effective solution for continuous glucose monitoring. FreeRTOS ensures efficient task scheduling, and interrupt handling enables real-time data processing
Overview
Traditional blood glucose monitoring methods are invasive, painful, and expensive. This project uses 940 nm infrared sensors to estimate blood glucose levels non-invasively. The system detects voltage variations caused by glucose concentration in blood and processes this data using a custom algorithm implemented on an ESP32 
Key Features
Non-invasive monitoring: No skin pricking required.
Near-Infrared Technology: Utilizes 940 nm IR light for glucose detection.
Real-Time Processing: Uses interrupts for immediate voltage sampling and response.
FreeRTOS Integration: Ensures multitasking for data acquisition, processing, and IoT alerts.
IoT Functionality: Real-time monitoring and notifications via smartphones.
Linear Regression Model: Achieves a Mean Squared Error (MSE) of 5.27.
Objectives
1. Develop a non-invasive blood glucose monitoring system.
2. Ensure real-time, reliable glucose readings with minimal error using NIR technology.
3. Utilize FreeRTOS for task management and interrupts for efficient signal handling.
4. Integrate IoT functionality for wireless data transmission and real-time alerts.
5. Lay the foundation for future wearable health devices.
Methodology
Working Principle
1. Light Interaction: Glucose in the blood interacts with IR rays, causing absorption based on glucose concentration.
2. Voltage Analysis: Voltage output from the photodiode is processed to determine glucose levels.
3. Algorithm: Linear regression maps voltage values to glucose concentrations in mg/dL.
Software Components
FreeRTOS: Manages tasks like data acquisition, processing, and communication.
Task 1: Data analysis and glucose calculation
Task 2: Interrupt handling 
Interrupts: Used for real-time voltage sampling.
Arduino IDE: For microcontroller programming.
Python with Scikit-learn: Implements the linear regression model for data analysis.
Twilio API: Enables IoT-based notifications.
Hardware Components
ESP32 Microcontroller
940 nm IR LED (1550nm for better precision)
Vishay BPW34 Photodiode (InGaAs recommended)
AD620 Instrumentation Amplifier
Results
Established a linear relationship between voltage and glucose concentration.
Achieved a Mean Squared Error (MSE) of 5.27.
Developed a generalized formula for glucose prediction:
Glucose Concentration (mg/dL)=62.23×𝑉−117.90 where V is the voltage from the IR sensor.
Real-Time Processing with FreeRTOS and Interrupts
Task Scheduling: FreeRTOS ensures efficient allocation of processing time across tasks like data acquisition, analysis, and communication.
Interrupt Handling: The ESP32 triggers interrupts for voltage sampling, enabling real-time response to changes in glucose levels.
Future Improvements
Enhance sensor sensitivity and algorithm accuracy.
Collect more extensive datasets for improved calibration.
Expand real-time processing capabilities for additional health metrics.
Develop portable, wearable solutions for continuous glucose monitoring.
