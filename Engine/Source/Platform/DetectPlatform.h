/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#ifdef _WIN32
#ifdef _WIN64
#define RE_PLATFORM_WINDOWS_64
#else
#define RE_PLATFORM_WINDOWS_32
#endif
#elif defined(__ANDROID__)
#define RE_PLATFORM_ANDROID
#elif defined(__LINUX__)
#define RE_PLATFORM_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE == 1
#define RE_PLATFORM_IOS
#elif TARGET_OS_MAC == 1
#define RE_PLATFORM_MACOS
#else
#error "Unknown Apple platform!"
#endif
#else
#error "Unkown platform!"
#endif