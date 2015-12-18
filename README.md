## Linux programming in C

#### Introduction :

Some simple linux programming codes.

--

##### # Memory and Allocaton:

- Memory alignment please refer to `Sven-Hendrik Haase - Alignment in C`.

| Code | Descriptions  |
| --- | --- |
| [malloc_free_sbrk.c](src/malloc_free_sbrk.c) | Observe how a program heap size changed by mallocing and freeing memory. |
| [memory_align.c](src/memory_align.c) | Demonstrate how "posix_memalign()" work. |

--

##### # Users and Groups:

- Retrieve info in `/etc/passwd` and `/etc/group`.

| Code | Descriptions  |
| --- | --- |
| [users_groups.c](src/users_groups.c) | Fetch users and groups info in `/etc/passwd` and `/etc/group`. |

--

##### # Process Credentials:

Show user ID:
```
$ id
uid=1000(vagrant) gid=1000(vagrant) groups=1000(vagrant)
```

Show all the IDs of a process:
```
$ ps -eo pid,ruid,euid,suid | grep YOUR_PID_HERE
2062  1000  1000  1000
```

The `a.out` is started by your login shell so it will be the vagrant ID.
```
$ sudo chown root:root ./a.out
$ ./a.out
ruid=1000, euid=1000, suid=1000
ruid=1000, euid=1000, suid=1000
```

Command "chmod 4755" will change the euid and suid to the same value as owner ID root 0:
```
$ sudo chown root:root ./a.out
$ sudo chmod 4755 ./a.out
$ ./a.out
ruid=1000, euid=0, suid=0
ruid=1000, euid=1000, suid=1000
```

Change the euid and suid to owner ID nobody 65534:
```
$ sudo chown nobody ./a.out
$ sudo chmod 4755 ./a.out
$ ./a.out
uid=1000, euid=65534, suid=65534
setuid(1000)
setuid() : Success
ruid=1000, euid=1000, suid=65534
```

The "sudo" means you login as root to execute “a.out”:
```
$ sudo ./a.out
uid=0, euid=0, suid=0
setuid(1000)
setuid() : Operation not permitted
ruid=1000, euid=1000, suid=1000
```

The "sudo" means you login as root to execute “a.out” but "chmod 4755" will change the euid and suid to the same value as owner ID nobody 65534:
```
$ sudo chown nobody ./a.out
$ sudo chmod 4755 ./a.out
$ sudo ./a.out
uid=0, euid=65534, suid=65534
setuid(1000)
setuid() : Operation not permitted
ruid=0, euid=65534, suid=65534
```

| Code | Descriptions  |
| --- | --- |
| [process_cred.c](src/process_cred.c) | Observe a process's real UID, effective UID and save UID.|

--