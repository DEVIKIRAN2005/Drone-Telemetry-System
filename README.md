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

<img width="751" height="593" alt="Screenshot 2026-05-01 022654" src="https://github.com/user-attachments/assets/7a08ed08-852e-4e6e-9b45-e4bec29af1bc" />
<img width="815" height="392" alt="Screenshot 2026-05-01 022717" src="https://github.com/user-attachments/assets/da31e00a-e97c-458f-aa60-9b0831d7a1f2" />
<img width="823" height="544" alt="Screenshot 2026-05-01 022751" src="https://github.com/user-attachments/assets/75e57664-149a-4f89-b5ef-13f1b7561d95" />




## 📌 Key Highlights

* Real-time data pipeline
* Embedded + networking integration
* Fault simulation (GPS loss)
* Battery monitoring & alerts

## 👨‍💻 Author

K DEVIKIRAN A HEGDE
