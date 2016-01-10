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

// Signal handler for SIGINT.

#include <stdio.h>
#include <stdlib.h>

#include <signal.h>

#include <errno.h>              // errno, perror()

// Function declarations
void handler (int);

int
main (int argc, char **argv)
{
  if (signal (SIGINT, handler) == SIG_ERR) {
    perror ("signal() ");
    exit (EXIT_FAILURE);
  }

  // while loop, waiting signal.
  for (;;)
    pause();

}

// This handler uses non-async-signal-safe functions(printf(), exit()).
void handler (int sig) {
  static int count = 0;

  if (count >= 3)
    exit(EXIT_SUCCESS);
  else {
    printf("Get %d SIGINT\n", count);
    count++;
  }
}
