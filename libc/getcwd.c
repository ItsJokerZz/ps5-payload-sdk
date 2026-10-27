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

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/syscall.h>


char*
getcwd(char* buf, size_t size) {
  char* tmp = 0;
  int err;

  if(buf) {
    if(!size) {
      errno = EINVAL;
      return 0;
    }

    if(size == 1) {
      errno = ERANGE;
      return 0;
    }
  } else {
    if(!size) {
      size = PATH_MAX;
    }

    if(!(buf=tmp=malloc(size))) {
      return 0;
    }
  }

  if((err=syscall(SYS___getcwd, buf, size))) {
    errno = err == -ENOMEM ? ERANGE : -err;
    free(tmp);
    return 0;
  }

  return buf;
}
