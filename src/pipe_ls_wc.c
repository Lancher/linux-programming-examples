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

// Simulate `ls | wc -l`.

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>             // fork(), getpid(), getppid()
#include <string.h>             // strlen()

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int fds[2];
  int status;
  pid_t pid;

  // Create pipe.
  if (pipe (fds) < 0) {
    perror ("pipe() ");
    exit (EXIT_FAILURE);
  }

  // First "ls" process.
  switch (fork ()) {
    // fork() failed.
    case -1:
      perror ("fork() ");
      exit (EXIT_FAILURE);

    // child process.
    case 0:
      // close read end.
      if (close (fds[0]) < 0) {
        perror ("close() ");
        exit (EXIT_FAILURE);
      }

      if (fds[1] != STDOUT_FILENO) {
        if (dup2 (fds[1], STDOUT_FILENO) < 0) {
          perror ("dup2() ");
          exit (EXIT_FAILURE);
        }
        // close unused fd.
        if (close (fds[1]) < 0) {
          perror ("close() ");
          exit (EXIT_FAILURE);
        }
      }

      // exec ls.
      execlp ("ls", "ls", (char *) NULL);
      exit (EXIT_FAILURE);

    // default
    default:
      break;
  }

  // Second "wc -l" process.
  switch (fork ()) {
    // fork() failed.
    case -1:
      perror ("fork() ");
      exit (EXIT_FAILURE);

    // child process.
    case 0:
      // close write end.
      if (close (fds[1]) < 0) {
        perror ("close() ");
        exit (EXIT_FAILURE);
      }

      if (fds[0] != STDIN_FILENO) {
        if (dup2 (fds[0], STDIN_FILENO) < 0) {
          perror ("dup2() ");
          exit (EXIT_FAILURE);
        }
        // close unused fd.
        if (close (fds[0]) < 0) {
          perror ("close() ");
          exit (EXIT_FAILURE);
        }
      }

      // exec ls.
      execlp ("wc", "wc", "-l", (char *) NULL);
      exit (EXIT_FAILURE);

    // default
    default:
      break;
  }

  // Parent process.

  // close unused fd.
  if (close (fds[0]) < 0) {
    perror ("close() ");
    exit (EXIT_FAILURE);
  }
  if (close (fds[1]) < 0) {
    perror ("close() ");
    exit (EXIT_FAILURE);
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

}