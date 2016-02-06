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

// Fetch users and groups info in `/etc/passwd` and `/etc/group`.

#include <stdio.h>
#include <stdlib.h>

#include <pwd.h>                // struct passwd, getpwnam()
#include <grp.h>                // struct group, getgrnam()

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  char **p;
  struct passwd *pwd;
  struct group *grp;

  // Usage.
  if (argc < 2) {
    printf ("Usage: %s <name>\n", argv[0]);
    exit (EXIT_SUCCESS);
  }

  // Get user info by name.
  if ((pwd = getpwnam (argv[1])) == NULL ) {
    perror ("getgrnam() not found ");
  }
  else {
    // Print user info.
    printf("Name: %s\n", pwd->pw_name);
    printf("Password: %s\n", pwd->pw_passwd);
    printf("UID: %d\n", pwd->pw_uid);
    printf("GID: %d\n", pwd->pw_gid);
    printf("Comment: %s\n", pwd->pw_gecos);
    printf("Home folder: : %s\n", pwd->pw_dir);
    printf("Shell: %s\n", pwd->pw_shell);
  }

  // Get group info by name.
  if ((grp = getgrnam (argv[1])) == NULL ) {
    perror ("getgrnam() not found ");
  }
  else {
    // Print group info.
    printf("Group Name: %s\n", grp->gr_name);
    printf("Group Password: %s\n", grp->gr_passwd);
    printf("Group ID: %d\n", grp->gr_gid);
    for (p = grp->gr_mem; *p != NULL; p++)
      printf("Group Member: %s\n", *p);
  }

  exit (EXIT_SUCCESS);
}
