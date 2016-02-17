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

// Change virtual memory protection.

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>              // O_RDONLY
#include <string.h>             // memset()
#include <unistd.h>             // fork(), getpid(), getppid(), STDOUT_FILENO
#include <sys/mman.h>           // mmap()
#include <sys/stat.h>           // struct stat

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  char cmd[100];
  char *addr;
  int fd, page_size;

  // Get page size.
  page_size = sysconf (_SC_PAGESIZE);

  // Open /dev/zero.
  if ((fd = open ("/dev/zero", O_RDWR)) < 0) {
    perror ("open() ");
    exit (EXIT_FAILURE);
  }

  // Create a private file mapping.
  addr = mmap (NULL, page_size, PROT_NONE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    perror ("mmap() ");
    exit (EXIT_FAILURE);
  }

  // Close fd.
  if (close (fd) == -1) {
    perror ("close() ");
    exit (EXIT_FAILURE);
  }

  // Create cmd.
  snprintf (cmd, sizeof (cmd), "cat /proc/%ld/maps | grep zero", (long) getpid());
  system (cmd);

  // Change virtual memory protection.
  if (mprotect (addr, page_size, PROT_READ | PROT_WRITE) == -1) {
    perror ("mprotect() ");
    exit (EXIT_FAILURE);
  }

  // Create cmd.
  snprintf (cmd, sizeof (cmd), "cat /proc/%ld/maps | grep zero", (long) getpid());
  system (cmd);

}