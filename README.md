## Linux programming in C

#### Introduction :

Some simple linux programming codes.

--

#####  Memory and Allocaton:

- Memory alignment please refer to `Sven-Hendrik Haase - Alignment in C`.

| Code | Descriptions  |
| --- | --- |
| [malloc_free_sbrk.c](src/malloc_free_sbrk.c) | Observe how a program heap size changed by mallocing and freeing memory. |
| [memory_align.c](src/memory_align.c) | Demonstrate how "posix_memalign()" work. |

--

##### Users and Groups:

- Retrieve info in `/etc/passwd` and `/etc/group`.

| Code | Descriptions  |
| --- | --- |
| [users_groups.c](src/users_groups.c) | Fetch users and groups info in `/etc/passwd` and `/etc/group`. |

--

##### Process Credentials:

- Relation between real user ID(real uid, or ruid), the effective user ID (effective uid, or euid), and the saved user ID (saved uid, or suid).

- Please refer to paper `Setuid Demystified` for more details.

| Code | Descriptions  |
| --- | --- |
| [process_cred.c](src/process_cred.c) | Observe a process's real UID, effective UID and save UID.|

--

##### Time:

- Conversions between `time()`, `gmtime()`, `localtime()`, `asctime()`, `ctime()` and `mktime().`

- Process Time is divide to `User CPU time`(the time of the program access CPU) and `System CPU Time`(the time of kernel system call, page faults..).

| Code | Descriptions  |
| --- | --- |
| [time.c](src/time.c) | Time functions.|
| [process_time.c](src/process_time.c) | Get the User CPU Time and System CPU Time.|

--

##### File I/O:

- The `read()` and “write()` system calls don’t directly initiate disk access. Instead, they simply copy data between a user-space buffer and a buffer in the kernel buffer cache.


| Code | Descriptions  |
| --- | --- |
| [direct_io.c](src/direct_io.c) | Using `O_DIRECT` to bypass the buffer cache.|
| [file_info.c](src/file_info.c) | Get file flags and permission.|
| [inotify.c](src/inotify.c) | Use inotify to monitor files or directories .|

--

##### Signal:

- If signal number is equal to 0, means the `kill()` will only check if the process can be signaled.

- If the same signal is generated multiple times while it is blocked, then it is recorded in the set of pending signals, and later delivered, just once.

| Code | Descriptions |
| --- | --- |
| [signal_handler.c](src/signal_handler.c) | Construct a simple signal handler.|
| [signal_send.c](src/signal_send.c) | Use a signal to check if a process exist.|
| [signal_pending.c](src/signal_pending.c) | Multiple signals delivery to a process with signal mask.|

--

##### Process:

- The `fork()` allow one process, the parent, to create a child process. The child obtains the copies of stack, data, heap and text segments.

- File descriptors is also share between parent and child (use `dup()`).

- The `vfork()` will not copy the parent process's memory space until `exec()` or exit()`.

| Code | Descriptions |
| --- | --- |
| [process_child.c](src/process_child.c) | Create a child process.|
| [process_vfork.c](src/process_vfork.c) | How vfork() is different from fork().|
| [process_sig.c](src/process_sig.c) | Send signal from child to parent.|
| [process_exit_handler.c](src/process_exit_handler.c) | Demonstrate the "atexit()" and "on_exit()".|
| [process_wait.c](src/process_wait.c) | Wait children.|
| [process_zombie.c](src/process_zombie.c) | Create a Zombie process.|

--

##### Thread:

- Use `clone()` to create a thread.

| Code | Descriptions |
| --- | --- |
| [thread_create.c](src/thread_create.c) | Create a simple thread.|
| [thread_mutex.c](src/thread_mutex.c) | Create two threads to increment a value.|
| [thread_cond.c](src/thread_cond.c) | Simple producers and comsumers.|
| [thread_specific_data.c](src/thread_specific_data.c) | Example of thread-specific data.|
| [thread_cleanup_handler.c](src/thread_cleanup_handler.c) | Cancel a thread and thread handlers.|

--

##### Group process and control:

| Code | Descriptions |
| --- | --- |
| [pgroup_setpgid.c](src/pgroup_setpgid.c) | Set new a process group of a child process.|

--

##### Daemon:

| Code | Descriptions |
| --- | --- |
| [daemon.c](src/daemon.c) | Create a daemon process.|

--

##### Libraries:

Build the static library.

```
>> make
>> gcc -g -o main main.o -Llib -ldemo
```

| Code | Descriptions |
| --- | --- |
| [build_static_lib](src/build_static_lib) | Build the static library.|