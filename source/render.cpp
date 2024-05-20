# include <engine.hpp>
# define RAYGUI_IMPLEMENTATION
# include <raygui.h>

extern t_engine engine;

void mapBuilder() {

}

void menuUi(void) {
	if (GuiButton({40, 90, 100, 50}, "Solo")){
		engine.status.store(engine_status_save); 
	}
	if (GuiButton({40, 180, 100, 50}, "Setting")){
		engine.status.store(engine_status_setting);
	}
	if (GuiButton({40, 270, 100, 50}, "Online")){
		engine.status.store(engine_status_online);
	}
	if (GuiButton({40, 360, 100, 50}, "Exit")) {
		engine.status.store(engine_status_close);
	}
}

void settingUi() {
	if (GuiButton({40, 90, 100, 50}, "Video")){
	}
	if (GuiButton({40, 180, 100, 50}, "Sound")){
	}	
	if (GuiButton({40, 270, 100, 50}, "Input")){
	}	
	if (GuiButton({40, 360, 100, 50}, "Title Screen")){
		engine.status.store(engine_status_menu);
	}
}

void saveUi() {
	if (GuiButton({40, 90, 100, 50}, "LoadSave")){
		engine.status.store(engine_status_solo);
	}
	if (GuiButton({40, 180, 100, 50}, "NewSave")){
	}	
	if (GuiButton({40, 270, 100, 50}, "DeleteSave")){
	}	
	if (GuiButton({40, 360, 100, 50}, "Title Screen")){
		engine.status.store(engine_status_menu);
	}
}

void renderMenu(void) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		menuUi();
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

void renderSave() {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		saveUi();
	EndTextureMode();
}

void renderSetting() {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		settingUi();
	EndTextureMode();
}

