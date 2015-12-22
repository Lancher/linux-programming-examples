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

// Time functions.

#include <stdio.h>
#include <stdlib.h>

#include <time.h>               // time_t, time(), gmtime(), localtime(), asctime(), ctime() and mktime().

#include <errno.h>              // errno, perror()

int
main (int argc, char **argv)
{
  time_t t;
  struct tm *gm, *loc;
  struct tm gm_cp, loc_cp;

  // Use time() to get number of seconds from Eporch time(include timezone time).
  t = time (NULL);
  printf("The 'time()' get %lds\n\n", (long) t);

  // Use gmtime() to get Broken down time(not include timezone time).
  gm = gmtime (&t);
  if (gm == NULL) {
    perror ("gmtime() ");
    exit (EXIT_FAILURE);
  }
  printf ("The 'gmtime()' Broken down time:\n");
  printf ("year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm->tm_year, gm->tm_mon, gm->tm_mday, gm->tm_hour, gm->tm_min, gm->tm_sec);
  printf ("wday=%d yday=%d isdst=%d\n\n", gm->tm_wday, gm->tm_yday, gm->tm_isdst);

  // Use localtime() to get Broken down time(include timezone time).
  loc = localtime (&t);
  if (loc == NULL) {
    perror ("localtime() ");
    exit (EXIT_FAILURE);
  }
  printf ("The 'localtime()' Broken down time:\n");
  printf ("year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", loc->tm_year, loc->tm_mon, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);
  printf ("wday=%d yday=%d isdst=%d\n", loc->tm_wday, loc->tm_yday, loc->tm_isdst);
  printf ("\n");

  // Use a copy one because 'asctime()' may modify gm or loc.
  printf ("asctime() formats the gmtime() value as:\t\t  %s", asctime (gmtime (&t)));
  printf ("asctime() formats the localtime() value as:\t\t  %s", asctime (localtime (&t)));
  printf ("ctime() formats the time() value as:\t\t\t  %s", ctime (&t));
  printf ("\n");

  // Use a copy one because 'asctime()' may modify gm or loc.
  printf ("mktime() of gmtime() value:\t\t %ld secs\n", (long) mktime (gmtime (&t)));
  printf ("mktime() of localtime() value:\t\t %ld secs\n", (long) mktime (localtime (&t)));
}

