# Sistemas Operativos - 2024

This repository contains all my laboratory projects from the Operating Systems course (FAMAF, 2024).

## Course Structure

### [Lab 1](./lab1/) - Shell Implementation ( MyBash )
**Completed:** September 2024  
**Topics Covered:**
- Process creation and management (fork, exec, wait)
- Inter-process communication using pipes
- File descriptors and I/O redirection
- System calls and UNIX concurrency primitives
- Abstract Data Types (ADTs) and modular design

**Key Features:**
- Command execution in foreground and background modes
- Input/output redirection (<, >)
- Pipeline support connecting multiple commands (|)
- Built-in commands: cd, help, exit

[View Lab 1 Details →](./lab1/README.md)

---

### [Lab 2](./lab2/) - Semaphores Implementation in XV6
**Completed:** October 2024  
**Topics Covered:**
- System call implementation in kernel space
- Named semaphores (POSIX-style)
- Process synchronization mechanisms
- Kernel programming in RISC-V architecture
- Race conditions and concurrency control

**Key Features:**
- Four syscalls: sem_open(), sem_up(), sem_down(), sem_close()
- Global semaphore system accessible to all processes
- Kernel-level synchronization primitives (acquire, release,sleep, wakeup)
- User-space program "pingpong" demonstrating process synchronization
- Prevention of race conditions and deadlocks

[View Lab 2 Details →](./lab2/README.md)

---

### [Lab 3](./lab3/) - CPU Scheduler Implementation (MLFQ)
**Completed:** November 2024  
**Topics Covered:**
- CPU scheduling algorithms (Round Robin, MLFQ)
- Process states and context switching
- Quantum and time slicing mechanisms
- Performance analysis and benchmarking
- I/O-bound vs CPU-bound process behavior

**Key Features:**
- Analysis of xv6-riscv's original Round Robin scheduler
- Implementation of Multi-Level Feedback Queue (MLFQ) scheduler
- Three priority levels with dynamic priority adjustment
- Performance benchmarking tools (cpubench, iobench)
- Comparative analysis: quantum size effects on different workloads
- Comprehensive report with graphs and measurements

[View Lab 3 Details →](./lab3/README.md)

---

## Technologies Used

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Git](https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white)

- **Language:** C
- **Development Environment:** Linux
- **Tools:** GCC, Make, GDB, Valgrind
- **Concepts:** Process management, Memory management, File systems, Concurrency

## Learning Outcomes

Through these projects, I gained hands-on experience with:
- Low-level system programming in C
- POSIX APIs and system calls
- Process creation and management
- Inter-process communication
- Memory allocation and management
- Debugging and performance optimization


- University: FAMAF (Facultad de Matemática, Astronomía, Física y Computación)
- Course: Operating Systems (2024)

## License

This project is part of academic coursework at FAMAF.

---

Built with ❤️ as part of my Computer Science journey | [Portfolio](https://theotherflaneur.vercel.app/)
