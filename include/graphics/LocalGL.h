#pragma once

#define GL_GLEXT_PROTOTYPES

#ifdef _WIN32
#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#else
#include <GL/glew.h>
#endif
