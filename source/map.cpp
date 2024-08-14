#include <engine.hpp>
#include <raygui.h>

extern t_engine engine;

void drawLevel(t_level &level) {
	Vector2 beg = GetScreenToWorld2D({0,0}, engine. camera);
	Vector2 end = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, engine. camera);

	for (float y = beg.y; y < level.dimension.y && y < end.y; y ++) {
		for (float z = 0; z < level.dimension.z; z += 32){
			for (float x = beg.x; x < level.dimension.x && x < end.x; x += 32) {
				DrawTextureRec(engine.textures.env[engine.texture_dictionnary[level.tileset]], {(float)level.terrain[linearIndexFromCoordinate({x, y, z}, level.dimension.x, level.dimension.z)], }, {x, z}, WHITE);
			}
		}
	}
}
