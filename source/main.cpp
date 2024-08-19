# include <engine.hpp>
# include <raygui.h>

t_engine engine;

int main(void) {
	engine.status = engine_status_menu;
	engine.camera.zoom = 2.0f;
	engine.camera.target = Vector2Zero();

	InitWindow(1920, 1080, "noheaven");
	SetTargetFPS(120);
	GuiLoadStyle("include/styles/terminal/style_terminal.rgs");
	engine.fbo = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	engine.textures = loadAllTexture();
	engine.players = loadAllSave();
	engine.levels = loadAllLevel();
	loadInput(engine.input);
	engine.camera.offset = {(float)(GetScreenWidth() * 0.5), (float)(GetScreenHeight() * 0.5)};
	engine.level_idx = 0;

	while (engine.status != engine_status_close) {
		if (WindowShouldClose()) {
			engine.status.store(engine_status_close);
		}
		switch (engine.status.load()) {
			case (engine_status_solo): {
				updateInput();
				if (updatePlayer()) {
					updatePlayerAnimation(&engine.animation_queue[engine.current_save->animation_idx], engine.current_save->action);
				} else {
					updatePlayerAnimation(&engine.animation_queue[engine.current_save->animation_idx], player_action_default);
				}
				renderSolo();
				break;
			}
			case (engine_status_menu): {
				renderMenu();
				break;
			}
			case (engine_status_save): {
				renderSave();
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
			/*if (enviroenment == Darkness) {
				BeginShaderMode(engine.shader);
					DrawRectangle(0, 0, engine.display.width, engine.display.height, WHITE);
				EndShaderMode();
			}*/
		EndDrawing();
	}

	for (int i = 0; i < engine.players.size(); i++) {
		savePlayerData(engine.players[i]);
		engine.players[i].name.clear();
	}
	engine.players.clear();
	UnloadRenderTexture(engine.fbo);
	CloseWindow();
}
