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

// Send multiple signals to self process. Observe the signal mask and
// signal pendiing.

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>             // getpid()
#include <signal.h>             // signal(), sigemptyset(), sigaddset(), sigprocmask()

#include <errno.h>              // errno, perror()

// Function declarations
void handler (int);

int
main (int argc, char **argv)
{
  int i;
  sigset_t new_set, old_set;

  printf("Process ID: %d.\n", getpid());

  // Add SIGINI with a handler.
  if (signal (SIGINT, handler) == SIG_ERR) {
    perror ("signal() ");
    exit (EXIT_FAILURE);
  }

  // Create a mask on SIGINT.
  if (sigemptyset (&new_set) < 0) {
    perror ("sigemptyset() ");
    exit (EXIT_FAILURE);
  }
  if (sigaddset (&new_set, SIGINT)) {
    perror ("sigaddset() ");
    exit (EXIT_FAILURE);
  }

  // Block the signal SIGINT.
  if (sigprocmask (SIG_BLOCK, &new_set, &old_set) < 0) {
    perror ("sigprocmask() ");
    exit (EXIT_FAILURE);
  }
  printf ("Block signal %d.\n", SIGINT);

  // Send signal to self process
  for (i = 0; i < 5; i++) {
    if (kill (getpid(), SIGINT) < 0) {
      perror ("kill() ");
      exit (EXIT_FAILURE);
    }
    printf ("Send signal %d to pid %d.\n", SIGINT, getpid());
  }

  // sleep() is one of those functions that is never re-started when interrupted.
  for (i = 0; i < 10; i++)
    sleep (1);

  // Unblock the signal SIGINT.
  if (sigprocmask (SIG_SETMASK, &old_set, NULL) < 0) {
    perror ("sigprocmask() ");
    exit (EXIT_FAILURE);
  }
  printf ("Unblock signal %d.\n", SIGINT);

  // while loop, waiting signal.
  for (;;)
    pause();

}

// This handler uses non-async-signal-safe functions(printf(), exit()).
// Yout should use "volatile sig_atomic_t" as a global flag.
void handler (int sig) {
  static int count = 0;

  if (count >= 3)
    exit(EXIT_SUCCESS);
  else {
    printf("Get %d SIGINT\n", count);
    count++;
  }
}