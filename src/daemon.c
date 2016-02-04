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

// Create a daemon process.

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>             // fork(), getpid(), getppid()
#include <fcntl.h>              // O_RDWR
#include <sys/stat.h>           // umask()

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int fd, max_fd;

  // Create a child process.
  switch (fork ()) {
    case -1:
      exit (EXIT_FAILURE);
    case 0:
      break;
    default:
      exit (EXIT_SUCCESS);
  }

  // Set new session.
  if (setsid () < 0)
    exit (EXIT_FAILURE);

  // Make sure not a session leader (later open terminal will become control process).
  switch (fork ()) {
    case -1:
      exit (EXIT_FAILURE);
    case 0:
      break;
    default:
      exit (EXIT_SUCCESS);
  }

  // Set file permission to default.
  umask (0);

  // Change work directory (avoid of not able to unmount).
  chdir ("/");

  // Close all open file descriptors (avoid long-lived files opening).
  max_fd = sysconf (_SC_OPEN_MAX);
  for (fd = 0; fd < max_fd; fd++)
    close (fd);

  // Reopen standard file descriptor.
  close (STDIN_FILENO);
  fd = open("/dev/null", O_RDWR);

  if (fd != STDIN_FILENO)
    exit (EXIT_FAILURE);
  if (dup2 (STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
    exit (EXIT_FAILURE);
  if (dup2 (STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
    exit (EXIT_FAILURE);

  for (;;)
    sleep (1);

  exit (EXIT_SUCCESS);
}