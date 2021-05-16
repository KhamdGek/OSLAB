# LAB -5 THREAD POOLING 

## Designing a Thread Pool
# TASK
Thread pools were introduced in Section 4.5.1. When thread pools are used, a task is submitted
to the pool and executed by a thread from the pool. Work is submitted to the pool using a queue,
and an available thread removes work from the queue. If there are no available threads, the work
remains queued until one becomes available. If there is no work, threads await notification until a
task becomes available. This project involves creating and managing a thread pool, and it can be
completed using Pthreds and POSIX synchronization.


