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

// Wait children.

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>             // fork(), getpid(), getppid()

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int i;
  int status;
  pid_t pid;

  // Create 5 child processes.
  for (i = 0; i < 5; i++) {
    // Create a child process.
    pid = fork();

    // pid == -1, failed to create a process.
    if (pid < 0) {
      perror ("fork() ");
      _exit (EXIT_FAILURE);
    }
    // pid == 0, the child process.
    else if (pid == 0) {
      printf ("This is child with pid %u and parent pid %u\n", getpid(), getppid());
      // Sleep i seconds.
      sleep (i);
      // Exit with i status.
      _exit (EXIT_SUCCESS);
    }
    // pid > 0, the parent process..
    else {
      continue;
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