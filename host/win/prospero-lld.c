#include <limits.h>
#include <process.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>


/**
 * Quote an argument for the command line of the child, so that the runtime of
 * the child splits it the same way again.
 **/
static char*
quote(const char* arg) {
    size_t len = strlen(arg);
    char* out;
    char* p;

    if(len && !strpbrk(arg, " \t\n\v\"")) {
        return strdup(arg);
    }

    if(!(out=malloc(len*2 + 3))) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    p = out;
    *p++ = '"';
    for(size_t i=0;; i++) {
        size_t slashes = 0;

        while(arg[i] == '\\') {
            slashes++;
            i++;
        }

        if(arg[i] == '\0') {
            memset(p, '\\', slashes*2);
            p += slashes*2;
            break;
        } else if(arg[i] == '"') {
            memset(p, '\\', slashes*2 + 1);
            p += slashes*2 + 1;
            *p++ = '"';
        } else {
            memset(p, '\\', slashes);
            p += slashes;
            *p++ = arg[i];
        }
    }
    *p++ = '"';
    *p = '\0';

    return out;
}


int
main(int argc, char** argv) {
    char sdkdir[PATH_MAX];
    char ldscript[PATH_MAX*2];
    const char* fixed[] = {
        "ld.lld", "-m", "elf_x86_64", "-pie", "--eh-frame-hdr",
        "-z", "max-page-size=0x4000", "-T", ldscript
    };
    const int nfixed = sizeof(fixed)/sizeof(fixed[0]);
    const char* str;
    char** args;
    intptr_t ret;
    int n = 0;

    if((str=getenv("PS5_PAYLOAD_SDK"))) {
        snprintf(sdkdir, sizeof(sdkdir), "%s", str);
    } else if((str=dirname(argv[0])) && _fullpath(sdkdir, str, sizeof(sdkdir)-4)) {
        strcat(sdkdir, "/..");
    } else {
        fprintf(stderr, "PS5_PAYLOAD_SDK is not set\n");
        return EXIT_FAILURE;
    }

    if((size_t)snprintf(ldscript, sizeof(ldscript), "%s/ldscripts/elf_x86_64.x", sdkdir) >= sizeof(ldscript)) {
        fprintf(stderr, "PS5_PAYLOAD_SDK is too long\n");
        return EXIT_FAILURE;
    }

    if(!(args=calloc(nfixed + argc + 1, sizeof(char*)))) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    for(int i=0; i<nfixed; i++) {
        args[n++] = quote(fixed[i]);
    }
    for(int i=1; i<argc; i++) {
        args[n++] = quote(argv[i]);
    }

    // spawn does not add .exe to a name that has an extension already
    if((ret=_spawnvp(_P_WAIT, "ld.lld.exe", (const char* const*)args)) == -1) {
        perror("ld.lld");
        return EXIT_FAILURE;
    }

    return (int)ret;
}