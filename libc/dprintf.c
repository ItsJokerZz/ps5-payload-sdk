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

#define _WITH_DPRINTF

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int
vdprintf(int fd, const char * __restrict fmt, va_list ap) {
  char stackbuf[0x400];
  char *buf = stackbuf;
  va_list aq;
  long n;
  int len;
  int off = 0;

  va_copy(aq, ap);
  len = vsnprintf(stackbuf, sizeof(stackbuf), fmt, aq);
  va_end(aq);

  if(len < 0) {
    return -1;
  }

  // the output did not fit, format it again into a buffer of the right size
  if((size_t)len >= sizeof(stackbuf)) {
    if(!(buf=malloc(len + 1))) {
      errno = ENOMEM;
      return -1;
    }
    vsnprintf(buf, len + 1, fmt, ap);
  }

  while(off < len) {
    if((n=write(fd, buf + off, len - off)) < 0) {
      if(errno == EINTR) {
        continue;
      }
      len = -1;
      break;
    }
    off += n;
  }

  if(buf != stackbuf) {
    free(buf);
  }

  return len;
}


int
dprintf(int fd, const char * __restrict fmt, ...) {
  va_list ap;
  int len;

  va_start(ap, fmt);
  len = vdprintf(fd, fmt, ap);
  va_end(ap);

  return len;
}
