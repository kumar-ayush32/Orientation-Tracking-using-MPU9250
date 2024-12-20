# Orientation-Tracking-using-MPU9250
## Objective
The objective of the project is to Track the real-time orientation of MPU and plot it in Matplotlib and transmit data it to a particular server using ESP32's inbuilt WiFi.
## Methodology
The MPU9250 is a versatile 9-axis MotionTracking device, combining a 3-axis gyroscope, 3-axis accelerometer, and 3-axis magnetometer in a single package. This allows it to measure orientation, motion, and magnetic fields, making it a powerful sensor for robotics, IoT, and navigation applications.

In this project, I:

1. Connected the MPU9250 to an ESP32 microcontroller.

2. Uploaded Arduino code to interface with the sensor.

3. Wrote a Python code to process the sensor data and plot its orientation in real time.

### Software tools
#### Arduino IDE: Used for writing, compiling, and uploading code to the ESP32.

#### Python Environment: Python code processes the data sent by the ESP32.

#### Key libraries: 

  1. Serial: Facilitates serial communication between ESP32 and the Python script.

  2. NumPy: Handles numerical operations, such as parsing sensor data.

  3. Matplotlib: Visualizes orientation data in a 3D plot.

#### Real-Time Plotting:

Matplotlib’s 3D plotting capabilities help visualize real-time orientation. Quiver plots dynamically display the acceleration vector.

Select the correct board (Tools > Board > ESP32 Dev Module) and port (Tools > Port > COMx)

Open the Serial Monitor (Tools > Serial Monitor) and set the baud rate to 115200. You should see accelerometer and gyroscope data being printed.

### Hardware Circuit
![Circuit](https://github.com/user-attachments/assets/c761e26e-3715-48a0-96f6-a026a0ee8613)
### Data send to Laptop
![Screenshot (146)](https://github.com/user-attachments/assets/7c57ab58-645f-421b-aad4-da6015c34b3a)
### Results
<img src="https://github.com/user-attachments/assets/f755fdde-9787-48da-bfcd-024676beda37" alt="Screenshot (182)" width="400" height="400">
<img src="https://github.com/user-attachments/assets/1e45f6cf-5e41-4679-a642-efe670eef32c" alt="Screenshot (151)" width="400" height="400">

<img src="https://github.com/user-attachments/assets/5eec01a3-0e9e-4fd6-b1a7-064bcedb13a6" alt="Video1" width="400" height="500">
<img src="https://github.com/user-attachments/assets/4f778c25-0a0c-4075-93b9-21bb47ea0827" alt="Video2" width="400" height="500">

## How to run the code
Download the arudino ide and run the files.

For simulation:
```bash
pip install serial matplotlib numpy
```
Then you are good to go
