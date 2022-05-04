//
// Created by mati on 21/04/2022.
//

#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h> // for va_*
#include <limits.h> // for PATH_MAX
#include <string.h> // for strlen
#include <cassert>

#include <errno.h>
#include <limits.h> // for PATH_MAX
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <unistd.h>

#include "debug.hpp"
#include "../iosyscalls-internal.hpp"
#include "../syscalls_real.hpp"

namespace vfs = vfsn::linux::internal;
using fs      = purefs::fs::filesystem;

namespace real
{
    __REAL_DECL(fprintf);
    __REAL_DECL(fwrite);
    __REAL_DECL(fread);
    __REAL_DECL(fopen);
    //__REAL_DECL(fopen64);
    __REAL_DECL(fclose);
    __REAL_DECL(fputc);
    __REAL_DECL(fputs);
    __REAL_DECL(putc);
    __REAL_DECL(fgetc);
    __REAL_DECL(fgets);
    __REAL_DECL(getc);
    __REAL_DECL(freopen);
    __REAL_DECL(fdopen);
    __REAL_DECL(fseek);
    __REAL_DECL(ftell);
    __REAL_DECL(fgetpos);
    //__REAL_DECL(fgetpos64);
    __REAL_DECL(fsetpos);
    //__REAL_DECL(fsetpos64);
    __REAL_DECL(feof);
    __REAL_DECL(rewind);
    __REAL_DECL(fileno);
    __REAL_DECL(ferror);
    __REAL_DECL(fflush);
    __REAL_DECL(remove);
    __REAL_DECL(rename);
} // namespace real

namespace real
{
    __REAL_DECL(link);
    __REAL_DECL(unlink);
    __REAL_DECL(rmdir);
    __REAL_DECL(symlink);

    __REAL_DECL(fcntl);
#if __GLIBC__ > 2 || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 28))
    __REAL_DECL(fcntl64);
#endif
    __REAL_DECL(chdir);
    __REAL_DECL(fchdir);

    __REAL_DECL(getcwd);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    __REAL_DECL(getwd);
#pragma GCC diagnostic pop

    //__REAL_DECL(get_current_dir_name);
    __REAL_DECL(mkdir);

    __REAL_DECL(chmod);
    __REAL_DECL(fchmod);
    __REAL_DECL(fsync);
    //__REAL_DECL(fdatasync);

    __REAL_DECL(stat);
    __REAL_DECL(lstat);
    __REAL_DECL(fstat);

    //__REAL_DECL(stat64);
    //__REAL_DECL(lstat64);
    //__REAL_DECL(fstat64);
    //__REAL_DECL(__xstat);
    //__REAL_DECL(__lxstat);
    //__REAL_DECL(__fxstat);

    //__REAL_DECL(__xstat64);
    //__REAL_DECL(__lxstat64);
    //__REAL_DECL(__fxstat64);

    __REAL_DECL(read);
    __REAL_DECL(write);
    __REAL_DECL(lseek);
    //__REAL_DECL(lseek64);

    __REAL_DECL(mount);
    //__REAL_DECL(umount);

    __REAL_DECL(ioctl);
    __REAL_DECL(poll);
    __REAL_DECL(statvfs);
} // namespace real

extern "C"
{

    FILE *fopen(const char *pathname, const char *mode)
    {
        FILE *ret{};
        char tmp[PATH_MAX];
        const auto path = vfs::npath_translate(pathname, tmp);
        TRACE_SYSCALLN("(%s,%s) -> (%s) linux fs", pathname, mode, path);
        ret = real::fopen(path, mode);
        TRACE_SYSCALLN("(%s,%s)=%p", pathname, mode, ret);
        return ret;
    }

    int lstat(const char *pathname, struct stat *statbuf)
    {
        TRACE_SYSCALLN("(%s) -> linux fs", pathname);
        char tmp[PATH_MAX];
        const auto newpath = vfs::npath_translate(pathname, tmp);
        return real::lstat(newpath, statbuf);
    }
}
