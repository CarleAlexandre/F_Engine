# include <engine.hpp>
# include <raylib.h>
# define RAYGUI_IMPLEMENTATION
# include <raygui.h>

extern t_engine engine;

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
	static Vector2 scroll;
	static Rectangle view;
	const Rectangle panelrec = {100, 100, 260, 200};
	Rectangle contentrec = {0, 0, 240, 0};
	static u32 deleteidx = 0;
	static bool promptdelete = false;

	contentrec.height = engine.players.size() * 80;
	switch (stats) {
		case (0): {
			if (GuiButton({40, 90, 100, 50}, "LoadSave")){
				stats = 1;
				if (!engine.players.size()) {
					stats = 0;
				}
			}
			if (GuiButton({40, 180, 100, 50}, "NewSave")){
				stats = 2;
			}	
			if (GuiButton({40, 270, 100, 50}, "DeleteSave")){
				stats = 3;
				if (!engine.players.size()) {
					stats = 0;
				}
			}	
			if (GuiButton({40, 360, 100, 50}, "Title Screen")){
				engine.status.store(engine_status_menu);
				stats = 0;
			}
			break;
		}
		case (1):{
			GuiScrollPanel(panelrec, "LoadSave", contentrec, &scroll, &view);

			BeginDrawing();
			BeginScissorMode(view.x, view.y, view.width, view.height);
				for (int i = 0; i < engine.players.size(); i++) {
					if (GuiButton({panelrec.x + 20 + scroll.x, panelrec.y + 30 + 70 * i + scroll.y, contentrec.width - 20, 60}, engine.players[i].name.c_str())) {
						engine.current_save = &engine.players[i];
						engine.current_save->to = engine.current_save->pos;
						engine.status.store(engine_status_solo);
						engine.current_save->animation_idx = addAnimationToQueue(engine.current_save->skin, engine.animation_queue, engine.current_save->pos, 6, 0, frame_loop_enable);
						stats = 0;
					}
				}
			EndScissorMode();

			if (GuiButton({40, 360, 100, 50}, "Back")){
				scroll.y = 0;
				stats = 0;
			}
			break;
		}
		case (2):{
			static char player_name[15] = "\0";
			static char player_skinidx = 1;
			Texture2D tmp =  engine.textures[engine.texture_dictionnary["skin"]];

			GuiTextBox({20, 50, 200, 20}, player_name, 15, true);
			if (player_skinidx > 1) {
				if (GuiButton({20, 80, 80, 20}, "Prev")) {
					player_skinidx--;
#ifdef DEBUG
			std::cout << TextFormat("Hero_%02i\n", player_skinidx);
#endif
				}
			}
			if (player_skinidx < 30) {
				if (GuiButton({120, 80, 80, 20}, "Next")) {
					player_skinidx++;
#ifdef DEBUG
			std::cout << TextFormat("Hero_%02i\n", player_skinidx);
#endif
				}
			}
			player_skinidx = clamp(player_skinidx, 1, 30);
			if (GuiButton({200, 360, 100, 50}, "Create!")) {
				t_player new_player = defaultPlayerInit(Vector3Zero());
				new_player.name = player_name;
				new_player.skin = engine.texture_dictionnary[TextFormat("Hero_%02i", player_skinidx)];
				new_player.to = new_player.pos;
				engine.players.push_back(new_player);
				engine.current_save = engine.players.data() + ((engine.players.size() - 1) * sizeof(t_player));
				engine.status.store(engine_status_solo);
				engine.current_save->animation_idx = addAnimationToQueue(engine.current_save->skin, engine.animation_queue, engine.current_save->pos, 6, 0, frame_loop_enable);
				stats = 0;
			}
			if (GuiButton({40, 360, 100, 50}, "Back")){
				stats = 0;
				scroll.y = 0;
			}
			DrawTextureRec(engine.textures[engine.texture_dictionnary[TextFormat("Hero_%02i", player_skinidx)]], {0, 0, 32, 32}, {40, 100}, WHITE);
			break;
		}
		case (3):{
			GuiScrollPanel(panelrec, "DeleteSave", contentrec, &scroll, &view);

			BeginDrawing();
			BeginScissorMode(view.x, view.y, view.width, view.height);
				for (int i = 0; i < engine.players.size(); i++) {
					if (GuiButton({panelrec.x + 20 + scroll.x, panelrec.y + 30 + 70 * i + scroll.y, contentrec.width - 20, 60}, engine.players[i].name.c_str())) {
						promptdelete = true;
						deleteidx = i;
					}
				}
			EndScissorMode();

			if (promptdelete) {
				int result = GuiMessageBox({100, 100, 200, 200}, "#152#DELETE", "#152#Are you sure ??", "#152#;NO!!");
					if (result == 1) {
						std::remove(TextFormat("save/%s.player", engine.players[deleteidx].name.c_str()));
						engine.players.erase(engine.players.begin() + deleteidx);
						stats = 0;
					}
					if (result >= 0) {
						promptdelete = false;
						deleteidx = 0;
					}
			}

			if (GuiButton({40, 360, 100, 50}, "Back")){
				stats = 0;
				scroll.y = 0;
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
			for (int i = 0; i < engine.animation_queue.size(); i++) {
				if (updateAnimation(&engine.animation_queue[i], NULL) == -1) {
					deleteAnimationFromQueue(engine.animation_queue, i);
				} else {
					renderAnimationFrame(engine.animation_queue[i]);
				}
			}
			renderAnimationFrame(engine.animation_queue[engine.current_save->animation_idx]);
			//drawLevel(engine.levels[0]);
		EndMode2D();
		//DrawPixel(GetScreenWidth() * 0.5, GetScreenHeight() * 0.5, PINK);
		DrawText(TextFormat("x: %.1f, z:%.1f", engine.current_save->pos.x, engine.current_save->pos.z), 20, 40, 20, GREEN);
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

