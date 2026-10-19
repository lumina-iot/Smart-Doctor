# Smart-Doctor
# Smart Doctor – ESP32 & Fog Computing IoT Health Monitoring System

**Smart Doctor** is an IoT-based edge and fog health monitoring system built with an **ESP32** microcontroller, ambient temperature sensing, and simulated patient vital sensors. The system processes critical health data locally (fog processing) to trigger real-time LED and LCD alerts while simultaneously synchronizing data with the **Blynk Cloud Dashboard** for remote access and automated event notifications.

---

## 🌟 Key Features

* **Edge/Fog Decision Making**: Evaluates patient vital signs (Heart Rate and Systolic Blood Pressure) locally on the ESP32 for zero-latency alert toggling.
* **Dual Monitoring Display**:
  * **Local LCD (16x2 I2C)**: Displays real-time Heart Rate, Blood Pressure, Room Temperature, and System Status (`NORMAL` / `ALERT`).
  * **Blynk Cloud Dashboard**: Streams real-time sensor metrics to virtual pins (`V0`–`V3`) and triggers automated push alerts (`health_alert`).
* **Visual Status Indicators**:
  * 🟢 **Green LED**: Indicates normal, healthy vital parameters.
  * 🔴 **Red LED**: Indicates abnormal vital ranges or environmental warnings.
* **Ambient Monitoring**: Uses a **DHT22** sensor to monitor room temperature alongside patient vitals.

---

## 🛠 Hardware & Components

| Component | Description / Pin Assignment |
| :--- | :--- |
| **ESP32 Microcontroller** | Central processing unit with Wi-Fi connectivity |
| **Potentiometer 1 (Heart Rate)** | Analog input connected to `GPIO 34` (Simulated 40–140 BPM) |
| **Potentiometer 2 (Systolic BP)** | Analog input connected to `GPIO 35` (Simulated 70–180 mmHg) |
| **DHT22 Temperature Sensor** | Digital input connected to `GPIO 15` |
| **16x2 I2C LCD Display** | SDA: `GPIO 21`, SCL: `GPIO 22` (Address `0x27`) |
| **Red LED (Alert)** | Digital output connected to `GPIO 26` |
| **Green LED (Normal)** | Digital output connected to `GPIO 27` |

---

## 📊 Threshold & Operating Logic

The system evaluates simulated vitals using local threshold checking:

| Metric | Safe / Normal Range | Alert Threshold |
| :--- | :--- | :--- |
| **Heart Rate** | $50 - 120\text{ BPM}$ | $< 50\text{ BPM}$ or $> 120\text{ BPM}$ |
| **Systolic BP** | $90 - 140\text{ mmHg}$ | $< 90\text{ mmHg}$ or $> 140\text{ mmHg}$ |
| **Room Temp** | $\le 35.0^\circ\text{C}$ | $> 35.0^\circ\text{C}$ (Environmental Warning) |

---

## ☁️ Blynk Datastream Mapping

| Datastream Name | Pin | Data Type | Description |
| :--- | :--- | :--- | :--- |
| **Heart Rate** | `V0` | Integer | Patient simulated heart rate (BPM) |
| **Systolic BP** | `V1` | Integer | Patient simulated blood pressure (mmHg) |
| **Room Temperature** | `V2` | Double | Ambient room temperature ($^\circ\text{C}$) |
| **Patient Status** | `V3` | String | System status (`NORMAL` / `ALERT`) |

---

## 🚀 Getting Started

### Prerequisites
* [Arduino IDE](https://www.arduino.cc/en/software) or [Wokwi Simulator](https://wokwi.com)
* Required Libraries:
  * Blynk
  * `DHT sensor library`
  * `LiquidCrystal I2C`

### Code Configuration
1. Clone or download this repository.
2. Open the `.ino` sketch file.
3. Replace the Blynk credentials at the top of the file with your template credentials:
   ```cpp
   char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";
├── sketch.ino        # Main ESP32 Arduino source code
├── diagram.json      # Wokwi simulation circuit configuration
├── libraries.txt     # Dependency library list for Wokwi execution
└── README.md         # Project documentation
