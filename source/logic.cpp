# include <raymath.h>
# include <raylib.h>
# include <cfloat>
# include <iostream>
# include "../HavenLib/include/haven.hpp"

int linearIndexFromCoordinate(Vector3 dim, int max_x, int max_y) {
    int a = 1;
    int b = max_x;
    int c = (max_x) * (max_y);
    int d = 0;
    return (a * dim.x + b * dim.y + c * dim.z + d);
}

Vector3 coordinateFromLinearIndex(int idx, float max_x, float max_y) {
    float x =  idx % (int)(max_x);
    idx /= (max_x);
    float y = idx % (int)(max_y);
    idx /= (max_y);
    float z = idx;
    return ((Vector3){x,y,z});
}

Vector2 getBlockPos(Vector2 pos) {
	Vector2 result;

	modff(pos.x, &result.x);
	modff(pos.y, &result.y);
	result.y += 16;
	result.x -= static_cast<int>(result.x) % 32;
	result.y -= static_cast<int>(result.y) % 32;
	return (result);
}

const Vector2 getVector2Pos(const unsigned int index, const int width) {
	return ((Vector2){getXpos(index, width), getYpos(index, width)});
}

void travelTarget(Vector2 *current, const Vector2 target, const float velocity, const double deltaTime) {
	Vector2 travel = {target.x - current->x, target.y - current->y};
	float distance = Vector2Distance(*current, target);
	float step = velocity * deltaTime;
	if (distance > step * step) {
		float tmp = rsqrt(distance);
		travel.x *= tmp;
		travel.y *= tmp;
		travel.x *= step;
		travel.y *= step;
		current->x += travel.x;
		current->y += travel.y;
		return;
	}
	*current = target;
	return;
}

bool IsInBond(Vector2 pos, Vector2 hi, Vector2 low) {
	return (!(pos.x < low.x || pos.x > hi.x || pos.y < low.y || pos.y > hi.y));
}

bool IsMouseInBound(Rectangle bound, Vector2 pos, Vector2 mousePos) {
	return (mousePos.x >= pos.x && mousePos.x <= pos.x + bound.width
		&& mousePos.y >= pos.y && mousePos.y <= pos.y + bound.height);
}

int getRenderIdx(const Vector3 pos, const Vector3 level_dim, const Camera2D camera) {
	static int idx = 0;
	static int y = 0;
	static int z = 0;

	int ret = -1;

	for (; y < level_dim.y; y++) {
		for (; z < level_dim.z; z++) {
			if (pos.y == y) {
				if (pos.z == z) {
					if (IsInBond({pos.x, pos.z}, GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, camera),  GetScreenToWorld2D({0,0}, camera))) {
						ret = idx;
						idx++;
						return (ret);
					} else {
						return (ret);
					}
				}
			}
		}
	}
	return (ret);
}
