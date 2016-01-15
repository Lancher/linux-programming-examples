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

// Demonstrate the "atexit()" and "on_exit()".

#include <stdio.h>
#include <stdlib.h>             // atexit(), on_exit()

#include <errno.h>              // errno, perror()

static void
atexit_func1 (void)
{
  printf ("atexit function 1 called\n");
}

static void
atexit_func2 (void)
{
  printf ("atexit function 2 called\n");
}

static void
onexit_func (int exitStatus, void *arg)
{
  printf("on_exit function called: status=%d, arg=%ld\n", exitStatus, (long) arg);
}

int
main (int argc, char **argv)
{
  if (on_exit (onexit_func, (void *) 10) != 0)
    perror ("on_exit() ");

  if (atexit (atexit_func1) != 0)
    perror ("atexit() ");

  if (atexit (atexit_func2) != 0)
    perror ("atexit() ");

  if (on_exit (onexit_func, (void *) 20) != 0)
    perror ("on_exit() ");

  // Exit with status 2.
  exit(2);
}