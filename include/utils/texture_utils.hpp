#if !defined(TEXTURE_UTILS_HPP)
#define TEXTURE_UTILS_HPP

#include <cstdio>
#include <cstdlib>

#include <external/stb_image.hpp>
#include <external/glad/glad.h>


GLuint LoadTextureImage(const char *filename);

#endif // TEXTURE_UTILS_HPP
