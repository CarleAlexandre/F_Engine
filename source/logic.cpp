# include <engine.hpp>
# include <cfloat>
# include <iostream>

Vector2 getBlockPos(Vector2 pos) {
	Vector2 result;

	modff(pos.x, &result.x);
	modff(pos.y, &result.y);
	result.y += 16;
	result.x -= static_cast<int>(result.x) % 32;
	result.y -= static_cast<int>(result.y) % 32;
	return (result);
}

const float getXpos(const u32 idx, const int width) {
	return (idx % width);
}

const float getYpos(const u32 idx, const int width) {
	return ((float)idx / width);
}
const u32 getLinearIndex(const float x, const float y, const int width) {
	return (x + y * width);
}

const Vector2 getVector2Pos(const u32 index, const int width) {
	return ((Vector2){getXpos(index, width), getYpos(index, width)});
}

bool isWall(Vector2 pos, const t_level &level) {
	Vector2 tmp = getBlockPos(pos);
	tmp.x /= 32;
	tmp.y /= 32;
	//static double time = 0;
	//time += GetFrameTime();
	//if (time >= 3) {
	//	std::cout << tmp.x << ", " << tmp.y << ". getblockposcoord ! \n";
	//	std::cout << pos.x << ", " << pos.y << ". current pos ! \n";
	//	std::cout << engine.level.map[tmp.x + tmp.y * engine.level.width].pos.x << ", " << engine.level.map[tmp.x + tmp.y * engine.level.width].pos.y << "\n";
	//	time = 0;
	//}
	if (tmp.x > level.dimension.x || tmp.y > level.dimension.y || tmp.x < 0 || tmp.y < 0) {
		std::cerr << "Error, Coordinate is OutOfBound!!\n";
		return (true);
	}
	if (level.wall[(int)(tmp.x + tmp.y * level.dimension.x)]) {
		return (true);
	}
	return (false);
}

Rectangle GetNearestWallBound(Vector2 pos, const t_level &level, Rectangle *bound) {
	float minDist1 = FLT_MAX;
    float minDist2 = FLT_MAX;

	bound[0] = {0};
	bound[1] = {0};

	int idx = (pos.x / 32 - 1) + (pos.y / 32 - 1) * level.dimension.x;
	int hi = (pos.x / 32 + 1) + (pos.y / 32 + 1) * level.dimension.x;
	for (int i = idx; i < hi; i++) {
		if (level.wall[i]) {
			Vector2 tmp = {i / level.dimension.x * 32, (float)(i % (int)level.dimension.y) * 32};
			float dist = Vector2Distance(pos, tmp);
			if (dist < minDist1 && level.wall[i]) {
				minDist2 = minDist1;
				bound[1] = bound[0];
				minDist1 = dist;
				bound[0] = {tmp.x, tmp.y, 32, 32};
			} else if (dist < minDist2) {
				minDist2 = dist;
				bound[1] = {tmp.x, tmp.y, 32, 32};
			}
		}
	}
	return (bound[1]);
}

void CorrectWallCollision(t_player *player, const t_level &level, Rectangle *collision) {
	double deltaTime = GetFrameTime();
	Rectangle wall[2];
	collision[0] = (Rectangle){0, 0, 0, 0};
	collision[1] = (Rectangle){0, 0, 0, 0};

	GetNearestWallBound({player->pos.x, player->pos.y}, level, wall);
	if (CheckCollisionRecs(player->hitbox, wall[0]) || CheckCollisionRecs(player->hitbox, wall[1])) {
		if (NORTH & player->dir) {
			player->pos.y += player->stats.move_speed * deltaTime;
		} else if (SOUTH & player->dir) {
			player->pos.y -= player->stats.move_speed * deltaTime;
		}
		if (EAST & player->dir) {
			player->pos.x -= player->stats.move_speed * deltaTime;
		} else if (WEST & player->dir) {
			player->pos.x += player->stats.move_speed * deltaTime;
		}
		player->hitbox.x = player->pos.x - player->hitbox.width * 0.5;
		player->hitbox.y = player->pos.y + player->hitbox.height * 0.5;
		collision[0] = GetCollisionRec(player->hitbox, wall[0]);
		collision[1] = GetCollisionRec(player->hitbox, wall[1]);
	}
}

//GetNearestEntities(){}

//void SpellCollision(t_engine &engine, Rectangle *collision) {
//	double deltaTime = GetFrameTime();
//	Rectangle wall[2];
//	*collision = (Rectangle){0, 0, 0, 0};

//	GetNearestEntities(engine.entities, engine.level.map, wall);
//		//*collision = wall;
//	if (CheckCollisionRecs(player.hitbox, wall[0]) || CheckCollisionRecs(player.hitbox, wall[1])) {
//		collision[0] = GetCollisionRec(player.hitbox, wall[0]);
//		collision[1] = GetCollisionRec(player.hitbox, wall[1]);


//	}
//}
//check spell radius, if entity is in it, it take damage
//do another function for the auto attacks where i check if entities hitbox is under mouse if yes then launch autoattacks

i32 gcd(i32 a, i32 b) {
	while (1) {
		if (b == 0) {
			break;
		}
		a ^= b;
		b ^= a;
		a ^= b;
		b %= a;
	}
	return (a);
}

f32 rsqrt(f32 num) {
	long i;
	f32 x2, y;
	const f32 threehalfs = 1.5f;

	x2 = num * 0.5f;
	y = num;
	i = *(long *) &y;
	i = 0x5f3759df - (i >> 1);
	y = *(f32 *) &i;
	y = y * (threehalfs - (x2 * y * y));

	return y;
}

f64 pow(f64 number, long power) {
	f64 result = 1;
	while (power > 0) {
		if (power & 1) {
			result *= (result * number);
		}
		number = (number * number);
		power >>= 1;
	}
	return (result);
}

f32 smoothStep(f32 t) {
    f32 v1 = t * t;
    f32 v2 = 1.0f - (1.0f - t) * (1.0f - t);
    return (Lerp(v1, v2, t));
}

f32 clamp(f32 value, f32 lo, f32 hi) {
	if (value < lo) {
		return (lo);
	}
	if (value > hi) {
		return (hi);
	}
	return (value);
}

void travelTarget(Vector2 *current, const Vector2 target, const f32 velocity, const f32 deltaTime) {
	Vector2 travel = {target.x - current->x, target.y - current->y};
	f32	distance = Vector2Distance(*current, target);
	f32	step = velocity * deltaTime;
	if (distance > step * step) {
		f32 tmp = rsqrt(distance);
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
