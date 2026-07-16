# Real-time-weather-monitoring-system-with-SMS-alert
# Real-Time Weather Monitoring System

## Overview

The **Real-Time Weather Monitoring System** is an embedded systems project developed using the **LPC2129 ARM7 microcontroller**. The system continuously monitors the ambient temperature using an **LM35 temperature sensor** interfaced through an **MCP3204 12-bit SPI ADC**, displays the current time and temperature on a **16×2 LCD**, and automatically sends an SMS alert through a **SIM800L GSM module** whenever the temperature exceeds a predefined threshold (**35°C**).

The current time is obtained from a **DS1307 Real-Time Clock (RTC)** using the **I²C protocol**. A custom degree (`°`) symbol is created using the LCD's **CGRAM** for a cleaner user interface.

To avoid sending multiple SMS alerts repeatedly, a one-hour lockout timer is implemented using **Timer1**.

---

# Features

* Real-time temperature monitoring using **LM35**
* High-resolution temperature conversion using **MCP3204 (12-bit ADC)**
* Real-time clock using **DS1307 RTC**
* Displays current time in **HH:MM** format
* Displays temperature with one decimal place
* Custom **°C** symbol using LCD CGRAM
* Automatic SMS alert when temperature exceeds **35°C**
* One-hour delay before sending the next SMS alert
* Uses four communication peripherals:

  * GPIO
  * SPI
  * I²C
  * UART

---

# Hardware Used

| Component          | Description                     |
| ------------------ | ------------------------------- |
| LPC2129            | ARM7 Microcontroller            |
| LM35               | Temperature Sensor              |
| MCP3204            | 12-bit SPI ADC                  |
| DS1307             | Real Time Clock                 |
| SIM800L            | GSM Module                      |
| 16×2 LCD           | HD44780 Compatible (4-bit mode) |
| 32.768 kHz Crystal | RTC Oscillator                  |
| Pull-up Resistors  | I²C Bus                         |
| Power Supply       | 5V                              |

---

# Software Used

* **Keil µVision**
* **Proteus Professional**
* Embedded C

---

# Communication Interfaces

## SPI

Used for communication between the LPC2129 and the MCP3204 ADC.

| LPC2129 | MCP3204 |
| ------- | ------- |
| SCK     | CLK     |
| MISO    | DOUT    |
| MOSI    | DIN     |
| P0.7    | CS      |

---

## I²C

Used to communicate with the DS1307 RTC.

| LPC2129 | DS1307 |
| ------- | ------ |
| P0.2    | SCL    |
| P0.3    | SDA    |

---

## UART

Used to communicate with the SIM800L GSM module.

| LPC2129 | SIM800L |
| ------- | ------- |
| TXD0    | RX      |
| RXD0    | TX      |

Baud Rate:

```text
9600 bps
```

---

## LCD Interface

16×2 LCD operated in **4-bit mode**.

| LCD Pin | LPC2129 |
| ------- | ------- |
| D4-D7   | GPIO    |
| RS      | GPIO    |
| E       | GPIO    |
| RW      | GND     |

---

# Working Principle

1. The LM35 measures the surrounding temperature.
2. The analog voltage from the LM35 is converted into digital data using the MCP3204 ADC through SPI.
3. The ADC value is converted into voltage and then into temperature.
4. The DS1307 RTC provides the current time through the I²C bus.
5. The LCD continuously displays:

   * Current Time
   * Temperature
6. If the temperature exceeds **35°C**:

   * An SMS warning is sent using the SIM800L GSM module.
   * Timer1 starts counting.
7. Additional SMS alerts are blocked for one hour.
8. After one hour, the system is ready to send another alert if the temperature is still above the threshold.

---

# Temperature Calculation

LM35 Output:

```text
10 mV / °C
```

ADC Conversion:

```text
Voltage = (ADC Count × 3.3) / 4096
```

Temperature:

```text
Temperature (°C) = Voltage × 100
```

---

# LCD Display

Example:

```text
TIME: 10:45
Temp: 34.8°C
```

The degree (`°`) symbol is generated using the LCD's CGRAM.

---

# SMS Alert

When the temperature exceeds **35°C**, the SIM800L sends a message similar to:

```text
Temp is 36.7 degree celsius

Day is too hot!!!
Take care
```

---

# Project Structure

```text
main.c
│
├── LCD Driver
│     ├── LCD Initialization
│     ├── Command
│     ├── Data
│     ├── Integer Display
│     ├── Float Display
│     └── CGRAM Custom Character
│
├── SPI Driver
│     ├── SPI Initialization
│     ├── SPI Read
│     └── MCP3204 Driver
│
├── I2C Driver
│     ├── Start
│     ├── Stop
│     ├── Read
│     ├── Write
│     └── DS1307 Interface
│
├── UART Driver
│     ├── UART Initialization
│     ├── SMS Functions
│     └── Float Transmission
│
└── Timer Functions
      ├── Delay
      └── One-hour SMS Timer
```

---

# How to Run

1. Open the project in **Keil µVision**.
2. Build the project.
3. Load the generated HEX file into the LPC2129 in **Proteus**.
4. Connect:

   * LM35
   * MCP3204
   * DS1307
   * SIM800L
   * LCD
5. Run the simulation.
6. Vary the LM35 temperature in Proteus.
7. Observe:

   * Temperature updates on the LCD.
   * Time updates from the RTC.
   * SMS alert is transmitted when the temperature exceeds **35°C**.

---

# Applications

* Weather Monitoring Stations
* Server Room Temperature Monitoring
* Industrial Temperature Monitoring
* Greenhouse Automation
* Smart Home Monitoring
* Laboratory Equipment Monitoring

---

# Future Improvements

* Add humidity sensing using **DHT11/DHT22**
* Log temperature data to an SD card
* Upload readings to a cloud platform using Wi-Fi
* Add buzzer and relay control
* Implement adjustable temperature thresholds through a keypad
* Display maximum and minimum recorded temperatures
* Add battery backup monitoring
* Integrate with a mobile application for remote monitoring

---

# Author

**Kirran Vignesh**

**Platform:** LPC2129 (ARM7)

**Language:** Embedded C

**Development Tools:** Keil µVision, Proteus Professional

**Communication Protocols:** GPIO, SPI, I²C, UART
