/*
*  Thank you for staying with us.
*  (c)Yume Games 2020 - 2021
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>

#ifndef RBPCH_H
#define RBPCH_H

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "freetype.lib")
#pragma comment (lib, "irrKlang.lib")

#ifdef NDEBUG
#pragma comment (linker, "/ENTRY:mainCRTStartup")
#endif

#endif

#include <Platform/DetectPlatform.h>

#include <spdlog/spdlog.h>