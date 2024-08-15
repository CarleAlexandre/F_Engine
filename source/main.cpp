# include <engine.hpp>
# include <raygui.h>

//#include <queue>
//std::queue<t_thread_queue> thread_queue;
t_engine engine;

//void callthread(void (*fun)(void *), void *arg) {
//	thread_queue.push((t_thread_queue){
//		.fun = fun,
//		.arg = arg,
//	});
//}

//void syncThread(int max_thread) {
//	std::vector<t_thread_handle> thread_pool;

//	for (int i = 0; i < max_thread; i++) {
//		thread_pool.push_back((t_thread_handle){});
//	}

//	while (engine.status.load() != engine_status_close) {
//		if (!thread_queue.empty()) {
//			for (int i = 0; i < max_thread; i++) {
//				if (thread_pool[i].mtx.try_lock()) {
//					if (thread_pool[i].available) {
//						thread_pool[i].available = false;
//						thread_pool[i].th = std::thread(thread_queue.front().fun, thread_queue.front().arg);
//						thread_queue.pop();
//					}
//					thread_pool[i].mtx.unlock();
//				}
//			}
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(16));
//		//need to join all thread here and send back isready true and wait for it to be false to restart another loop
//	}
//}

void updateFun() {

}

void audioFun() {

}

t_player defaultPlayerInit(const Vector3 spawn) {
	t_player default_player = (t_player){
		.pos = spawn,
		.dir = NORTH,
		.status = player_status_well,
		.lvl = 1,
		.xp = 0.0f,
		.inv = INVENTORY(),
		.stats = {
			.move_speed = 10,
			.crit_chance = 0.0f,
			.crit_dmg = 1.2f,
			.raw_dmg = 0.0f,
			.dmg_reduction = 0.0f,
			.armor = 30,
			.attack_speed = 0.6f,
			.life_steal = 0.0f,
			.mana = 300,
			.magic_affinity = 1.0f,
			.life = 600,
			.max_life = 600,
		},
		.hitbox = {spawn.x + 6, spawn.y - 8, 12, 12},
		.frame = (Rectangle){0, 0, 32, 32},
		.name = "default",
		.animation_idx = 0,
	};
	return (default_player);
}

int main(void) {
	//std::thread sync_thread;
	engine.status = engine_status_menu;
	engine.camera.zoom = 2.0f;
	engine.camera.target = Vector2Zero();
	//unsigned int max_thread = sync_thread.hardware_concurrency();
	//if (max_thread%2 == 0) {
	//	max_thread *= 0.5;
	//}
	//if (max_thread < 8) {
	//	max_thread = 2;
	//}
	//#ifdef DEBUG
	//	std::cout << max_thread;
	//#endif
	//sync_thread = std::thread(syncThread, max_thread);
	//#ifdef DEBUG
	//	std::cout << "Sync Thread Started!" << __LINE__ << std::endl;
	//#endif

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
				DisableEventWaiting();
				renderOnline();
				break;
			}
			case (engine_status_level_editor): {
				EnableEventWaiting();
				engine.status = mapBuilder(engine.textures->env);
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

	//sync_thread.join();
	for (int i = 0; i < engine.players.size(); i++) {
		savePlayerData(engine.players[i]);
		engine.players[i].name.clear();
	}
	engine.players.clear();
	for (int i = 0; i < engine.levels.size(); i++) {
		freeLevel(&engine.levels[i]);
	}
	UnloadRenderTexture(engine.fbo);
	CloseWindow();
}
