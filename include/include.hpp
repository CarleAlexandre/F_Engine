#ifndef INCLUDE_HPP
# define INCLUDE_HPP

#include <raymath.h>
#include <raylib.h>

#define MAX_INPUT 11

typedef enum {
	SOUTH	= 1 << 0,
	NORTH	= 1 << 1,
	EAST	= 1 << 2,
	WEST	= 1 << 3,
}diraction_e;

int linearIndexFromCoordinate(Vector3 dim, int max_x, int max_y);
Vector3 coordinateFromLinearIndex(int idx, float max_x, float max_y);
Vector2 getBlockPos(Vector2 pos);
const Vector2 getVector2Pos(const unsigned int index, const int width);
void travelTarget(Vector2 *current, const Vector2 target, const float velocity, const double deltaTime);
bool IsInBond(Vector2 pos, Vector2 hi, Vector2 low);
bool IsMouseInBound(Rectangle bound, Vector2 pos, Vector2 mousePos);
int getRenderIdx(const Vector3 pos, const Vector3 level_dim, const Camera2D camera);

#endif