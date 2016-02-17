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

// Create a Posix share anonymous mapping.

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
  char *addr;
  int fd, flags;
  char name [] = "/pshm", str [] = "Hello World";
  mode_t perms;
  struct stat st;

  // Read wrtite flags.
  flags = O_RDWR | O_CREAT;

  // Permissions.
  perms = 0664;

  // Open named share memory.
  if ((fd = shm_open (name, flags, perms)) < 0) {
    perror ("shm_open() ");
    exit (EXIT_FAILURE);
  }

  // Named share memory is always init 0 size, so resize it to str length.
  if (ftruncate (fd, strlen (str)) < 0) {
    perror ("ftruncate() ");
    exit (EXIT_FAILURE);
  }

  // Create a shared anonymous mapping.
  addr = mmap (NULL, strlen (str), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    perror ("mmap() ");
    exit (EXIT_FAILURE);
  }

  // Copy str.
  memcpy (addr, str, strlen (str));

  // Close fd.
  if (close (fd) == -1) {
    perror ("close() ");
    exit (EXIT_FAILURE);
  }

  // Unmap.
  if (munmap (addr, sizeof (int)) == -1) {
    perror ("munmap() ");
    exit (EXIT_FAILURE);
  }

  // Create a child process.
  switch (fork ()) {
    case -1:
      perror ("fork() ");
      exit (EXIT_FAILURE);

    case 0:  // child

      // Open named share memory.
      if ((fd = shm_open (name, flags, perms)) < 0) {
        perror ("shm_open() ");
        exit (EXIT_FAILURE);
      }

      // Get file descriptor stats.
      if (fstat (fd, &st) < 0) {
        perror ("fstat() ");
        exit (EXIT_FAILURE);
      }

      // Create a shared anonymous mapping.
      addr = mmap (NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      if (addr == MAP_FAILED) {
        perror ("mmap() ");
        exit (EXIT_FAILURE);
      }

      // print memory.
      write (STDOUT_FILENO, addr, st.st_size);

      exit (EXIT_SUCCESS);

    default:  // parent
      break;
  }

  // Wait child process.
  if (wait (NULL) == -1) {
    perror ("wait() ");
    exit (EXIT_FAILURE);
  }

  exit (EXIT_SUCCESS);

}