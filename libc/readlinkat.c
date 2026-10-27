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
#include <fcntl.h>
#include <unistd.h>


/**
 * The kernels of the PS4 and the PS5 answer ENOSYS to the readlinkat system
 * call. A path that is absolute or relative to the working directory is read
 * with readlink. For a descriptor, the working directory is changed while
 * the link is read, which is not safe when other threads use relative paths
 * at the same time.
 **/
ssize_t
readlinkat(int fd, const char *__restrict path, char *__restrict buf, size_t bufsize) {
  ssize_t n;
  int cwd;
  int err;

  if(!path) {
    errno = EFAULT;
    return -1;
  }
  if(fd == AT_FDCWD || path[0] == '/') {
    return readlink(path, buf, bufsize);
  }

  if((cwd=open(".", O_RDONLY)) < 0) {
    return -1;
  }
  if(fchdir(fd)) {
    err = errno;
    close(cwd);
    errno = err;
    return -1;
  }

  n = readlink(path, buf, bufsize);
  err = errno;
  fchdir(cwd);
  close(cwd);
  errno = err;

  return n;
}
