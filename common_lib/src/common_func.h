#ifndef __COMMON_FUNC_H
#define __COMMON_FUNC_H

#define COLOR_NONE          "\e[0m"
#define BLACK               "\e[0;30m"
#define RED                 "\e[0;31m"
#define GREEN               "\e[0;32m"
#define BROWN               "\e[0;33m"
#define YELLOW              "\e[1;33m"
#define BLUE                "\e[0;34m"
#define PURPLE              "\e[0;35m"
#define CYAN                "\e[0;36m"
#define GRAY                "\e[0;37m"
#define WHITE               "\e[1;37m"

typedef enum {
    L_DEBUG,
    L_INFO,
    L_WARN,
    L_ERROR,
    L_PUBLIC,
    L_MAX
}LogLevel;

const static char LOG_COLOR[L_MAX][10] = {
    COLOR_NONE,
    GREEN,
    YELLOW,
    RED,
    COLOR_NONE
};

void FormatPrint(LogLevel level, const char *file, int line, const char *fmt, ...);

#define LOGD(args ...) FormatPrint(L_DEBUG, __FILE__, __LINE__, ##args)
#define LOGI(args ...) FormatPrint(L_INFO, __FILE__, __LINE__, ##args)
#define LOGW(args ...) FormatPrint(L_WARN, __FILE__, __LINE__, ##args)
#define LOGE(args ...) FormatPrint(L_ERROR, __FILE__, __LINE__, ##args)

#endif