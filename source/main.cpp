#include <engine.hpp>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

t_engine engine;

void menuUi(void) {
	if (GuiButton({40, 90, 100, 50}, "Solo")){
		engine.status = engine_status_save; 
	}
	if (GuiButton({40, 180, 100, 50}, "Setting")){
		engine.status = engine_status_setting;
	}
	if (GuiButton({40, 270, 100, 50}, "Online")){
		engine.status = engine_status_online;
	}
	if (GuiButton({40, 360, 100, 50}, "Map Editor")){
		engine.status = engine_status_level_editor;
	}
	if (GuiButton({40, 450, 100, 50}, "Exit")) {
		engine.status = engine_status_close;
	}
}

void settingUi() {
	static int stats = 0;

	switch (stats) {
		case (0): {
			if (GuiButton({40, 90, 100, 50}, "MAP_BUILDER")){
				stats = 1;
			}
			if (GuiButton({40, 180, 100, 50}, "Sound")){
				stats = 2;
			}	
			if (GuiButton({40, 270, 100, 50}, "Input")){
				stats = 3;
			}	
			if (GuiButton({40, 360, 100, 50}, "Title Screen")){
				engine.status = engine_status_menu;
				stats = 0;
			}
			break;
		}
		case (1):{
			break;
		}
		case (2):{
			break;
		}
		case (3):{
			break;
		}
		default: break;
	}
}

void saveUi(int save_number/*need to add save file list or database*/, ATLAS &atlas,PLAYER &player) {
	static int stats = 0;
	static Vector2 scroll;
	static Rectangle view;
	const Rectangle panelrec = {100, 100, 260, 200};
	Rectangle contentrec = {0, 0, 240, 0};
	static u32 deleteidx = 0;
	static bool promptdelete = false;

	contentrec.height = save_number * 80;
	switch (stats) {
		case (0): {
			if (GuiButton({40, 90, 100, 50}, "LoadSave")){
				stats = 1;
				if (!save_number) {
					stats = 0;
				}
			}
			if (GuiButton({40, 180, 100, 50}, "NewSave")){
				stats = 2;
			}	
			if (GuiButton({40, 270, 100, 50}, "DeleteSave")){
				stats = 3;
				if (!save_number) {
					stats = 0;
				}
			}	
			if (GuiButton({40, 360, 100, 50}, "Title Screen")){
				engine.status = engine_status_menu;
				stats = 0;
			}
			break;
		}
		case (1):{
			GuiScrollPanel(panelrec, "LoadSave", contentrec, &scroll, &view);

			BeginDrawing();
			BeginScissorMode(view.x, view.y, view.width, view.height);
				for (int i = 0; i < save_number; i++) {
					//load player and save list
					// if (GuiButton({panelrec.x + 20 + scroll.x, panelrec.y + 30 + 70 * i + scroll.y, contentrec.width - 20, 60}, engine.players[i].name.c_str())) {
					// 	engine.current_save = &engine.players[i];
					// 	engine.current_save->to = engine.current_save->pos;
					// 	engine.status.store(engine_status_solo);
					// 	engine.current_save->animation_idx = addAnimationToQueue(engine.current_save->skin, engine.animation_queue, engine.current_save->pos, 6, 0, frame_loop_enable);
					// 	stats = 0;
					// }
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

			GuiTextBox({20, 50, 200, 20}, player_name, 15, true);
			if (player_skinidx > 1) {
				if (GuiButton({20, 80, 80, 20}, "Prev")) {
					player_skinidx--;
				}
			}
			if (player_skinidx < 30) {
				if (GuiButton({120, 80, 80, 20}, "Next")) {
					player_skinidx++;
				}
			}
			player_skinidx = clamp(player_skinidx, 1, 30);
			if (GuiButton({200, 360, 100, 50}, "Create!")) {
				// t_player new_player = defaultPlayerInit(Vector3Zero());
				// new_player.name = player_name;
				// new_player.skin = player_skinidx;
				// new_player.to = new_player.pos;
				// engine.players.push_back(new_player);
				// engine.current_save = engine.players.data() + ((engine.players.size() - 1) * sizeof(t_player));
				// engine.status.store(engine_status_solo);
				// engine.current_save->animation_idx = addAnimationToQueue(engine.current_save->skin, engine.animation_queue, engine.current_save->pos, 6, 0, frame_loop_enable);
				// stats = 0;
			}
			if (GuiButton({40, 360, 100, 50}, "Back")){
				stats = 0;
				scroll.y = 0;
			}
			DrawTextureRec(atlas.getTexture(player_skinidx), {0, 0, 32, 32}, {40, 100}, WHITE);
			break;
		}
		case (3):{
			GuiScrollPanel(panelrec, "DeleteSave", contentrec, &scroll, &view);

			BeginDrawing();
			BeginScissorMode(view.x, view.y, view.width, view.height);
				for (int i = 0; i < save_number; i++) {
					if (GuiButton({panelrec.x + 20 + scroll.x, panelrec.y + 30 + 70 * i + scroll.y, contentrec.width - 20, 60}, "")) {
						promptdelete = true;
						deleteidx = i;
					}
				}
			EndScissorMode();

			if (promptdelete) {
				int result = GuiMessageBox({100, 100, 200, 200}, "#152#DELETE", "#152#Are you sure ??", "#152#;NO!!");
					if (result == 1) {
						// std::remove(TextFormat("assets/save/%s.player", engine.players[deleteidx].name.c_str()));
						// engine.players.erase(engine.players.begin() + deleteidx);
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

void renderSolo(ATLAS &atlas, PLAYER &player, MAP &map, Entity &entities) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		BeginMode2D(engine.camera);
			map.render(engine.camera, atlas);
			//add function to render animation frame before player pos and after player pos
			entities.render();
			atlas.renderAnimationFrame();
			atlas.renderPlayerAnimation(player.dir);
		EndMode2D();
		//DrawPixel(GetScreenWidth() * 0.5, GetScreenHeight() * 0.5, PINK);
		player.inv.render(atlas.getTexture(1));
		player.inv.tool_bar.render();
		DrawText(TextFormat("x: %.1f, z:%.1f", player.pos.x, player.pos.y), 20, 40, 20, GREEN);
		DrawFPS(0, 0);
	EndTextureMode();
}

void renderSave(ATLAS &atlas, PLAYER &player) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		saveUi(0, atlas, player);
	EndTextureMode();
}

void renderSetting() {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		settingUi();
	EndTextureMode();
}

int main(void) {
	printf("UwU\n");
	InitWindow(1920, 1080, "noheaven");
	InitAudioDevice();
	printf("initWindow\n");
	GuiLoadStyle("include/styles/terminal/style_terminal.rgs");
	printf("LoadGui\n");
	engine.fbo = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	printf("LOAD FBO\n");
	engine.camera.offset = {(float)(GetScreenWidth() * 0.5), (float)(GetScreenHeight() * 0.5)};
	printf("Camera\n");
	SetTargetFPS(120);

	MAP *map = new MAP();
	printf("Map\n");
	PLAYER player({1000, 1000});
	printf("player\n");
	ATLAS atlas(&player.pos);
	printf("atlas\n");
	SOUND sound = SOUND();
	printf("sound\n");
	Entity entities = Entity();
	printf("entity\n");

	engine.status = engine_status_solo;
	engine.camera.zoom = 4.0f;
	engine.camera.target = {1000, 1000};

	entities.spawnMobs({1032, 1032}, 2);
	entities.spawnMobs({1022, 1022}, 1);

	while (engine.status != engine_status_close) {
		if (WindowShouldClose()) {
			engine.status = engine_status_close;
		}
		 switch (engine.status) {
			case (engine_status_solo): {
				player.updateInput(engine.camera);
				player.update();
				entities.update();
				atlas.updateAnimation();
				atlas.updatePlayerAnimation(player.action, player.dir);
				renderSolo(atlas, player, *map, entities);
				break;
			}
			case (engine_status_menu): {
				renderMenu();
				break;
			}
			case (engine_status_save): {
				//renderSave(atlas, player);
				break;
			}
			case (engine_status_setting): {
				renderSetting();
				break;
			}
			case (engine_status_online): {
				renderOnline();
				break;
			}
			case (engine_status_level_editor): {
				EnableEventWaiting();
				//engine.status = mapBuilder(engine.textures);
				DisableEventWaiting();
				break;
			}
			default:
				break;
		}
		BeginDrawing();
			ClearBackground(BLACK);
			DrawTextureRec(engine.fbo.texture, {0, 0, (float)GetScreenWidth(), -(float)GetScreenHeight()}, {0, 0}, WHITE);
		// 	/*if (enviroenment == Darkness) {
		// 		BeginShaderMode(engine.shader);
		// 			DrawRectangle(0, 0, engine.display.width, engine.display.height, WHITE);
		// 		EndShaderMode();
		// 	}*/
		EndDrawing();
	}
	UnloadRenderTexture(engine.fbo);
	CloseAudioDevice();
	CloseWindow();
}
