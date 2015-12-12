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

// Use 'posfix_memalign()' to align memory('memaling()' is obsolete).

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int i, r;
  void *ptr;

  for (i = 1; i <=4; i++) {
    r = posix_memalign (&ptr, 4096, 1000 * i);

    // Return 0 if there is no errors.
    if (r != 0) {
      perror ("posfix_memalign() ");
      exit (EXIT_FAILURE);
    }
    else {
      printf ("Address: %10p\n", ptr);
    }
  }

  exit (EXIT_SUCCESS);
}