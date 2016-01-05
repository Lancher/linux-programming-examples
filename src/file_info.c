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

// Retrive all the information of a file.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>              // fcntl()
#include <sys/stat.h>           // struct stat

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  int fd, flags;
  mode_t u;
  struct stat fstat;

  // Clear default umask.
  u = umask (0);

  // create a new file for reading and writing with permission rw-rw-rw
  fd = open ("test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

  // Get the flags
  if ((flags = fcntl (fd, F_GETFL)) < 0) {
    perror ("fcntl() ");
    exit (EXIT_FAILURE);
  }

  // Set with append mode
  flags |= O_APPEND;
  if (fcntl (fd, F_SETFL, flags) < 0) {
    perror ("fcntl() ");
    exit (EXIT_FAILURE);
  }

  // Print all the flags.
  printf ("O_RDONLY: %d\n", (flags &  O_RDONLY) ? 1 : 0);
  printf ("O_WRONLY: %d\n", (flags &  O_WRONLY) ? 1 : 0);
  printf ("O_RDWR: %d\n", (flags &  O_RDWR) ? 1 : 0);
  printf ("---------------\n");
  printf ("O_CLOEXEC: %d\n", (flags &  O_CLOEXEC) ? 1 : 0);
  printf ("O_CREAT: %d\n", (flags &  O_CREAT) ? 1 : 0);
  printf ("O_DIRECT: %d\n", (flags &  O_DIRECT) ? 1 : 0);
  printf ("O_DIRECTORY: %d\n", (flags &  O_DIRECTORY) ? 1 : 0);
  printf ("O_EXCL: %d\n", (flags &  O_EXCL) ? 1 : 0);
  printf ("O_LARGEFILE: %d\n", (flags &  O_LARGEFILE) ? 1 : 0);
  printf ("O_NOATIME: %d\n", (flags &  O_NOATIME) ? 1 : 0);
  printf ("O_NOCTTY: %d\n", (flags &  O_NOCTTY) ? 1 : 0);
  printf ("O_NOFOLLOW: %d\n", (flags &  O_NOFOLLOW) ? 1 : 0);
  printf ("O_TRUNC: %d\n", (flags &  O_TRUNC) ? 1 : 0);
  printf ("---------------\n");
  printf ("O_APPEND: %d\n", (flags &  O_APPEND) ? 1 : 0);
  printf ("O_ASYNC: %d\n", (flags &  O_ASYNC) ? 1 : 0);
  printf ("O_DSYNC: %d\n", (flags &  O_DSYNC) ? 1 : 0);
  printf ("O_NONBLOCK: %d\n", (flags &  O_NONBLOCK) ? 1 : 0);
  printf ("O_SYNC: %d\n", (flags &  O_SYNC) ? 1 : 0);
  printf ("---------------\n");

  close (fd);

  // Get file stats.
  if (stat ("test.txt", &fstat) < 0) {
    perror ("stat() ");
    exit (EXIT_FAILURE);
  }

  printf ("Information\n");
  printf ("---------------------------\n");
  printf ("File Size: \t\t%d bytes\n", (int) fstat.st_size);
  printf ("Number of Links: \t%d\n", (int) fstat.st_nlink);
  printf ("File inode: \t\t%d\n", (int) fstat.st_ino);

  printf ("File Permissions: \t");
  printf ((S_ISDIR (fstat.st_mode)) ? "d" : "-");
  printf ((fstat.st_mode & S_IRUSR) ? "r" : "-");
  printf ((fstat.st_mode & S_IWUSR) ? "w" : "-");
  printf ((fstat.st_mode & S_IXUSR) ? "x" : "-");
  printf ((fstat.st_mode & S_IRGRP) ? "r" : "-");
  printf ((fstat.st_mode & S_IWGRP) ? "w" : "-");
  printf ((fstat.st_mode & S_IXGRP) ? "x" : "-");
  printf ((fstat.st_mode & S_IROTH) ? "r" : "-");
  printf ((fstat.st_mode & S_IWOTH) ? "w" : "-");
  printf ((fstat.st_mode & S_IXOTH) ? "x" : "-");
  printf ("\n\n");

  printf("The file %s a symbolic link\n", (S_ISLNK (fstat.st_mode)) ? "is" : "is not");

}
