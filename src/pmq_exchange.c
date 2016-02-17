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

// Using POSIX message queue exchage data between parent process and child process.

#include <stdio.h>
#include <stdlib.h>

#include <string.h>             // memset()
#include <unistd.h>             // fork(), getpid(), getppid()
#include <mqueue.h>             // mq_open(), mq_getattr(), mq_receive(), mq_send()
#include <fcntl.h>              // O_* constants
#include <sys/stat.h>           // mode_t constants

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int i, flags, status, msg_num = 5;
  char name [] = "/mq", msg [2048];
  mode_t perms;
  mqd_t mqd;
  struct mq_attr attr;
  pid_t pid;

  // Set message queue attributes.
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = 2048;

  // Read wrtite flags.
  flags = O_RDWR | O_CREAT;

  // Permissions.
  perms = 0664;

  // Create message queue.
  if ((mqd = mq_open (name, flags, perms, &attr)) < 0) {
    perror ("mq_open() ");
    exit (EXIT_FAILURE);
  }

  if (mq_getattr (mqd, &attr) < 0) {
    perror ("mq_getattr() ");
    exit (EXIT_FAILURE);
  }
  printf ("Maximum of messages on queue: %ld\n", attr.mq_maxmsg);
  printf ("Maximum message size: %ld\n", attr.mq_msgsize);
  printf ("Messages currently on queue: %ld\n", attr.mq_curmsgs);

  // Create a child process.
  pid = fork();

  // pid == -1, failed to create a process.
  if (pid < 0) {
    perror ("fork() ");
    exit (EXIT_FAILURE);
  }
  // pid == 0, the child process.
  else if (pid == 0) {

    // Received 5 messages.
    for (i = 0; i < msg_num; ++i) {
      memset (msg, 0, sizeof (msg));
      if (mq_receive (mqd, msg, sizeof (msg), NULL) < 0) {
        perror ("mq_receive() ");
        exit (EXIT_FAILURE);
      }
      else {
        printf ("Received [%s]\n", msg);
      }
    }
    exit (EXIT_SUCCESS);
  }
  // pid > 0, the parent process..
  else {
  }

  // Send 5 messages.
  for (i = 0; i < msg_num; ++i) {
    memset (msg, 0, sizeof (msg));
    snprintf (msg, sizeof (msg), "message %d", i);
    if (mq_send (mqd, msg, strlen (msg), 30) < 0) {
      perror ("mq_send() ");
      exit (EXIT_FAILURE);
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

  // Unlink the queue.
  mq_unlink (name);

  exit (EXIT_SUCCESS);
}