#include "MacSystemTheme.h"
#include <Cocoa/Cocoa.h>

namespace mac {
bool isSystemThemeDark()
{
    NSString* str = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
    return [str caseInsensitiveCompare:@"Dark"] == NSOrderedSame;
    // const char* themeName = [str cStringUsingEncoding:NSUTF8StringEncoding];
    // dark = QString(themeName).endsWith("Dark", Qt::CaseInsensitive);
    // return themeName;
}
}// namespace mac