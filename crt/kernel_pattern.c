/* Copyright (C) 2025 John Törnblom
   Copyright (C) 2026 ItsJokerZz

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

#include "kernel.h"


/**
 * Number of kernel bytes copied out per step when scanning for a pattern.
 **/
#define SCAN_CHUNK 0x1000


/**
 * Longest pattern, in characters, kernel_find_pattern() accepts.
 **/
#define PATTERN_MAX 0x100


/**
 * Compare two memory regions.
 **/
static int
memcmp(const unsigned char* dst, const unsigned char* src, unsigned long len) {
  for(unsigned long i=0; i<len; i++) {
    if(dst[i] != src[i]) {
      return src[i] - dst[i];
    }
  }

  return 0;
}


/**
 * Get the length of a string.
 **/
static unsigned long
strlen(const char *str) {
  const char *start = str;

  while(*str) {
    str++;
  }

  return str - start;
}


/**
 * Convert a hexchar to a nibble (half byte).
 **/
static short
hex2nib(char hexchar) {
  if(hexchar >= '0' && hexchar <= '9') {
    return hexchar - '0';
  }

  if(hexchar >= 'A' && hexchar <= 'F') {
    return hexchar - 'A' + 10;
  }

  if(hexchar >= 'a' && hexchar <= 'f') {
    return hexchar - 'a' + 10;
  }

  return -1;
}


/**
 * Check if a byte value matches the first two characters of a hexstring,
 * where '?' matches any nibble.
 **/
static short
byte_match(unsigned char bval, const char* hexstr) {
  short nib;

  if(!hexstr[0] || hexstr[0] == '?') {
    return 1;
  }

  if((nib=hex2nib(hexstr[0])) < 0) {
    return -1;
  }

  if(nib != (bval >> 4)) {
    return -1;
  }

  if(!hexstr[1] || hexstr[1] == '?') {
    return 1;
  }

  if((nib=hex2nib(hexstr[1])) < 0) {
    return -1;
  }

  if(nib != (bval & 0xf)) {
    return -1;
  }

  return 1;
}


/**
 * Check if a memory region starts with the bytes described by a hexstring.
 * A region that ends before the pattern does is not a match.
 **/
static int
pattern_match(const unsigned char* addr, unsigned int len, const char* hexstr) {
  while(len && *hexstr) {
    if(byte_match(*addr, hexstr) < 0) {
      return -1;
    }

    hexstr++;
    addr++;
    len--;

    if(*hexstr) {
      hexstr++;
    }
  }

  if(*hexstr) {
    return -1;
  }

  return 1;
}


unsigned long
kernel_find_pattern(unsigned long addr, unsigned int len, const char* pattern) {
  unsigned char buf[SCAN_CHUNK + PATTERN_MAX/2];
  unsigned long pattern_len;
  unsigned long pattern_bytes;
  unsigned int chunk;
  unsigned int size;

  if(!addr || !len || !pattern) {
    return 0;
  }

  pattern_len = strlen(pattern);
  if(!pattern_len || pattern_len > PATTERN_MAX) {
    return 0;
  }
  pattern_bytes = (pattern_len + 1) / 2;

  // copy the kernel out in chunks that overlap by the length of the pattern
  // so that a match crossing a chunk boundary is found
  for(unsigned int off=0; off<len; off+=SCAN_CHUNK) {
    chunk = len - off < SCAN_CHUNK ? len - off : SCAN_CHUNK;
    size = chunk + pattern_bytes - 1;
    if(size > len - off) {
      size = len - off;
    }

    if(kernel_copyout(addr + off, buf, size)) {
      return 0;
    }

    for(unsigned int i=0; i<chunk; i++) {
      if(pattern_match(buf + i, size - i, pattern) > 0) {
	return addr + off + i;
      }
    }
  }

  return 0;
}


int
kernel_patch(unsigned long addr, const void* before, const void* after,
	     unsigned short size) {
  unsigned char current[size];

  // obtain current pattern
  if((kernel_copyout(addr, current, size))) {
    return -1;
  }

  // already patched
  if(!memcmp(current, after, size)) {
    return 0;
  }

  // before does not match current, wrong offset?
  if(before && memcmp(before, current, size)) {
    return -1;
  }

  // apply patch
  if((kernel_copyin(after, addr, size))) {
    return -1;
  }

  return 0;
}
