#include <filesystem>

#include "bx/platform.h"

#if BX_PLATFORM_WINDOWS
#include <windows.h>
#elif BX_PLATFORM_OSX
#include <mach-o/dyld.h>
#include <climits>
#elif BX_PLATFORM_LINUX
#include <unistd.h>
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX

std::filesystem::path get_exe_file_path() {
#if BX_PLATFORM_WINDOWS
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW( NULL, szPath, MAX_PATH );
#elif BX_PLATFORM_OSX
    char szPath[PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    if (!_NSGetExecutablePath(szPath, &bufsize))
        return std::filesystem::path{szPath}.parent_path() / ""; // to finish the folder path with (back)slash
    return {};  // some error
#elif BX_PLATFORM_LINUX
    char szPath[PATH_MAX];
    ssize_t count = readlink( "/proc/self/exe", szPath, PATH_MAX );
    if( count < 0 || count >= PATH_MAX )
        return {}; // some error
    szPath[count] = '\0';
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX
}
