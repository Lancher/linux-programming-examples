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

// Pipe between child and parent process.

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>             // fork(), getpid(), getppid()
#include <string.h>             // strlen()

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int fds[2];
  char buf[100];
  int num;

  // Usage.
  if (argc != 2) {
    printf ("usage: %s <string>\n", argv[0]);
    exit (EXIT_FAILURE);
  }

  // Create pipe.
  if (pipe (fds) < 0) {
    perror ("pipe() ");
    exit (EXIT_FAILURE);
  }

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

      // Read from pipe.
      for (;;) {
        num = read (fds[0], buf, sizeof (buf));
        if (num < 0) {  // error
          perror ("read() ");
          exit (EXIT_FAILURE);
        }
        else if (num == 0) {  // read end
          break;
        }
        else {
          if (write (STDOUT_FILENO, buf, num) != num) {
            perror ("read() ");
            exit (EXIT_FAILURE);
          }
        }
      }

      // flush to terminal
      write (STDOUT_FILENO, "\n", 1);

      // close read end.
      if (close (fds[0]) < 0) {
        perror ("close() ");
        exit (EXIT_FAILURE);
      }

      exit (EXIT_SUCCESS);

    // default
    default:
      // close read end.
      if (close (fds[0]) < 0) {
        perror ("close() ");
        exit (EXIT_FAILURE);
      }

      // write
      if (write (fds[1], argv[1], strlen (argv[1])) != strlen (argv[1])) {
        perror ("write() ");
        exit (EXIT_FAILURE);
      }

      // close write end.
      if (close (fds[1]) < 0) {
        perror ("close() ");
        exit (EXIT_FAILURE);
      }

      wait (NULL);
      exit (EXIT_SUCCESS);

  }

}