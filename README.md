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
| [users_groups.c](src/users_groups.c) | Fetch users and groups info in "/etc/passwd" and "/etc/group". |

