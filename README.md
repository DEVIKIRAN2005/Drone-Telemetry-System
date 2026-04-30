# 🚀 Drone Telemetry System

A real-time multi-drone telemetry system built using C (QNX) and Python.

## 🔧 Features

* Real-time drone position tracking (Latitude, Longitude)
* Velocity, Speed, and Heading calculation
* Battery simulation 🔋
* Collision detection ⚠️
* Multi-layer architecture:

  * Drone (data generation)
  * Client (processing)
  * Ground station (visualization)

## 🧠 System Architecture

DroneSender → Client → Ground Station (Python)

## ⚙️ Technologies Used

* C (QNX / POSIX sockets)
* Python (matplotlib)
* UDP Communication

## ▶️ How to Run

### 1. Compile

make

### 2. Run client

./client

### 3. Run ground station

python groundstation.py

### 4. Run drone sender

./DroneSender

## 📸 Output

(Add screenshot here)

## 📌 Key Highlights

* Real-time data pipeline
* Embedded + networking integration
* Fault simulation (GPS loss)
* Battery monitoring & alerts

## 👨‍💻 Author

Your Name
