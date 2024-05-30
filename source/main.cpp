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
	t_player ret;

	ret.name = "default";
	ret.status = player_status_well;
	ret.pos = {spawn.x, spawn.z};
	ret.xp = 0.0f;
	ret.lvl = 1;
	ret.y = spawn.y;
	ret.dir = NORTH;
	ret.hitbox = {ret.pos.x + 6, ret.pos.y - 8, 12, 12};
	ret.inventory.clear();
	ret.inventory_size = 36;
	ret.frame = (Rectangle){0, 0, 32, 32};
	ret.stats = {
		.move_speed = 20,
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
	};
	return (ret);
}

int main(void) {
	//std::thread sync_thread;
	engine.status = engine_status_menu;
	engine.height = 480;
	engine.width = 720;
	engine.camera.zoom = 2.0f;
	engine.camera.offset = {(float)(engine.width * 0.5), (float)(engine.height * 0.5)};
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

	InitWindow(engine.width, engine.height, "noheaven");
	SetTargetFPS(120);

	GuiLoadStyle("include/styles/terminal/style_terminal.rgs");
	engine.fbo = LoadRenderTexture(engine.width, engine.height);
	engine.textures = loadAllTexture(engine.texture_dictionnary);
	engine.players = loadAllSave();
	engine.levels = loadAllLevel();
	loadInput(engine.input);

	while (engine.status != engine_status_close) {
		if (WindowShouldClose()) {
			engine.status.store(engine_status_close);
		}
		switch (engine.status.load()) {
			case (engine_status_solo): {
				updateInput();
				updatePlayer();
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
			default:
				break;
		}
		BeginDrawing();
			ClearBackground(BLACK);
			DrawTextureRec(engine.fbo.texture, {0, 0, (float)engine.width.load(), -(float)engine.height.load()}, {0, 0}, WHITE);
			/*if (enviroenment == Darkness) {	
				BeginShaderMode(engine.shader);
					DrawRectangle(0, 0, engine.display.width, engine.display.height, WHITE);
				EndShaderMode();
			}*/
			DrawFPS(0, 0);
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
