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

#include <kenv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void
kenv_dump(void) {
  char* buf;
  int size;

  if((size=kenv(KENV_DUMP, 0, 0, 0)) < 0) {
    perror("kenv");
    return;
  }

  if(!(buf=calloc(1, size+1))) {
    perror("calloc");
    return;
  }

  if(kenv(KENV_DUMP, 0, buf, size) < 0) {
    perror("kenv");
    free(buf);
    return;
  }

  // the dump is size bytes of NUL-terminated entries, and an entry can be
  // empty (the PS4 kernel starts with one), so walk it by size
  for(char *p=buf; p<buf+size; p+=strlen(p)+1) {
    if(*p) {
      printf("%s\n", p);
    }
  }

  free(buf);
}


static int
kenv_getint(const char* name) {
  char buf[KENV_MVALLEN+1];

  if(kenv(KENV_GET, name, buf, sizeof(buf)) < 0) {
    perror("kenv");
  }
  buf[KENV_MVALLEN] = 0;

  return atoi(buf);
}


static void
kenv_setint(const char* name, int val) {
  char nbuf[KENV_MNAMELEN+1];
  char vbuf[KENV_MVALLEN+1];

  snprintf(nbuf, KENV_MNAMELEN, "%s", name);
  snprintf(vbuf, KENV_MVALLEN, "%d", val);

  if(kenv(KENV_SET, nbuf, vbuf, KENV_MVALLEN) < 0) {
    perror("kenv");
  }

}

int main(void) {
  int counter = kenv_getint("payload.counter");
  kenv_setint("payload.counter", counter+1);
  kenv_dump();
}
