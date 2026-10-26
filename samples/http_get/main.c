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
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int sceNetInit();
int sceNetPoolCreate(const char*, int, int);
int sceNetPoolDestroy(int);

int sceSslInit(size_t);
int sceSslTerm(int);

int sceHttpInit(int, int, size_t);
int sceHttpTerm(int);

int sceHttpCreateTemplate(int, const char*, int, int);
int sceHttpDeleteTemplate(int);

int sceHttpCreateConnectionWithURL(int, const char*, int);
int sceHttpDeleteConnection(int);

int sceHttpCreateRequestWithURL(int, int, const char*, uint64_t);
int sceHttpDeleteRequest(int);

int sceHttpSendRequest(int, const void*, size_t);
int sceHttpGetStatusCode(int, int*);
int sceHttpReadData(int, void *, size_t);


#define SCE_HTTP_VERSION_1_1 2
#define SCE_HTTP_METHOD_GET  0


static int g_libnetMemId  = -1;
static int g_libsslCtxId  = -1;
static int g_libhttpCtxId = -1;
static int g_tmplId       = -1;
static int g_connId       = -1;
static int g_reqId        = -1;


/**
 *
 **/
static int
http_init(const char* agent, const char* url) {
  if(sceNetInit()) {
    perror("sceNetInit");
    return -1;
  }

  if((g_libnetMemId=sceNetPoolCreate("http_get", 32*1024, 0)) < 0) {
    perror("sceNetPoolCreate");
    return -1;
  }

  if((g_libsslCtxId=sceSslInit(256*1024)) < 0) {
    perror("sceSslInit");
    return -1;
  }

  if((g_libhttpCtxId=sceHttpInit(g_libnetMemId, g_libsslCtxId, 256*1024)) < 0) {
    perror("sceHttpInit");
    return -1;
  }

  if((g_tmplId=sceHttpCreateTemplate(g_libhttpCtxId, agent,
				     SCE_HTTP_VERSION_1_1, 1)) < 0) {
    perror("sceHttpCreateTemplate");
    return -1;
  }

  if((g_connId=sceHttpCreateConnectionWithURL(g_tmplId, url, 0)) < 0) {
    perror("sceHttpCreateConnectionWithURL");
    return -1;
  }

  if((g_reqId=sceHttpCreateRequestWithURL(g_connId, SCE_HTTP_METHOD_GET,
					  url, 0)) < 0) {
    perror("sceHttpCreateRequestWithURL");
    return -1;
  }

  return 0;
}


/**
 *
 **/
static int
http_get(void) {
  char buf[0x1000];
  int length = -1;
  int status = -1;

  if(sceHttpSendRequest(g_reqId, NULL, 0)) {
    perror("sceHttpSendRequest");
    return -1;
  }

  if(sceHttpGetStatusCode(g_reqId, &status)) {
    perror("sceHttpGetStatusCode");
    return -1;
  }

  if(status == 200) {
    while((length=sceHttpReadData(g_reqId, buf, sizeof(buf)-1)) > 0) {
      buf[length] = 0;
      printf("%s", buf);
    }
  } else {
    printf("Status: %d\n", status);
  }

  return status;
}


/**
 *
 **/
static void
http_fini(void) {
  if(g_reqId != -1) {
    if(sceHttpDeleteRequest(g_reqId)) {
      perror("sceHttpDeleteRequest");
    }
  }

  if(g_connId != -1) {
    if(sceHttpDeleteConnection(g_connId)) {
      perror("sceHttpDeleteConnection");
    }
  }

  if(g_tmplId != -1) {
    if(sceHttpDeleteTemplate(g_tmplId)) {
      perror("sceHttpDeleteTemplate");
    }
  }

  if(g_libhttpCtxId != -1) {
    if(sceHttpTerm(g_libhttpCtxId)) {
      perror("sceHttpTerm");
    }
  }

  if(g_libsslCtxId != -1) {
    if(sceSslTerm(g_libsslCtxId)) {
      perror("sceSslTerm");
    }
  }

  if(g_libnetMemId != -1) {
    if(sceNetPoolDestroy(g_libnetMemId)) {
      perror("sceNetPoolDestroy");
    }
  }
}


/**
 *
 **/
int
main() {
  int error = 0;

  if(!(error=http_init("http_get/1.0", "http://192.168.1.1"))) {
    error = http_get();
  }

  http_fini();

  return error;
}
