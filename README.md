 🐳 Mini Container Runtime with Kernel Memory Monitor

## 📌 Project Overview

This project implements a lightweight container runtime in C along with a Linux Kernel Module (LKM) to monitor and enforce memory limits on containers.

The system is divided into:

- **User-space Engine (`engine.c`)** → Creates and manages containers  
- **Kernel-space Monitor (`monitor.c`)** → Tracks memory and enforces limits  

---

## ⚙️ Features

- Run multiple containers using `clone()`
- Namespace isolation:
  - PID namespace
  - UTS namespace
  - Mount namespace
- Real-time logging using pipes + threads
- Kernel-level memory monitoring
- Soft and hard memory limit enforcement
- Automatic process termination (SIGKILL)

- output screenshots
- <img width="737" height="562" alt="123564" src="https://github.com/user-attachments/assets/24ca5da3-0a9a-46e6-8d19-1ee07a181a03" />
<img width="775" height="671" alt="ssss1" src="https://github.com/user-attachments/assets/c325e332-cfcd-4508-8e0f-e61931534659" />
<img width="798" height="595" alt="sss9" src="https://github.com/user-attachments/assets/f70f8a34-a002-4fd0-b4d3-de45a299ed4c" />
<img width="556" height="457" alt="sss8" src="https://github.com/user-attachments/assets/9ca8e92e-06d9-4207-9062-891fbf0081b2" />
<img width="1253" height="801" alt="sss7" src="https://github.com/user-attachments/assets/f55bbdc6-449a-44cf-ae98-ded96da3d800" />





