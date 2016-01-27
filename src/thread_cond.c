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

// Simple producers and comsumers.

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>            // pthread_create(), pthread_join()

#include <errno.h>              // errno, perror()

// Global variables
static int producer_val = 0;
static int consumer1_val = 0;
static int consumer2_val = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Function declarations
void *producer_thread_func (void *);

void *consumer1_thread_func (void *);
void *consumer2_thread_func (void *);

int
main (int argc, char **argv)
{
  pthread_t p, c1, c2;

  // Create a thread p.
  if (pthread_create (&p, NULL, producer_thread_func, NULL) != 0) {
    perror ("pthread_create() ");
    exit (EXIT_FAILURE);
  }

  // Create a thread c1.
  if (pthread_create (&c1, NULL, consumer1_thread_func, NULL) != 0) {
    perror ("pthread_create() ");
    exit (EXIT_FAILURE);
  }

  // Create a thread c2.
  if (pthread_create (&c2, NULL, consumer2_thread_func, NULL) != 0) {
    perror ("pthread_create() ");
    exit (EXIT_FAILURE);
  }

  // Wait the thread p to be done.
  if (pthread_join (p, NULL) != 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  // Wait the thread c1 to be done.
  if (pthread_join (c1, NULL) != 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  // Wait the thread c1 to be done.
  if (pthread_join (c2, NULL) != 0) {
    perror ("pthread_join() ");
    exit (EXIT_FAILURE);
  }

  printf("producer_val: %d\n", producer_val);
  printf("consumer1_val: %d\n", consumer1_val);
  printf("consumer2_val: %d\n", consumer2_val);

  exit (EXIT_SUCCESS);
}

void *consumer1_thread_func (void *arg) {

  while (consumer1_val + consumer2_val < 1000000 / 2) {
    // Lock.
    if (pthread_mutex_lock (&mutex) != 0) {
      perror ("pthread_mutex_lock() ");
      exit (EXIT_FAILURE);
    }

    // Always use while loop to wrap predicate.
    while (producer_val == 0) {
        if (pthread_cond_wait (&cond, &mutex) != 0) {
          perror ("pthread_cond_wait() ");
          exit (EXIT_FAILURE);
        }
    }

    // Consume all.
    while (producer_val > 0) {
      --producer_val;
      ++consumer1_val;
    }

    // Unlock.
    if (pthread_mutex_unlock (&mutex) != 0) {
      perror ("pthread_mutex_unlock() ");
      exit (EXIT_FAILURE);
    }
  }
  return NULL;
}

void *consumer2_thread_func (void *arg) {

  while (consumer1_val + consumer2_val < 1000000 / 2) {
    // Lock.
    if (pthread_mutex_lock (&mutex) != 0) {
      perror ("pthread_mutex_lock() ");
      exit (EXIT_FAILURE);
    }

    // Always use while loop to wrap predicate.
    while (producer_val == 0) {
        if (pthread_cond_wait (&cond, &mutex) != 0) {
          perror ("pthread_cond_wait() ");
          exit (EXIT_FAILURE);
        }
    }

    // Consume all.
    while (producer_val > 0) {
      --producer_val;
      ++consumer2_val;
    }

    // Unlock.
    if (pthread_mutex_unlock (&mutex) != 0) {
      perror ("pthread_mutex_unlock() ");
      exit (EXIT_FAILURE);
    }
  }
  return NULL;
}

void *producer_thread_func (void *arg) {
  int i;

  for (i = 0; i < 1000000; ++i) {
    // Lock.
    if (pthread_mutex_lock (&mutex) != 0) {
      perror ("pthread_mutex_lock() ");
      exit (EXIT_FAILURE);
    }

    // incremant val.
    ++producer_val;

    // Unlock.
    if (pthread_mutex_unlock (&mutex) != 0) {
      perror ("pthread_mutex_unlock() ");
      exit (EXIT_FAILURE);
    }

    // Signal the condition.
    if (pthread_cond_signal (&cond) != 0) {
      perror ("pthread_cond_signal() ");
      exit (EXIT_FAILURE);
    }
  }
  return NULL;
}