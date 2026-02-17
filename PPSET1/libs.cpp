#include "libs.h"

int DEBUG = 0;

int debug_printf(const char *format, ...) {
    if (DEBUG == 1) {
        va_list args;
        va_start(args, format);
        int result = vprintf(format, args);
        va_end(args);
        return result;
    }
    return 0;
}