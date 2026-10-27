/* Copyright (C) 2025 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

#include <unistd.h>


/**
 * The kernels of the PS4 and the PS5 answer ENOSYS to the closefrom system
 * call, so close the descriptors one by one.
 **/
void
closefrom(int lowfd) {
  int maxfd = getdtablesize();

  if(lowfd < 0) {
    lowfd = 0;
  }
  if(maxfd <= 0) {
    maxfd = 1024;
  }

  for(int fd=lowfd; fd<maxfd; fd++) {
    close(fd);
  }
}
