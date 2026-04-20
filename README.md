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


