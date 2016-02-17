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

// Using POSIX semaphore to race a value.

#include <stdio.h>
#include <stdlib.h>

#include <string.h>             // memset()
#include <unistd.h>             // fork(), getpid(), getppid()
#include <semaphore.h>
#include <fcntl.h>              // O_* constants
#include <sys/stat.h>           // mode_t constants

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int i, flags, status, val = 1000;
  char name [] = "/sem";
  mode_t perms;
  sem_t *sem;
  pid_t pid;

  // Read wrtite flags.
  flags = O_RDWR | O_CREAT;

  // Permissions.
  perms = 0664;

  // Create message queue.
  if ((sem = sem_open (name, flags, perms, val)) < 0) {
    perror ("mq_open() ");
    exit (EXIT_FAILURE);
  }

  // Create 5 process to race the value.
  for (i = 0; i < 5; ++i) {
    // Fork a process.
    switch (fork ()) {
      // fork() failed.
      case -1:
        perror ("fork() ");
        exit (EXIT_FAILURE);

      // child process.
      case 0:
        val = 0;
        for (;;) {
          if (sem_wait (sem) == -1) {
            perror ("sem_wait() ");
            exit (EXIT_FAILURE);
          }
          ++val;
          printf ("Pid %u current val %d.\n", getpid(), val);
        }

      // default
      default:
        break;
    }
  }

  // Wait all the child processes.
  for (;;) {
    pid = wait (&status);
    if (pid == -1) {
      if (errno == ECHILD) {
        printf ("No More children\n");
        exit (EXIT_SUCCESS);
      }
      else {
        perror ("wait() ");
        exit (EXIT_FAILURE);
      }
    }
    else {
      printf ("Wait child with pid %u and status %d\n", pid, status);
    }
  }

  exit (EXIT_SUCCESS);
}