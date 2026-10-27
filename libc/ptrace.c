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
#include <unistd.h>

#include <sys/ptrace.h>
#include <sys/syscall.h>

#include <ps5/kernel.h>


static long
__syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
  unsigned long ret;
  char iserror;

  register long r10 __asm__("r10") = a4;
  register long r8 __asm__("r8") = a5;
  register long r9 __asm__("r9") = a6;

  __asm__ __volatile__(
     "syscall" : "=a"(ret), "=@ccc"(iserror), "+r"(r10), "+r"(r8), "+r"(r9) :
     "a"(n), "D"(a1), "S"(a2), "d"(a3) :
     "rcx", "r11", "memory"
     );

  return iserror ? -ret : ret;
}


int
ptrace(int request, pid_t pid, caddr_t addr, int data) {
  uint64_t authid = kernel_get_ucred_authid(-1);
  pid_t my_pid = getpid();
  long ret;

  kernel_set_ucred_authid(my_pid, 0x4800000000010003l);
  ret = __syscall6(SYS_ptrace, request, pid, (long)addr, data, 0, 0);
  kernel_set_ucred_authid(my_pid, authid);

  if(ret < 0) {
    errno = -ret;
    return -1;
  }

  return ret;
}
