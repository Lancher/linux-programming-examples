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

// Using O_DIRECT to bypass kernel buffer cache.
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>          // ssize_t, size_t
#include <fcntl.h>              // O_RDONLY, O_DIRECT

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int r, fd;
  ssize_t numRead;
  size_t length;
  off_t offset;
  void *buf;

  // Usage.
  if (argc < 2) {
    printf ("Usage: %s <filepath>\n", argv[0]);
    exit (EXIT_SUCCESS);
  }

  // Open file with direct io.
  if ((fd = open (argv[1], O_RDONLY | O_DIRECT)) < 0) {
    perror ("open() ");
    exit (EXIT_FAILURE);
  }

  // The data buffer being transferred must be aligned to blocksize.
  r = posix_memalign (&buf, 4096, 10000);
  // The file or device offset at which data transfer commences must be a multiple of the block size.
  offset = 0;
  // The length of the data to be transferred must be a multiple of the block size.
  length = 4096;

  // Read operation.
  if ((numRead = read(fd, buf, length)) < 0) {
    perror ("read() ");
    exit (EXIT_FAILURE);
  }
  printf ("%d bytes read.", (int) numRead);

  exit(EXIT_SUCCESS);
}