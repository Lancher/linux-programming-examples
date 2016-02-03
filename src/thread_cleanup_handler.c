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

// Cancel a thread and thread handlers.

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>            // pthread_create(), pthread_join()

#include <errno.h>              // errno, perror()

// Global variables
static int global_val = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Function declarations
void *thread_func (void *);
void cleanup_handler (void *);

int
main (int argc, char **argv)
{
  pthread_t t;
  void *result;

  // Create a thread.
  printf ("Create a thread.\n");
  if (pthread_create (&t, NULL, thread_func, NULL) < 0) {
    perror ("pthread_create() ");
    exit (EXIT_FAILURE);
  }

  printf ("Sleep 2s.\n");
  sleep (2);

  // Cancel the thread.
  if (pthread_cancel (t) != 0) {
    perror ("pthread_cancel() ");
    exit (EXIT_FAILURE);
  }

  // Wait the thread to be done.
  if (pthread_join (t, &result) < 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  // Check the status of thread.
  if (result == PTHREAD_CANCELED)
    printf ("The thread was canceled.\n");
  else
    printf ("The thread terminated normally.\n");

  exit (EXIT_SUCCESS);
}

// Thread function.
void *thread_func (void *arg) {

  void *buf = malloc (100); // Not a cancellation point.

  if (pthread_mutex_lock (&mutex) != 0)  // Not a cancellation point.
    perror ("pthread_mutex_lock() ");

  // Push a cleanup handler.
  pthread_cleanup_push (cleanup_handler, buf);

  // Condition predicate.
  while (global_val == 0) {
    if (pthread_cond_wait (&cond, &mutex) != 0) // A cancellation point.
      perror ("pthread_cond_wait() ");
  }

  // Execute the cleanup handler.
  pthread_cleanup_pop (1);

  return NULL;
}

// Cleanup handler.
void cleanup_handler (void *arg) {
  // Free addresses.
  printf ("Free memory: %p.\n", arg);
  free (arg);

  // Unlock mutex.
  printf ("Unlock mutex.\n");
  if (pthread_mutex_unlock (&mutex) != 0)
    perror ("pthread_mutex_unlock() ");

}