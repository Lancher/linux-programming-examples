/*  Copyright (C) 2015  Steve (unicornonair@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Retrieve Real UID, Effective UID and Saved UID.

// Example - 1
// Show user ID:
// ```
// $ id
// uid=1000(vagrant) gid=1000(vagrant) groups=1000(vagrant)
// ```

// Example - 2
// Show all the IDs of a process:
// ```
// $ ps -eo pid,ruid,euid,suid | grep YOUR_PID_HERE
// 2062  1000  1000  1000
// ```

// Example - 3
// The `a.out` is started by your login shell so it will be the vagrant ID.
// ```
// $ sudo chown root:root ./a.out
// $ ./a.out
// ruid=1000, euid=1000, suid=1000
// ruid=1000, euid=1000, suid=1000
// ```

// Example - 4
// Command "chmod 4755" will change the euid and suid to the same value as owner ID root 0:
// ```
// $ sudo chown root:root ./a.out
// $ sudo chmod 4755 ./a.out
// $ ./a.out
// ruid=1000, euid=0, suid=0
// ruid=1000, euid=1000, suid=1000
// ```

// Example - 5
// Change the euid and suid to owner ID nobody 65534:
// ```
// $ sudo chown nobody ./a.out
// $ sudo chmod 4755 ./a.out
// $ ./a.out
// uid=1000, euid=65534, suid=65534
// setuid(1000)
// setuid() : Success
// ruid=1000, euid=1000, suid=65534
// ```

// Example - 6
// The "sudo" means you login as root to execute “a.out”:
// ```
// $ sudo ./a.out
// uid=0, euid=0, suid=0
// setuid(1000)
// setuid() : Operation not permitted
// ruid=1000, euid=1000, suid=1000
// ```

// Example - 7
// The "sudo" means you login as root to execute “a.out” but "chmod 4755" will change the euid and suid to the same value as owner ID nobody 65534:
// ```
// $ sudo chown nobody ./a.out
// $ sudo chmod 4755 ./a.out
// $ sudo ./a.out
// uid=0, euid=65534, suid=65534
// setuid(1000)
// setuid() : Operation not permitted
// ruid=0, euid=65534, suid=65534
// ```

#include <stdio.h>
#include <unistd.h>             // getresuid()

#include <errno.h>              // errno, perror()

int
main(int argc, char const *argv[])
{
  uid_t id = 1000;
  uid_t ruid = -1, euid = -1, suid = -1;

  // Get the three UIDs.
  getresuid (&ruid, &euid, &suid);
  printf("uid=%d, euid=%d, suid=%d\n", ruid, euid, suid);

  // Set Real UID.
  setuid(id);
  printf ("setuid(%d)\n", id);
  perror ("setuid() ");

  // Get the three UIDs.
  getresuid (&ruid, &euid, &suid);
  printf ("ruid=%d, euid=%d, suid=%d\n", ruid, euid, suid);

  return 0;
}