#ifndef INCLUDE_HPP
# define INCLUDE_HPP

#define GLSL_VERSION 330
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <raygui.h>
#include <unordered_map>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>
#include <list>
#include <array>
#include <queue>
#include "haven.hpp"

#define MAX_INPUT 11

typedef enum {
	SOUTH	= 1 << 0,
	NORTH	= 1 << 1,
	EAST	= 1 << 2,
	WEST	= 1 << 3,
}diraction_e;

typedef enum {
	frame_loop_none = 0,
	frame_loop_enable = 1,
	frame_loop_reverse = 2,
	frame_loop_updown = 3,
} frame_loop_e;

Vector2 getBlockPos(Vector2 pos);
const Vector2 getVector2Pos(const unsigned int index, const int width);
void travelTarget(Vector2 *current, const Vector2 target, const float velocity, const double deltaTime);
bool IsInBond(Vector2 pos, Vector2 hi, Vector2 low);
bool IsMouseInBound(Rectangle bound, Vector2 pos, Vector2 mousePos);
int getRenderIdx(const Vector3 pos, const Vector3 level_dim, const Camera2D camera);

#endif
