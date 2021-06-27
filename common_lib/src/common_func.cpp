#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include "common_func.h"

void FormatPrint(LogLevel level, const char *file, int line, const char *fmt, ...)
{
    if (level < L_DEBUG) {
        return;
    }

    char buf[1024];

    va_list ag;
    va_start(ag, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ag);
    va_end(ag);

    const char *file_s, *file_p;
    file_s = file_p = file;
    while (*file_s) {
        if (*file_s == '\\' || *file_s == '/')
            file_p = file_s + 1;
        file_s++;
    }

    printf("%s", LOG_COLOR[level]);
    printf("[%s:%d] %s",file_p, line, buf);
    printf(COLOR_NONE);
    printf("\n");
}