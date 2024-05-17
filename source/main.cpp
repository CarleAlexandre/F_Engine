
# define DEBUG
# include <engine.hpp>

std::queue<t_thread_queue> thread_queue;
t_engine engine;

void callthread(void (*fun)(void *), void *arg) {
	thread_queue.push((t_thread_queue){
		.fun = fun,
		.arg = arg,
	});
}

void syncThread(int max_thread) {
	t_thread_handle thread_pool[max_thread];

	while (engine.status.load() != engine_status_close) {
		if (!thread_queue.empty()) {
			for (int i = 0; i < max_thread; i++) {
				if (thread_pool[i].mtx.try_lock()) {
					if (thread_pool[i].available) {
						thread_pool[i].available = false;
						thread_pool[i].th = std::thread(thread_queue.front().fun, thread_queue.front().arg);
						thread_queue.pop();
					}
					thread_pool[i].mtx.unlock();
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		//need to join all thread here and send back isready true and wait for it to be false to restart another loop
	}
}

void updateFun() {

}

void audioFun() {
}

int main(void) {
	std::thread sync_thread;
	engine.status = engine_status_menu;
	engine.height = 480;
	engine.width = 720;

	unsigned int max_thread = sync_thread.hardware_concurrency();
	if (max_thread%2 == 0) {
		max_thread *= 0.5;
	}
	if (max_thread < 8) {
		max_thread = 2;
	}
	#ifdef DEBUG
		std::cout << max_thread;
	#endif
	sync_thread = std::thread(syncThread, max_thread);
	#ifdef DEBUG
		std::cout << "Sync Thread Started!" << __LINE__ << std::endl;
	#endif

	InitWindow(engine.width, engine.height, "noheaven");
	SetTargetFPS(120);

	engine.fbo = LoadRenderTexture(engine.width, engine.height);
	SetTextureFilter(engine.fbo.texture, TEXTURE_FILTER_TRILINEAR);

	while (engine.status != engine_status_close) {
		if (WindowShouldClose()) {
			engine.status.store(engine_status_close);
		}
		switch (engine.status) {
			case (engine_status_solo): {
				renderSolo();
				break;
			}
			case (engine_status_menu): {
				renderMenu();
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

	sync_thread.join();
	UnloadRenderTexture(engine.fbo);
	CloseWindow();
}
