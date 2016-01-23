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

// Simply create a thread.

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>            // pthread_create(), pthread_join()

#include <errno.h>              // errno, perror()

// Function declarations
void *thread_func (void *);

int
main (int argc, char **argv)
{
  pthread_t t;
  void *result;

  // Create a thread.
  if (pthread_create (&t, NULL, thread_func, "Hello World") < 0) {
    perror ("pthread_create() ");
    exit (EXIT_FAILURE);
  }

  // Wait the thread to be done.
  if (pthread_join (t, &result) < 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  printf ("Main thread get [%ld].\n", (long) result);

  exit (EXIT_SUCCESS);
}

// Do nothing.
void *thread_func (void *arg) {
  char *str = (char *) arg;
  printf ("Thread get [%s].\n", str);
  return (void *) strlen (str);
}