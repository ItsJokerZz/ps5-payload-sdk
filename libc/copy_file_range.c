/* Copyright (C) 2026 ItsJokerZz

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
#include <sys/types.h>
#include <unistd.h>


/**
 * The kernel predates copy_file_range(), it arrived with FreeBSD 13. libc++
 * calls it on every FreeBSD target and falls back to another way of copying
 * a file when it fails with ENOSYS.
 **/
ssize_t
copy_file_range(int infd, off_t *inoffp, int outfd, off_t *outoffp,
		size_t len, unsigned int flags) {
  errno = ENOSYS;
  return -1;
}
