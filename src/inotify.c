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

// Monitor a file or a directory.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/inotify.h>        // inotify(), struct inotify_event

#include <errno.h>              // errno, perror()

// Function declarations
void print_event (struct inotify_event *);

int
main (int argc, char **argv)
{
  int fd, wfd;
  int nums;
  char buf[sizeof (struct inotify_event) * 10];
  struct inotify_event *event;
  char *p;

  // Usage.
  if (argc != 2) {
    printf ("Usage: %s <path>\n", argv[0]);
    exit (EXIT_SUCCESS);
  }

  // Create inotify file descriptor.
  if ((fd = inotify_init()) < 0) {
    perror ("inotify_init() ");
    exit (EXIT_FAILURE);
  }

  // Add pathname to inotify file descriptor.
  if ((wfd = inotify_add_watch (fd, argv[1], IN_ALL_EVENTS)) < 0) {
    perror ("inotify_add_watch() ");
    exit (EXIT_FAILURE);
  }

  printf ("Watch %s using wfd %d.\n", argv[1], wfd);

  // The read operation will block.
  for (;;) {

    // Read from fd.
    if ((nums = read (fd, buf, sizeof (buf))) <= 0 ) {
      perror ("read() ");
      exit (EXIT_FAILURE);
    }

    printf("Read %d bytes from fd %d.\n", nums, fd);

    // Print each inotify event.
    for (p = buf; p < buf + nums;) {
      event = (struct inotify_event *) p;
      print_event (event);

      p += sizeof(struct inotify_event) + event->len;
    }
  }

  exit (EXIT_SUCCESS);
}

void
print_event (struct inotify_event *event)
{
  printf ("wfd =%2d; ", event->wd);

  if (event->cookie > 0) printf ("cookie =%4d; ", event->cookie);

  printf ("mask = ");
  if (event->mask & IN_ACCESS) printf ("IN_ACCESS ");
  if (event->mask & IN_ATTRIB) printf ("IN_ATTRIB ");
  if (event->mask & IN_CLOSE_NOWRITE) printf ("IN_CLOSE_NOWRITE ");
  if (event->mask & IN_CLOSE_WRITE) printf ("IN_CLOSE_WRITE ");
  if (event->mask & IN_CREATE) printf ("IN_CREATE ");
  if (event->mask & IN_DELETE) printf ("IN_DELETE ");
  if (event->mask & IN_DELETE_SELF) printf ("IN_DELETE_SELF ");
  if (event->mask & IN_IGNORED) printf ("IN_IGNORED ");
  if (event->mask & IN_ISDIR) printf ("IN_ISDIR ");
  if (event->mask & IN_MODIFY) printf ("IN_MODIFY ");
  if (event->mask & IN_MOVE_SELF) printf( "IN_MOVE_SELF ");
  if (event->mask & IN_MOVED_FROM) printf ("IN_MOVED_FROM ");
  if (event->mask & IN_MOVED_TO) printf ("IN_MOVED_TO ");
  if (event->mask & IN_OPEN) printf ("IN_OPEN ");
  if (event->mask & IN_Q_OVERFLOW) printf ("IN_Q_OVERFLOW ");
  if (event->mask & IN_UNMOUNT) printf ("IN_UNMOUNT ");

  printf ("\n");
  if (event->len > 0) printf ("name = %s\n", event->name);
}