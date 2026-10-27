/* Copyright (C) 2024 John Törnblom

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
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <sched.h>
#include <semaphore.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/ptrace.h>
#include <sys/ktrace.h>
#include <sys/sysctl.h>
#include <sys/event.h>
#include <sys/ucontext.h>
#include <sys/jail.h>
#include <sys/acl.h>
#include <sys/extattr.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <aio.h>
#include <sys/cpuset.h>
#include <sys/rtprio.h>
#include <sys/thr.h>
#include <sys/umtx.h>
#include <sys/module.h>
#include <sys/linker.h>
#include <sys/procctl.h>
#include <sys/mac.h>
#include <sys/loginclass.h>
#include <netinet/sctp.h>
#include <ufs/ufs/quota.h>
#include <sys/kenv.h>
#include <sys/select.h>
#include <sys/socketvar.h>
#include <sys/syscall.h>
#include <sys/param.h>
#include <sys/cpuset.h>
#include <sys/_cpuset.h>
#include <sys/rctl.h>
#include <sys/timex.h>
#include <sys/ipc.h>
#include <sys/utsname.h>
#include <sys/reboot.h>
#include <sys/_types.h>
#include <pthread_np.h>
#include <sys/random.h>
#include <sys/_umtx.h>
#include <sys/user.h>
#include <machine/sysarch.h>
#include <sys/auxv.h>
#include <bsm/audit.h>
#include <sys/vmmeter.h>


/**
 * Set errno from the error number a failed system call left in eax and return
 * -1, like the libc wrappers of FreeBSD do.
 **/
long
__syscall_set_errno(int err) {
  errno = err;
  return -1;
}


/**
 * Jumped to by a wrapper when the kernel set the carry flag.
 **/
void
__syscall_cerror() {
  asm(".intel_syntax noprefix\n"
    "  push rbp\n"
    "  mov rbp, rsp\n"
    "  mov edi, eax\n"
    "  call __syscall_set_errno\n"
    "  pop rbp\n"
    "  ret\n"
    );
}


/**
 * Return the negated error number when the system call fails, like on the PS4.
 **/
__attribute__((naked)) int
syscall(int a0, ...) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, rdi\n"
    "  mov rdi, rsi\n"
    "  mov rsi, rdx\n"
    "  mov rdx, rcx\n"
    "  mov r10, r8\n"
    "  mov r8,  r9\n"
    "  mov r9,  qword ptr [rsp + 8]\n"
    "  syscall\n"
    "  jnc 1f\n"
    "  neg rax\n"
    "1:\n"
    "  ret\n"
    );
}

__attribute__((naked)) off_t
__syscall(quad_t a0, ...) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, rdi\n"
    "  mov rdi, rsi\n"
    "  mov rsi, rdx\n"
    "  mov rdx, rcx\n"
    "  mov r10, r8\n"
    "  mov r8,  r9\n"
    "  mov r9,  qword ptr [rsp + 8]\n"
    "  syscall\n"
    "  jnc 1f\n"
    "  neg rax\n"
    "1:\n"
    "  ret\n"
    );
}

__attribute__((naked)) pid_t
fork(void) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 2\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) pid_t
wait4(pid_t a0, int *a1, int a2, struct rusage *a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 7\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
link(const char *a0, const char *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 9\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fchdir(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 13\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
mknod(const char *a0, mode_t a1, dev_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 14\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
chown(const char *a0, uid_t a1, gid_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 16\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
dup(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 41\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
ktrace(const char *a0, int a1, int a2, pid_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 45\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setlogin(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 50\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
acct(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 51\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
reboot(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 55\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
revoke(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 56\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
symlink(const char *restrict a0, const char *restrict a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 57\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
readlink(const char *restrict a0, char *restrict a1, size_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 58\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) mode_t
umask(mode_t a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 60\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
chroot(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 61\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) pid_t
vfork(void) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 66\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) void *
sbrk(intptr_t a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 69\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
sstk() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 70\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
vadvise() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 72\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) pid_t
getpgrp(void) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 81\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setpgid(pid_t a0, pid_t a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 82\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
swapon(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 85\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fchown(int a0, uid_t a1, gid_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 123\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
mkfifo(const char *a0, mode_t a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 132\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
adjtime(const struct timeval *a0, struct timeval *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 140\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) pid_t
setsid(void) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 147\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
quotactl(const char *a0, int a1, int a2, void *a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 148\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
nlm_syscall(int a0, int a1, int a2, char **a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 154\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
nfssvc(int a0, void *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 155\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
lgetfh(const char *a0, fhandle_t *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 160\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getfh(const char *a0, fhandle_t *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 161\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
rtprio(int a0, pid_t a1, struct rtprio *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 166\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
semsys(int a0, ...) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 169\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
msgsys(int a0, ...) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 170\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
shmsys(int a0, ...) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 171\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setfib(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 175\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
ntp_adjtime(struct timex *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 176\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setgid(gid_t a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 181\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) long
pathconf(const char *a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 191\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) long
fpathconf(int a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 192\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
undelete(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 205\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getpgid(pid_t a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 207\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
semget(key_t a0, int a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 221\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
semop(int a0, struct sembuf *a1, size_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 222\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
msgget(key_t a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 225\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
msgsnd(int a0, const void *a1, size_t a2, int a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 226\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
msgrcv(int a0, void *a1, size_t a2, long a3, int a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 227\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) void *
shmat(int a0, const void *a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 228\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
shmdt(const void *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 230\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
shmget(key_t a0, size_t a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 231\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
ffclock_getcounter() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 241\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
ffclock_setestimate() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 242\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
ffclock_getestimate() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 243\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
clock_nanosleep(clockid_t a0, int a1, const struct timespec *a2, struct timespec *a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 244\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
clock_getcpuclockid2(id_t a0, int a1, clockid_t *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 247\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
ntp_gettime(struct ntptimeval *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 248\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
minherit(void *a0, size_t a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 250\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) pid_t
rfork(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 251\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
lchown(const char *a0, uid_t a1, gid_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 254\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
aio_read(struct aiocb *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 255\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
aio_write(struct aiocb *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 256\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
lio_listio(int a0, struct aiocb *const restrict *restrict a1, int a2, struct sigevent *a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 257\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
lchmod(const char *a0, mode_t a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 274\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
lutimes(const char *a0, const struct timeval *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 276\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
nstat() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 278\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
nfstat() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 279\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
nlstat() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 280\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fhopen(const struct fhandle *a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 298\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fhstat(const struct fhandle *a0, struct stat *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 299\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
modnext(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 300\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
modstat(int a0, struct module_stat *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 301\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
modfnext(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 302\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
modfind(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 303\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldload(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 304\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldunload(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 305\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldfind(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 306\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldnext(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 307\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldstat(int a0, struct kld_file_stat *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 308\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldfirstmod(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 309\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setresuid(uid_t a0, uid_t a1, uid_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 311\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setresgid(gid_t a0, gid_t a1, gid_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 312\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
aio_return(struct aiocb *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 314\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
aio_suspend(const struct aiocb *const *a0, int a1, const struct timespec *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 315\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
aio_cancel(int a0, struct aiocb *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 316\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
aio_error(const struct aiocb *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 317\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
yield() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 321\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
utrace(const void *a0, size_t a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 335\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldsym(int a0, int a1, void *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 337\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
jail(struct jail *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 338\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
nnpfs_syscall() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 339\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_get_file() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 347\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_set_file() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 348\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_get_fd() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 349\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_set_fd() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 350\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_delete_file() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 351\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_delete_fd() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 352\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_aclcheck_file() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 353\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_aclcheck_fd() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 354\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
extattrctl(const char *a0, int a1, const char *a2, int a3, const char *a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 355\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_set_file(const char *a0, int a1, const char *a2, const void *a3, size_t a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 356\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_get_file(const char *a0, int a1, const char *a2, void *a3, size_t a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 357\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
extattr_delete_file(const char *a0, int a1, const char *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 358\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
aio_waitcomplete(struct aiocb **a0, struct timespec *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 359\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getresuid(uid_t *a0, uid_t *a1, uid_t *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 360\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getresgid(gid_t *a0, gid_t *a1, gid_t *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 361\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_set_fd(int a0, int a1, const char *a2, const void *a3, size_t a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 371\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_get_fd(int a0, int a1, const char *a2, void *a3, size_t a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 372\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
extattr_delete_fd(int a0, int a1, const char *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 373\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__setugid() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 374\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
afs3_syscall() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 377\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}


void
__mac_get_proc() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 384\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_set_proc() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 385\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_get_fd() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 386\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_get_file() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 387\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_set_fd() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 388\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_set_file() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 389\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
kenv() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 390\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
lchflags(const char *a0, unsigned long a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 391\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
mac_syscall(const char *a0, int a1, void *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 394\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getfsstat(struct statfs *a0, long a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 395\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
statfs(const char *a0, struct statfs *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 396\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fstatfs(int a0, struct statfs *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 397\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fhstatfs(const struct fhandle *a0, struct statfs *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 398\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_get_pid() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 409\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_get_link() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 410\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_set_link() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 411\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_set_link(const char *a0, int a1, const char *a2, const void *a3, size_t a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 412\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_get_link(const char *a0, int a1, const char *a2, void *a3, size_t a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 413\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
extattr_delete_link(const char *a0, int a1, const char *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 414\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__mac_execve() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 415\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
swapoff(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 424\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_get_link() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 425\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_set_link() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 426\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_delete_link() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 427\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__acl_aclcheck_link() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 428\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_create(ucontext_t *a0, long *a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 430\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) void
thr_exit(long *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 431\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_self(long *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 432\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_kill(long a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 433\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
jail_attach(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 436\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_list_fd(int a0, int a1, void *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 437\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_list_file(const char *a0, int a1, void *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 438\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) ssize_t
extattr_list_link(const char *a0, int a1, void *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 439\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_suspend(const struct timespec *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 442\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_wake(long a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 443\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
kldunloadf(int a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 444\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
audit(const void *a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 445\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
auditon(int a0, void *a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 446\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getauid(au_id_t *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 447\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setauid(const au_id_t *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 448\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getaudit(struct auditinfo *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 449\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setaudit(const struct auditinfo *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 450\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getaudit_addr(struct auditinfo_addr *a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 451\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setaudit_addr(const struct auditinfo_addr *a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 452\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
auditctl(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 453\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_new(struct thr_param *a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 455\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
kmq_open() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 457\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
kmq_setattr() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 458\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
kmq_timedreceive() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 459\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
kmq_timedsend() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 460\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
kmq_notify() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 461\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
kmq_unlink() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 462\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) void
abort2(const char *a0, int a1, void **a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 463\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_set_name(long a0, const char *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 464\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
aio_fsync(int a0, struct aiocb *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 465\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
sctp_peeloff(int a0, sctp_assoc_t a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 471\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
sctp_generic_sendmsg() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 472\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
sctp_generic_sendmsg_iov() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 473\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
sctp_generic_recvmsg() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 474\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
thr_kill2(pid_t a0, long a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 481\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
cpuset(cpusetid_t *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 484\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
cpuset_setid(cpuwhich_t a0, id_t a1, cpusetid_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 485\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fchmodat(int a0, const char *a1, mode_t a2, int a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 490\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fchownat(int a0, const char *a1, uid_t a2, gid_t a3, int a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 491\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fexecve(int a0, char *const *a1, char *const *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 492\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
fstatat(int a0, const char *a1, struct stat *a2, int a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 493\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
futimesat(int a0, const char *a1, const struct timeval *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 494\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
linkat(int a0, const char *a1, int a2, const char *a3, int a4) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 495\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
mkdirat(int a0, const char *a1, mode_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 496\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
mkfifoat(int a0, const char *a1, mode_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 497\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
mknodat(int a0, const char *a1, mode_t a2, dev_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 498\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
openat(int a0, const char *a1, int a2, ...) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 499\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
renameat(int a0, const char *a1, int a2, const char *a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 501\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
symlinkat(const char *a0, int a1, const char *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 502\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
unlinkat(int a0, const char *a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 503\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
posix_openpt(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 504\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
gssd_syscall() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 505\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
jail_get(struct iovec *a0, unsigned int a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 506\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
jail_set(struct iovec *a0, unsigned int a1, int a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 507\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
jail_remove(int a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 508\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
semctl(int a0, int a1, int a2, ...) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 510\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
msgctl(int a0, int a1, struct msqid_ds *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 511\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
shmctl(int a0, int a1, struct shmid_ds *a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 512\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) long
lpathconf(const char *a0, int a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 513\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
__cap_rights_get() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 515\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
cap_enter() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 516\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
cap_getmode() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 517\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
pdfork() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 518\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
pdkill() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 519\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
pdgetpid() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 520\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
getloginclass(char *a0, size_t a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 523\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
setloginclass(const char *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 524\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
rctl_get_racct(const char *a0, size_t a1, char *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 525\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
rctl_get_rules(const char *a0, size_t a1, char *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 526\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
rctl_get_limits(const char *a0, size_t a1, char *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 527\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
rctl_add_rule(const char *a0, size_t a1, char *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 528\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
rctl_remove_rule(const char *a0, size_t a1, char *a2, size_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 529\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
posix_fallocate(int a0, off_t a1, off_t a2) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 530\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
posix_fadvise(int a0, off_t a1, off_t a2, int a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 531\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) pid_t
wait6(idtype_t a0, id_t a1, int *a2, int a3, struct __wrusage *a4, struct __siginfo *a5) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 532\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
cap_rights_limit() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 533\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
cap_ioctls_limit() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 534\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
cap_ioctls_get() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 535\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
cap_fcntls_limit() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 536\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
cap_fcntls_get() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 537\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
bindat(int a0, int a1, const struct sockaddr *a2, socklen_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 538\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
connectat(int a0, int a1, const struct sockaddr *a2, socklen_t a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 539\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
chflagsat(int a0, const char *a1, unsigned long a2, int a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 540\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
aio_mlock(struct aiocb *a0) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 543\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
procctl(idtype_t a0, id_t a1, int a2, void *a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 544\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
ppoll(struct pollfd *a0, nfds_t a1, const struct timespec *restrict a2, const sigset_t *restrict a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 545\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
futimens(int a0, const struct timespec *a1) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 546\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

__attribute__((naked)) int
utimensat(int a0, const char *a1, const struct timespec *a2, int a3) {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 547\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
numa_getaffinity() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 548\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

void
numa_setaffinity() {
  asm(".intel_syntax noprefix\n"
    "  mov rax, 549\n"
    "  mov r10, rcx\n"
    "  syscall\n"
    "  jc __syscall_cerror\n"
    "  ret\n"
    );
}

