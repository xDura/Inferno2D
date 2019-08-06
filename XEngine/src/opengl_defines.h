#ifdef _WIN32 
#include <windows.h>
#endif

#ifndef APIENTRY
#define APIENTRY
#endif

#ifdef IN_IDE_PARSER
# define GL_GLEXT_PROTOTYPES
# include "GL/gl.h"
#else
#include "External/GLAD/include/glad/glad.h"
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>


