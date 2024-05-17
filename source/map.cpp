#include <engine.hpp>

t_level readLevel(const char *map, const Vector2 dim) {
	t_level level;
	int size;

	size = dim.x * dim.y;
	level.dimension = dim;
	level.event = (int *)MemAlloc(size);
	level.terrain = level.event + size;
	level.wall = level.terrain + size;

	
	return (level);
}

void freeLevel(t_level *level) {
	MemFree(level->event);
	level->event = NULL;
	level->terrain = NULL;
	level->wall = NULL;
	level->dimension = {0, 0};
}
