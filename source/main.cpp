
# define DEBUG
# include <engine.hpp>

std::mutex render_mtx;
std::condition_variable render_sync;
bool ready = false;

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

	while (true) {
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
	}
	
}

void updateFun() {

}

void audioFun() {

}

void renderThread(void *arg) {
	RenderTexture fbo;

	rlglInit(engine.width.load(), engine.height.load());
	fbo = LoadRenderTexture(engine.width.load(), engine.height.load());
	switch (engine.status.load()) {
		case (engine_status_solo): {
			BeginTextureMode(fbo);
				ClearBackground(BLACK);
				BeginMode2D(engine.camera.load());
				
				EndMode2D();
			EndTextureMode();
			break;
		}
		case (engine_status_menu): {
			BeginTextureMode(fbo);
				ClearBackground(BLACK);
				DrawRectangle(100, 50, 50, 50, RED);
			EndTextureMode();
			break;
		}
		default:
			break;
	}
	engine.fbo.store(fbo);
    std::lock_guard<std::mutex> lock(render_mtx);
    ready = true;

    render_sync.notify_one();
	rlglClose();
}

void renderFun(t_engine &engine) {
	switch (engine.status.load()) {
		case (engine_status_solo): {
			callthread(renderThread, NULL);

			std::unique_lock<std::mutex> lock(render_mtx);
			render_sync.wait(lock, [] { return ready; });

			BeginDrawing();
				ClearBackground(BLACK);
				DrawTextureRec(engine.fbo.load().texture, {0, 0, (float)engine.width, -(float)engine.height}, {0, 0}, WHITE);
				/*if (enviroenment == Darkness) {	
					BeginShaderMode(engine.shader);
						DrawRectangle(0, 0, engine.display.width, engine.display.height, WHITE);
					EndShaderMode();
				}*/
				DrawFPS(0, 0);
			EndDrawing();
			ready = false;
			break;
		}
		case (engine_status_menu): {
			BeginDrawing();
				ClearBackground(BLACK);
				callthread(renderThread, NULL);

				std::unique_lock<std::mutex> lock(render_mtx);
				render_sync.wait(lock, [] { return ready; });

				ready = false;
			EndDrawing();
			break;
		}
		case (engine_status_online): {
			BeginDrawing();
				ClearBackground(BLACK);
			EndDrawing();
			break;
		}
		default:
			break;
	}

}

int main(void) {
	std::thread sync_thread;
	engine.status.store(engine_status_menu);
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

	while (engine.status != engine_status_close) {
		if (WindowShouldClose()) {
			engine.status.store(engine_status_close);
		}
		renderFun(engine);
		UnloadRenderTexture(engine.fbo);
	}

	sync_thread.join();

	CloseWindow();
}