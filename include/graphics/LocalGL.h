#pragma once

#define GL_GLEXT_PROTOTYPES

#ifdef _WIN32
#include <Windows.h>
#define GLEW_STATIC
#include <GL/glew.h>
#endif
