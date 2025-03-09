//
// Created by LMR on 25-3-1.
//

#pragma once
#include <QtGlobal>

namespace utils {
bool isSystemThemeDark();

#if defined(Q_OS_WIN)
bool isWinXOrGreater(unsigned long major_version, unsigned long minor_version, unsigned long build_number);

bool isWin11OrGreater();
#endif
} // namespace utils
