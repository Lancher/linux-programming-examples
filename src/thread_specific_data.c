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

// Example of thread-specific data.

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>            // pthread_create(), pthread_join()

#include <errno.h>              // errno, perror()

// global variables
static pthread_key_t buffer_key;
static pthread_once_t buffer_key_once = PTHREAD_ONCE_INIT;

// function declarations
void *thread_func (void *);
static void buffer_key_alloc ();
static void buffer_destroy (void *);

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
  if (pthread_join (t, NULL) < 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  exit (EXIT_SUCCESS);
}

// thread func
void *thread_func (void *arg) {

  char *str = (char *) arg;

  // Create the key only once.
  pthread_once (&buffer_key_once, buffer_key_alloc);

  // Allocate buffer.
  pthread_setspecific (buffer_key, malloc (100));

  // Get buffer.
  char *buf = (char *) pthread_getspecific (buffer_key);

  // Copy str.
  snprintf (buf, 100, "%s", str);

  pthread_exit (0);
}

// Allocate the key.
static void buffer_key_alloc() {
  pthread_key_create (&buffer_key, buffer_destroy);
}

// Free the thread-specific buffer.
static void buffer_destroy (void * buf) {
  free (buf);
}
