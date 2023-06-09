# ThreadIn
Freeze target threads (external - internal ) by avoiding SuspendThread detections. Or access registers from start address.

# Description

Changes thread context for set RIP to target function or WaitMessage.

WaitMessage ( https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-waitmessage )
```
Yields control to other threads when a thread has no other messages in its message queue. The WaitMessage function suspends the thread and does not return until a new message is placed in the thread's message queue.
```
![thread stack](https://github.com/illegal-instruction-co/ThreadIn/assets/79029454/9d27e24f-6d5c-4e2e-813a-99c44e35e4a2)
