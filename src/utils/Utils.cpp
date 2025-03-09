//
// Created by LMR on 25-3-1.
//

#include "Utils.h"

#if defined(Q_OS_WIN)
#include <QLibrary>
#include <windows.h>
#elif defined(Q_OS_LINUX)
// #include <gtk/gtk.h>
#elif defined(Q_OS_MAC)
#include "MacSystemTheme.h"
#endif

namespace utils {
bool isSystemThemeDark()
{
    bool dark = false;
#if defined(Q_OS_WIN)
    typedef LONG(WINAPI * tRegGetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD, LPDWORD, PVOID, LPDWORD);
    auto pRegGetValueW = tRegGetValueW(QLibrary::resolve("advapi32", "RegGetValueW"));

    if (pRegGetValueW) {
        DWORD value;
        DWORD size = sizeof(value);
        if (pRegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                          L"AppsUseLightTheme", RRF_RT_DWORD, nullptr, &value, &size) == ERROR_SUCCESS)
            dark = (value == 0);
    }
#elif defined(Q_OS_LINUX)
    // note: it's needs gtk lib, support on the Linux in the future
    // GtkSettings* settings = gtk_settings_get_default();
    // gchar* theme_name;
    // g_object_get(settings, "gtk-theme-name", &theme_name, nullptr);
    // dark = QString(theme_name).endsWith("Dark", Qt::CaseInsensitive);
#elif defined(Q_OS_MAC)
    dark = mac::isSystemThemeDark();
#endif
    return dark;
}

#if defined(Q_OS_WIN)
bool isWinXOrGreater(DWORD major_version, DWORD minor_version, DWORD build_number)
{
    bool is_win_x_or_greater = false;

    typedef NTSTATUS(WINAPI * tRtlGetVersion)(LPOSVERSIONINFOEXW);
    tRtlGetVersion pRtlGetVersion = tRtlGetVersion(QLibrary::resolve("ntdll", "RtlGetVersion"));

    if (pRtlGetVersion) {
        OSVERSIONINFOEXW os_info;
        memset(&os_info, 0, sizeof(OSVERSIONINFOEXW));
        os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
        auto status = pRtlGetVersion(&os_info);
        if (status == 0) {
            is_win_x_or_greater = (os_info.dwMajorVersion >= major_version && os_info.dwMinorVersion >= minor_version &&
                                   os_info.dwBuildNumber >= build_number);
        }
    }

    return is_win_x_or_greater;
}

bool isWin11OrGreater()
{
    bool is_win_11_or_greater = isWinXOrGreater(10, 0, 22000);

    return is_win_11_or_greater;
}
#endif
}// namespace utils