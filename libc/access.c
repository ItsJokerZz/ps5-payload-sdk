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

#include <sys/mount.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>


/**
 * The kernels of the PS4 and the PS5 answer ENOSYS to the eaccess and
 * faccessat system calls, so the access rights are checked here, with the
 * effective ids of the process.
 **/


/**
 * Is the group one of the groups of the process.
 **/
static int
in_group(gid_t gid) {
  gid_t groups[64];
  int n;

  if(gid == getegid()) {
    return 1;
  }

  n = getgroups(64, groups);
  for(int i=0; i<n; i++) {
    if(groups[i] == gid) {
      return 1;
    }
  }

  return 0;
}


int
eaccess(const char *path, int mode) {
  struct statfs fs;
  struct stat st;
  int bits;

  if(!path) {
    errno = EFAULT;
    return -1;
  }
  if(mode & ~(R_OK | W_OK | X_OK)) {
    errno = EINVAL;
    return -1;
  }
  if(stat(path, &st)) {
    return -1;
  }
  if(mode == F_OK) {
    return 0;
  }

  if((mode & W_OK) && (S_ISREG(st.st_mode) || S_ISDIR(st.st_mode) || S_ISLNK(st.st_mode))) {
    if(!statfs(path, &fs) && (fs.f_flags & MNT_RDONLY)) {
      errno = EROFS;
      return -1;
    }
  }

  // root may read and write everything, but only run what has an execute bit
  if(geteuid() == 0) {
    if((mode & X_OK) && !S_ISDIR(st.st_mode) &&
       !(st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
      errno = EACCES;
      return -1;
    }
    return 0;
  }

  if(st.st_uid == geteuid()) {
    bits = (st.st_mode >> 6) & 7;
  } else if(in_group(st.st_gid)) {
    bits = (st.st_mode >> 3) & 7;
  } else {
    bits = st.st_mode & 7;
  }

  if(mode & ~bits) {
    errno = EACCES;
    return -1;
  }

  return 0;
}


/**
 * Enter the directory a descriptor refers to and return a descriptor for the
 * directory to go back to, or -1. There is no system call to do this with, so
 * the working directory is changed while the caller runs, which is not safe
 * when other threads use relative paths at the same time.
 **/
static int
enter_dir(int fd) {
  int cwd = open(".", O_RDONLY);
  int err;

  if(cwd < 0) {
    return -1;
  }
  if(fchdir(fd)) {
    err = errno;
    close(cwd);
    errno = err;
    return -1;
  }

  return cwd;
}


static void
leave_dir(int cwd) {
  int err = errno;

  fchdir(cwd);
  close(cwd);
  errno = err;
}


int
faccessat(int fd, const char *path, int mode, int flag) {
  int cwd;
  int res;

  if(!path) {
    errno = EFAULT;
    return -1;
  }
  if(flag & ~AT_EACCESS) {
    errno = EINVAL;
    return -1;
  }

  if(fd == AT_FDCWD || path[0] == '/') {
    return (flag & AT_EACCESS) ? eaccess(path, mode) : access(path, mode);
  }

  if((cwd=enter_dir(fd)) < 0) {
    return -1;
  }
  res = (flag & AT_EACCESS) ? eaccess(path, mode) : access(path, mode);
  leave_dir(cwd);

  return res;
}
