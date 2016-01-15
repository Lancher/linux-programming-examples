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

// Send signal from child to parent.

#include <stdio.h>
#include <stdlib.h>

#include <signal.h>             // kill(), signal(), sigemptyset(), sigaddset(), sigprocmask()
#include <unistd.h>             // fork(), getpid(), getppid()

#include <errno.h>              // errno, perror()

// Function declarations
void handler (int);

int
main (int argc, char **argv)
{
  pid_t pid;
  sigset_t new_set, old_set;

  // Add SIGINI with a handler.
  if (signal (SIGUSR1, handler) == SIG_ERR) {
    perror ("signal() ");
    exit (EXIT_FAILURE);
  }

  // Create a mask on SIGUSR1.
  if (sigemptyset (&new_set) < 0) {
    perror ("sigemptyset() ");
    exit (EXIT_FAILURE);
  }
  if (sigaddset (&new_set, SIGUSR1)) {
    perror ("sigaddset() ");
    exit (EXIT_FAILURE);
  }

  // Block the signal SIGUSR1.
  if (sigprocmask (SIG_BLOCK, &new_set, &old_set) < 0) {
    perror ("sigprocmask() ");
    exit (EXIT_FAILURE);
  }
  printf ("Block signal %d.\n", SIGUSR1);

  // Create a child process.
  pid = fork();

  // pid == -1, failed to create a process.
  if (pid < 0) {
    perror ("fork() ");
    exit (EXIT_FAILURE);
  }
  // pid == 0, the child process.
  else if (pid == 0) {
    printf ("The child is doing something.\n");
    sleep(3);

    // Send dignal to parent.
    if (kill (getppid (), SIGUSR1) < 0) {
      perror ("kill() ");
      _exit (EXIT_FAILURE);
    }
    printf ("The child send signal to parent.\n");
  }
  // pid > 0, the parent process.
  else {
    printf ("The parent is waiting siganl.\n");

    // Waiting signal.
    if ((sigsuspend (&old_set) < 0) && (errno != EINTR)) {
      perror ("sigsuspend() ");
      exit (EXIT_FAILURE);
    }
    printf ("The parent got siganl.\n");
  }

  exit (EXIT_SUCCESS);
}

// Do nothing.
void handler (int sig) {

}