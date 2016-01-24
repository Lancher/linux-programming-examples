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

// Observe the variation of heap size during malloc() and free().

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>             // sbrk()

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int i;
  int nums_of_blocks = 500, blocksize = 1024;
  int *ptr[nums_of_blocks];

  // Before mallocing huge memory, print the break point;
  printf ("Before malloc %d blocks, break point %10p\n", nums_of_blocks, sbrk (0));

  // Allocate nums_of_blocks * blocksize bytes memory.
  for (i = 0; i < nums_of_blocks; i++)
    ptr[i] = malloc (blocksize);

  // After mallocing huge memory, print the break point;
  printf ("After malloc %d, break point %10p\n", nums_of_blocks, sbrk (0));

  // Free the block from 0 ~ 498.
  for (i = 0; i < nums_of_blocks - 1; i++)
    free (ptr[i]);

  printf ("After free the block 0 ~ 498, break point %10p\n", sbrk (0));

  // Free the last block.
  free (ptr[nums_of_blocks - 1]);

  printf ("After free the last block, break point %10p\n", sbrk (0));

  exit (EXIT_SUCCESS);
}