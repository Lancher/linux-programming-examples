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

// Simply create two threads increment a value.

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>            // pthread_create(), pthread_join()

#include <errno.h>              // errno, perror()

// Global variables
static int global_val = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function declarations
void *thread_func (void *);

int
main (int argc, char **argv)
{
  pthread_t t1, t2;
  void *result;

  // Create a thread t1.
  if (pthread_create (&t1, NULL, thread_func, NULL) != 0) {
    perror ("pthread_create() ");
    exit (EXIT_FAILURE);
  }

  // Create a thread t2.
  if (pthread_create (&t2, NULL, thread_func, NULL) != 0) {
    perror ("pthread_create() ");
    exit (EXIT_FAILURE);
  }

  // Wait the thread t1 to be done.
  if (pthread_join (t1, NULL) != 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  // Wait the thread t2 to be done.
  if (pthread_join (t2, NULL) != 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  printf("global_val: %d\n", global_val);

  exit (EXIT_SUCCESS);
}

void *thread_func (void *arg) {
  int i;

  for (i = 0; i < 1000000; ++i) {
    // Lock.
    if (pthread_mutex_lock (&mutex) != 0) {
      perror ("pthread_mutex_lock() ");
      exit (EXIT_FAILURE);
    }

    // incremant val.
    ++global_val;

    // Unlock.
    if (pthread_mutex_unlock (&mutex) != 0) {
      perror ("pthread_mutex_unlock() ");
      exit (EXIT_FAILURE);
    }
  }
  return NULL;
}