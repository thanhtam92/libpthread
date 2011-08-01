#ifndef _MISC_H_
#define _MISC_H_    1

#include <errno.h>

#include <winsock2.h>

static __inline int set_errno(int result)
{
    if (result != 0) {
        errno = result;
        return -1;
    }
    return 0;
}

/* Number of micro-seconds between the beginning of the Windows epoch
 * (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
 */
#define DELTA_EPOCH_IN_USEC     11644473600000000LL
static __inline __int64 FileTimeToUnixTime(FILETIME *input)
{
    return (((__int64) input->dwHighDateTime) << 32 | input->dwLowDateTime) / 10 - DELTA_EPOCH_IN_USEC;
}

/* Return micro-seconds since the Unix epoch (jan. 1, 1970) UTC */
static __inline __int64 arch_time_in_us(void)
{
    FILETIME time;

    GetSystemTimeAsFileTime(&time);
    return FileTimeToUnixTime(&time);
}

/* Return micro-seconds since the Unix epoch (jan. 1, 1970) UTC */
static __inline void arch_time_in_timespec(struct timespec *ts)
{
    __int64 t;
    FILETIME time;

    GetSystemTimeAsFileTime(&time);
    t = FileTimeToUnixTime(&time);
    ts->tv_sec = t / 1000000;
    ts->tv_nsec= (t % 1000000) * 1000;
}

static __inline __int64 arch_time_in_us_from_timespec(const struct timespec *ts)
{
    return ts->tv_sec * 1000000LL + ts->tv_nsec / 1000;
}

static __inline unsigned arch_rel_time_in_ms(const struct timespec *ts)
{
    __int64 t1 = arch_time_in_us_from_timespec(ts);
    __int64 t2 = arch_time_in_us();
    unsigned t = (unsigned) ((t1 - t2) / 1000);

    if (t1 < t2 || t1 >= t2 + 1000LL * 4294967295)
        return 0;

    return (unsigned) ((t1 - t2) / 1000);
}

#endif