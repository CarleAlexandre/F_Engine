#include <engine.hpp>

extern t_engine engine;

void freeLevel(t_level *level) {
	MemFree(level->event);
	level->event = NULL;
	level->terrain = NULL;
	level->wall = NULL;
	level->dimension = {0, 0};
}

void drawLevel(t_level &level) {
	Vector2 beg = GetScreenToWorld2D({0,0}, engine. camera);
	Vector2 end = GetScreenToWorld2D({(float)engine.width, (float)engine.height}, engine. camera);

	for (float y = beg.y; y < level.dimension.y && y < end.y; y += 32) {
		for (float x = beg.x; x < level.dimension.x && x < end.x; x += 32) {
			DrawTextureRec(engine.textures[engine.texture_dictionnary["RA_Ground_Tiles"]], {(float)level.terrain[getLinearIndex(x, y, level.dimension.x)], }, {x, y}, WHITE);
		}
	}
}
