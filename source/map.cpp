#include <engine.hpp>

extern t_engine engine;

void freeLevel(t_level *level) {
	MemFree(level->event);
	MemFree(level->terrain);
	MemFree(level->wall);
	level->dimension = {0, 0, 0};
}

void drawLevel(t_level &level) {
	Vector2 beg = GetScreenToWorld2D({0,0}, engine. camera);
	Vector2 end = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, engine. camera);

	for (float y = beg.y; y < level.dimension.y && y < end.y; y ++) {
		for (float z = 0; z < level.dimension.z; z += 32){
			for (float x = beg.x; x < level.dimension.x && x < end.x; x += 32) {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["RA_Ground_Tiles"]], {(float)level.terrain[(int)y][getLinearIndex(x, z, level.dimension.x)], }, {x, z}, WHITE);
			}
		}
	}
}
