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

// Create a share anonymous mapping.

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
  int *addr;
  int fd;

  // Open /dev/zero.
  if ((fd = open ("/dev/zero", O_RDWR)) < 0) {
    perror ("open() ");
    exit (EXIT_FAILURE);
  }

  // Create a private file mapping.
  addr = mmap (NULL, sizeof (int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    perror ("mmap() ");
    exit (EXIT_FAILURE);
  }

  // Close fd.
  if (close (fd) == -1) {
    perror ("close() ");
    exit (EXIT_FAILURE);
  }

  // Init with 1.
  *addr = 1;

  // Create a child process.
  switch (fork ()) {
    case -1:
      perror ("fork() ");
      exit (EXIT_FAILURE);

    case 0:  // child
      printf("Child started, value = %d\n", *addr);
      (*addr)++;

      // Unmap.
      if (munmap (addr, sizeof (int)) == -1) {
        perror ("munmap() ");
        exit (EXIT_FAILURE);
      }

      exit (EXIT_SUCCESS);

    default:  // parent
      // Wait child process.
      if (wait (NULL) == -1) {
        perror ("wait() ");
        exit (EXIT_FAILURE);
      }
      printf("In parent, value = %d\n", *addr);

      // Unmap.
      if (munmap (addr, sizeof (int)) == -1) {
        perror ("munmap() ");
        exit (EXIT_FAILURE);
      }

      exit (EXIT_SUCCESS);
  }

}