#include "stdafx.h"
#include "Util.h"
#ifdef _WIN32
#include <Windows.h>
#endif

void nxutil::Sleep(uint32_t timeout)
{
#ifdef _WIN32
    ::Sleep(timeout);
#else
    int was_error;
    struct timespec elapsed, tv;
    elapsed.tv_sec = timeout / 1000;
    elapsed.tv_nsec = (timeout % 1000) * 1000000;
    do {
        errno = 0;
        tv.tv_sec = elapsed.tv_sec;
        tv.tv_nsec = elapsed.tv_nsec;
        was_error = nanosleep(&tv, &elapsed);
    } while (was_error && (errno == EINTR));
#endif
}

