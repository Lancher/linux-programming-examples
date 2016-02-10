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

// Create a private file mapping.

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>              // O_RDONLY
#include <unistd.h>             // STDOUT_FILENO
#include <sys/mman.h>           // mmap()
#include <sys/stat.h>           // struct stat

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  char *addr;
  int fd;
  struct stat st;

  if (argc != 2) {
    printf("%s <filename>\n", argv[0]);
    exit (EXIT_FAILURE);
  }

  if ((fd = open (argv[1], O_RDONLY)) < 0) {
    perror ("open() ");
    exit (EXIT_FAILURE);
  }

  // Get file stats.
  if (stat (argv[1], &st) < 0) {
    perror ("stat() ");
    exit (EXIT_FAILURE);
  }

  // Create a private file mapping.
  addr = mmap (NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == MAP_FAILED) {
    perror ("mmap() ");
    exit (EXIT_FAILURE);
  }

  // Output the file content
  if (write (STDOUT_FILENO, addr, st.st_size) != st.st_size) {
    perror ("write() ");
    exit (EXIT_FAILURE);
  }

  // Unmap.
  if (munmap (addr, st.st_size) == -1) {
    perror ("munmap() ");
    exit (EXIT_FAILURE);
  }

 exit (EXIT_SUCCESS);
}