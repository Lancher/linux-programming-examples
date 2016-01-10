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

int
main (int argc, char **argv)
{
  int sig, r;

  // Usage.
  if (argc != 3) {
    printf ("Usage: %s <process id> <signal num>\n", argv[0]);
    exit (EXIT_SUCCESS);
  }

  // Send the signal.
  // If sig is equal to 0 (null signal), means the kill() will only check if the process
  // can be signaled.
  if (kill (atoi (argv[1]), atoi(argv[2])) < 0) {
    // Not permission.
    if (errno == EPERM)
      perror ("kill() ");
    // Process not exist.
    else if (errno == ESRCH)
      perror ("kill() ");
    exit (EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}