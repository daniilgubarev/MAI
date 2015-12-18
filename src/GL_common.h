#pragma once

#include <cinttypes>
#include <iostream>

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/intersect.hpp>

#include <SDL2/SDL.h>

#define GL_CHECK() {\
	GLenum err;\
	if ((err = glGetError()) != GL_NO_ERROR)\
		std::cout << __FILE__ << " : " << __LINE__ << " " << (char*)gluErrorString(err) << std::endl;\
	}\

