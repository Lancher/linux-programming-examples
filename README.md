## Linux programming examples in C

--

#####  Memory and Allocaton:

- Use `sbrk()` to observe the process heap size (`program break`).

- Memory alignment please refer to `Sven-Hendrik Haase - Alignment in C`.

| Code | Descriptions  |
| --- | --- |
| [malloc_free_sbrk.c](src/malloc_free_sbrk.c) | Observe the variation of heap size during malloc() and free().|
| [memory_align.c](src/memory_align.c) | Demonstrate how "posix_memalign()" work. |

--

##### Users and Groups:

- Retrieve infomation in `/etc/passwd` and `/etc/group`.

| Code | Descriptions  |
| --- | --- |
| [users_groups.c](src/users_groups.c) | Fetch users and groups info in `/etc/passwd` and `/etc/group`. |

--

##### Process Credentials:

- Relation between real user ID(real uid, or ruid), the effective user ID (effective uid, or euid), and the saved user ID (saved uid, or suid).

- Privileged process can use `setuid()` to change three IDs to any value.

- Unprivileged process can use `setuid()` to change euid to the same value as ruid or suid.

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

- Use `inotify_init()` and `inotify_add_watch()` to monitor files or directories.

| Code | Descriptions  |
| --- | --- |
| [direct_io.c](src/direct_io.c) | Using `O_DIRECT` to bypass the buffer cache.|
| [file_info.c](src/file_info.c) | Get file flags and permission.|
| [inotify.c](src/inotify.c) | Use inotify to monitor files or directories.|

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

- Orphan process is parent process ended and child process keep running. The child process will adopt by `INIT` (pid==1) process.

- Zombie process is child exit before parent perform `wait*` operation. The Zombie process release almost all the resources but remains an entry in the kernel’s process table recording the child’s process ID.

- The zombies can NOT be killed by a signal, the only way to remove them from the system is to kill their parent.

```
# Kill the parent process of zombies.
kill $(ps -A -o stat,ppid | awk '/[zZ]/{print $2}')
```

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

- The simplest way to avoid d`eadlocks` is to define a mutex hierarchy. When threads can lock the same set of mutexes, they should always lock them in the same order.

- Thread-specific data is a technique for making an existing function thread-safe without changing its interface.

- Clear handlers will be called when the thread is canceled by `pthread_cancel()`.

| Code | Descriptions |
| --- | --- |
| [thread_create.c](src/thread_create.c) | Create a simple thread.|
| [thread_mutex.c](src/thread_mutex.c) | Create two threads to increment a value.|
| [thread_cond.c](src/thread_cond.c) | Simple producers and comsumers.|
| [thread_specific_data.c](src/thread_specific_data.c) | Example of thread-specific data.|
| [thread_cleanup_handler.c](src/thread_cleanup_handler.c) | Cancel a thread and thread handlers.|

--

##### Group process and control:

- A process group is a collection of related processes.

- A session is a collection of related process groups.

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

- Static libraries is known as `libxxxxx.a`.

- Share libraries is known as `libxxxxx.so`.

| Code | Descriptions |
| --- | --- |
| [build_static_lib](src/build_static_lib) | Build the static library.|
| [build_share_lib](src/build_share_lib) | Build the share library.|

--

##### Pipe:

| Code | Descriptions |
| --- | --- |
| [pipe_parent_child.c](src/pipe_parent_child.c) | Pipe between child and parent process.|
| [pipe_ls_wc.c](src/pipe_ls_wc.c) | Simulate `ls | wc -l`.|

--

##### Mmap:

- Four types of `Private File Mapping`, `Share File Mapping`, `Private Anonymous Mapping` and `Shared Anonymous Mapping`.

| Code | Descriptions |
| --- | --- |
| [mmap_pri_f_map.c](src/mmap_pri_f_map.c) | Create a private file mapping.|
| [mmap_sh_f_map.c](src/mmap_sh_f_map.c) | Create a shared file mapping.|
| [mmap_sh_a_map.c](src/mmap_sh_a_map.c) | Create a share anonymous mapping.|
| [mmap_mprotect.c](src/mmap_mprotect.c) | Change virtual memory protection.|

--

##### POSIX IPC:

- Posix Message Queues.

- Posix semaphore.

- Posix shared memory.

| Code | Descriptions |
| --- | --- |
| [pmq_exchange.c](src/pmq_exchange.c) | Using POSIX message queue exchage data between parent process and child process.|
| [psem_get_val.c](src/psem_get_val.c) | Using POSIX semaphore to race a value.|
| [pshm.c](src/pshm.c) | Create a Posix shared anonymous mapping.|
