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
	static int stats = 0;

	switch (stats) {
		case (0): {
			if (GuiButton({40, 90, 100, 50}, "Video")){
				stats = 1;
			}
			if (GuiButton({40, 180, 100, 50}, "Sound")){
				stats = 2;
			}	
			if (GuiButton({40, 270, 100, 50}, "Input")){
				stats = 3;
			}	
			if (GuiButton({40, 360, 100, 50}, "Title Screen")){
				engine.status.store(engine_status_menu);
				stats = 0;
			}
		}
		case (1):{

		}
		case (2):{

		}
		case (3):{

		}
		default: break;
	}
}

void saveUi() {
		static int stats = 0;

	switch (stats) {
		case (0): {
			if (GuiButton({40, 90, 100, 50}, "LoadSave")){
				stats = 1;
			}
			if (GuiButton({40, 180, 100, 50}, "NewSave")){
				stats = 2;
			}	
			if (GuiButton({40, 270, 100, 50}, "DeleteSave")){
				stats = 3;
			}	
			if (GuiButton({40, 360, 100, 50}, "Title Screen")){
				engine.status.store(engine_status_menu);
				stats = 0;
			}
			break;
		}
		case (1):{
			if (GuiButton({40, 90, 100, 50}, "Slot1")){
				engine.status.store(engine_status_solo);
				//load player slot1
				stats = 0;
			}
			if (GuiButton({40, 180, 100, 50}, "Slot2")){
				engine.status.store(engine_status_solo);
				//load player slot2
				stats = 0;
			}
			if (GuiButton({40, 270, 100, 50}, "Slot3")){
				engine.status.store(engine_status_solo);
				//load player slot3
				stats = 0;
			}	
			if (GuiButton({40, 360, 100, 50}, "Back")){
				stats = 0;
			}
			break;
		}
		case (2):{
			if (GuiButton({40, 90, 100, 50}, "Slot1")){
				engine.status.store(engine_status_solo);
				//if exist ask if replace then init default player into slot;
				stats = 0;
			}
			if (GuiButton({40, 180, 100, 50}, "Slot2")){
				engine.status.store(engine_status_solo);
				//if exist ask if replace then init default player into slot;
				stats = 0;
			}
			if (GuiButton({40, 270, 100, 50}, "Slot3")){
				engine.status.store(engine_status_solo);
				//if exist ask if replace then init default player into slot;
				stats = 0;
			}	
			if (GuiButton({40, 360, 100, 50}, "Back")){
				stats = 0;
			}
			break;
		}
		case (3):{
			if (GuiButton({40, 90, 100, 50}, "Slot1")){
				//delete slot 1
			}
			if (GuiButton({40, 180, 100, 50}, "Slot2")){
				//delete slot 2
			}
			if (GuiButton({40, 270, 100, 50}, "Slot3")){
				//delete slot 3
			}	
			if (GuiButton({40, 360, 100, 50}, "Back")){
				stats = 0;
			}
			break;
		}
		default: break;
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

