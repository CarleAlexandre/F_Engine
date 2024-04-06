# include <engine.hpp>

extern t_engine engine;

void renderMenu(void) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		DrawRectangle(100, 50, 50, 50, RED);
	EndTextureMode();
}

void renderOnline(void) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		DrawRectangle(100, 50, 50, 50, RED);
	EndTextureMode();
}

void renderSolo(void) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		BeginMode2D(engine.camera);
		
		EndMode2D();
	EndTextureMode();
}
