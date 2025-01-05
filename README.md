# CMU 15-445/645: Database Systems Course

This repository is a personal workspace for my journey through Carnegie Mellon University's course **15-445/645 - Database Systems**. The course focuses on the principles and practices of modern database systems, covering foundational topics such as storage, indexing, query optimization, concurrency control, and recovery.

## About This Repository

- **Course Materials**: Includes notes, code snippets, and summaries of key lectures and readings.
- **Assignments**: My implementations of various projects and assignments, such as buffer pool management, B+ trees, and transaction management.
- **Additional Resources**: Links to supplementary readings, tools, and references that I've found helpful.

## Key Topics Covered

- **Storage and Indexing**: Understanding how data is physically stored and accessed efficiently.
- **Query Processing and Optimization**: Learning the execution pipeline of database queries and how to optimize them.
- **Concurrency Control**: Techniques for ensuring database consistency in multi-user environments.
- **Recovery Mechanisms**: Implementing and understanding system robustness against failures.

## Tools and Languages

- **Programming Language**: Primarily C++ (used for projects).
- **Database Systems**: Hands-on exploration with PostgreSQL and other systems.
- **Utilities**: Git for version control and GDB for debugging.

## Getting Started

To explore my work:

1. Clone this repository:
   ```bash
   https://github.com/AldonahZero/CMU-15-445-645.git
   ```
2. Navigate to the relevant folder to find project-specific details.

## Disclaimer

This repository is for personal learning and educational purposes. It adheres to the academic integrity guidelines of CMU. Code and solutions provided here are my original work and are shared responsibly to promote learning.

---

Feel free to follow this repository for updates and insights as I progress through the course. Suggestions and feedback are always welcome!

## Build

We recommend developing BusTub on Ubuntu 22.04, or macOS (M1/M2/Intel). We do not support any other environments (i.e., do not open issues or come to office hours to debug them). We do not support WSL. The grading environment runs
Ubuntu 22.04.

### Linux (Recommended) / macOS (Development Only)

To ensure that you have the proper packages on your machine, run the following script to automatically install them:

```console
# Linux
$ sudo build_support/packages.sh
# macOS
$ build_support/packages.sh
```

Then run the following commands to build the system:

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```

If you want to compile the system in debug mode, pass in the following flag to cmake:
Debug mode:

```console
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make -j`nproc`
```
This enables [AddressSanitizer](https://github.com/google/sanitizers) by default.

If you want to use other sanitizers,

```console
$ cmake -DCMAKE_BUILD_TYPE=Debug -DBUSTUB_SANITIZER=thread ..
$ make -j`nproc`
```

There are some differences between macOS and Linux (i.e., mutex behavior) that might cause test cases
to produce different results in different platforms. We recommend students to use a Linux VM for running
test cases and reproducing errors whenever possible.
