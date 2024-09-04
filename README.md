# Detecting speed on a bicycle using a GPS Neo-6M sensor

## Schematic
![N|Solid](https://github.com/fafnir96/Detecting-speed-on-a-bicycle-using-a-GPS-Neo-6M-sensor/blob/main/schematic.jpg?raw=true)

| Component PIN | ESP32 PIN |
| ------ | ------ |
| Pin RX | 16 |
| Pin TX | 17 |
| SDA | 21 |
| SCL | 22 |

## Purpose
The purpose of this research is to design and develop a speed detection system for bicycles using the GPS Neo-6M sensor, with the capability to send notifications via WhatsApp if the bicycle speed exceeds a specified limit. This system aims to provide a practical solution for real-time speed monitoring and enhance rider safety. The primary objectives of this system are:

1. Accurately Measure Bicycle Speed: Utilize the GPS Neo-6M sensor to accurately measure the bicycle's speed. The system should be capable of collecting real-time speed data and ensuring measurement reliability.
2. Set Speed Limits: Configure the speed limit according to relevant needs or regulations. The system must monitor the bicycle speed and compare it with the preset limit.
3. Send Notifications for Excess Speed: Develop a system that can automatically send notifications via WhatsApp to the user or relevant parties when the bicycle speed exceeds the set limit. This aims to provide alerts and increase awareness of excessive speed.


## Setup
1. Clone project
2. Build the circuit according to the schematic
3. Open project in Arduino IDE
4. Visit this link to get the WhatsApp API: `https://randomnerdtutorials.com/esp32-send-messages-whatsapp/`
4. Change the Wi-Fi SSID and password, as well as the phone number and API Key in the code.
5. Upload the code to ESP32
