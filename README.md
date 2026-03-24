*This project has been created as part of the 42 curriculum by luida-cu.*

## Description
Codexion is an advanced concurrency and resource management simulation written in C. It models a scenario where multiple coders must share limited USB dongles to compile their work. It implements custom threading synchronization and a Min-Heap priority queue to handle First-In-First-Out (FIFO) and Earliest-Deadline-First (EDF) scheduling.

## Instructions
**Compilation:**
Run `make` at the root of the repository to compile the `codexion` executable.

**Execution:**
`./codexion [num_coders] [burnout_time] [compile_time] [debug_time] [refactor_time] [required_compiles] [dongle_cooldown] [scheduler]`
*Note: The scheduler must be exactly 'fifo' or 'edf'.*

## Thread synchronization mechanisms
This project leverages POSIX threads (`pthread_create`, `pthread_join`) to represent each coder and the monitor. 
Shared resources are strictly protected using Mutexes (`pthread_mutex_t`):
* **Dongles:** Each dongle has a mutex. A coder must lock both adjacent dongles to compile.
* **Print Mutex:** Guarantees terminal output is perfectly serialized.
* **Coder Mutex:** Protects internal timestamps and compile counts from data races between the Coder thread and the Monitor thread.
* **Condition Variables (`pthread_cond_t`):** Used to manage waiting queues for dongles, utilizing `pthread_cond_broadcast` and `pthread_cond_timedwait` to enforce dongle cooldown periods efficiently.

## Blocking cases handled
* **Deadlock Prevention:** Prevented through an asymmetrical resource request hierarchy.
* **Starvation Prevention:** A 10ms stagger is introduced at initialization for even-numbered coders, allowing odd-numbered coders to grab dongles immediately and establishing a natural rotation.
* **Data Races:** Eliminated by providing every coder with a personal `coder_mutex` to protect read/write operations on their timestamps.
* **Precise Burnout Detection:** A dedicated monitor thread yields the CPU using 1ms sleeps to check timestamps, guaranteeing burnout detection within the strict 10ms tolerance limit.

## Resources
* POSIX Threads Programming Documentation
* Coffman's Conditions for Deadlock
* AI Usage: AI was utilized as a peer-programmer to help optimize the Min-Heap array mathematics for the scheduler and debug Valgrind/Helgrind race conditions by isolating unprotected variable reads.
