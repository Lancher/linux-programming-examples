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

// Get a process's user time, system time and clock click.

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>             // sysconf()
#include <sys/times.h>          // times()
#include <time.h>               // clock()

int
main (int argc, char **argv)
{
  int i, count = 0;
  struct tms t;
  clock_t clock_ticks;
  unsigned long sys_clock_ticks = 0;

  // The value returned by clock() is measured in units of CLOCKS_PER_SEC, so we must divide
  // by this value to arrive at the number of seconds of CPU time used by the process. The CLOCKS_PER_SEC
  // is fixed at 1 million by POSIX.1
  if ((clock_ticks = clock ()) < 0) {
    perror ("clock() ");
    exit (EXIT_FAILURE);
  }
  printf ("clock() - clocks: %ld, seconds: %.2f secs\n", (long) clock_ticks, (double) clock_ticks / CLOCKS_PER_SEC);

  // Get system clock ticks.
  if ((sys_clock_ticks = sysconf (_SC_CLK_TCK)) < 0) {
    perror ("sysconf() ");
    exit (EXIT_FAILURE);
  }
  printf ("System Clock Ticks: %lu\n", sys_clock_ticks);

  // Get user time, system time.
  if (times (&t) < 0) {
    perror ("times() ");
    exit (EXIT_FAILURE);
  }
  printf ("times() - User CPU: %.2f secs, System CPU: %.2f secs\n", (double) t.tms_utime / sys_clock_ticks, (double) t.tms_stime / sys_clock_ticks);

  // Run a loop
  for (i = 0; i < 10000000; i++)
    count += 1;

  // Get ticks again.
  if ((clock_ticks = clock ()) < 0) {
    perror ("clock() ");
    exit (EXIT_FAILURE);
  }
  printf ("clock() - clocks: %ld, seconds: %.2f secs\n", (long) clock_ticks, (double) clock_ticks / CLOCKS_PER_SEC);

  // Get user time, system time.
  if (times (&t) < 0) {
    perror ("times() ");
    exit (EXIT_FAILURE);
  }
  printf ("times() - User CPU: %.2f secs, System CPU: %.2f secs\n", (double) t.tms_utime / sys_clock_ticks, (double) t.tms_stime / sys_clock_ticks);


}