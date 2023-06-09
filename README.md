# ThreadIn
Freeze target threads (external - internal ) by avoiding SuspendThread detections. Or access registers from start address.

# Description

Changes thread context for set RIP to target function or WaitMessage.

WaitMessage ( https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-waitmessage )
```
Yields control to other threads when a thread has no other messages in its message queue. The WaitMessage function suspends the thread and does not return until a new message is placed in the thread's message queue.
```
![Thread stack]([http://url/to/img.png](https://github.com/illegal-instruction-co/ThreadIn/blob/main/thread%20stack.png?raw=true))
