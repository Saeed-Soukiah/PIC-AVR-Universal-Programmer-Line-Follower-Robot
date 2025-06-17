# 🔧 PIC/AVR Universal Programmer & Line Follower Robot

A comprehensive embedded system project combining a **universal programmer** for PIC, AVR, and EEPROM microcontrollers with the design of a **self-programmable PIC microcontroller** and a **line-following robot** using that MCU.

---

## 📌 Project Overview

This project includes:
- Designing a **self-programmable PIC programmer** that connects directly to a PC without external programmers.
- Building a **line-following robot** controlled by the same self-programmed PIC microcontroller.
- Support for **PIC**, **AVR**, and **8051** families by adapting connections and bootloader code.
- Using **Arduino boards** as a flexible flashing tool.

---

## 🌟 Project Features

- **Low cost**, using widely available components.
- **Flexible MCU support** — compatible with most PIC microcontrollers.
- **Universal flashing capabilities** using Arduino as a programmer.
- **Hardware scalability**, allowing integration into other embedded devices.
- Potential for **cost and connection optimization** by modifying the PCB and design.

---

## 💡 Applications

- Ideal for **low-cost embedded projects**.
- Eliminates the need for dedicated **USB-to-TTL interfaces** by using a PIC MCU with integrated USB or a **V-USB software implementation**.
- Reduced wiring and better integration with custom electronics.

---

## 🔩 Hardware Components

- **PIC18F2550** microcontroller with built-in USB support.
- **DC Gear Motor** for the robot chassis.
- **L298N motor driver**.
- **9V battery** as a mobile power source.
- **Arduino board** repurposed as a universal programmer for PIC microcontrollers.

---

## 🚧 Development Challenges

- Lack of **Arabic-language references** for some project concepts.
- Scarcity of specific **microcontrollers and components**.
- Unavailability of accurate circuit schematics for real-world application.
- Inability to fully simulate the system using standard tools.
- Requirement of **advanced programming skills** and in-depth hardware debugging.
- High cost and occasional failure of components due to poor quality.

---

## ✅ Solutions Adopted

- Sourced **English references, forums, and open-source projects** to bridge knowledge gaps.
- **Step-by-step troubleshooting and iterative testing**.
- Cross-referenced academic material for precise component calculations.
- Found **reliable electronics vendors** in Damascus.
- Explored open-source circuits and designs to learn from proven implementations.

---

## 🧠 Development Methodology

### 1. Software Development:
- Cleanly structured code with consideration for **real-time systems**, low resource usage, and modularity.
- Designed using **structured programming principles**, targeting reliability on limited-resource MCUs.

### 2. Hardware Development:
- Built and refined circuits on breadboards, maintaining electrical and safety standards.
- Continuous maintenance and testing during all development phases.
- Used **Proteus simulation software** to validate code–hardware integration prior to physical testing.
- Implemented modeling and simulation for parts not testable on-site.

> ✳️ Final validation involved syncing software and hardware, refining performance, and **protecting embedded code** from unauthorized replication.

---

## 👤 Author

Designed and developed by [Saeed].

---

## 📜 License

This project is open-source. Contributions and forks are welcome!
