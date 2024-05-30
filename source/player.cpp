# include <engine.hpp>

extern t_engine engine;

void updateInput(void) {
	for (int i = 0; i < MAX_INPUT; i++) {
		if (engine.input[i].ismouse && IsMouseButtonDown(engine.input[i].key)) {
			switch(engine.input[i].id){
				case(move):{
					engine.current_save->to = GetScreenToWorld2D(GetMousePosition(), engine.camera);
					engine.current_save->to.x -= 16;
					engine.current_save->to.y -= 24;
#ifdef DEBUG
					//std::cout << engine.current_save->to.x << ", " <<  engine.current_save->to.y, "\n";
#endif
					break;
				}
				case(autoattack):{
					break;
				}
				default:
				break;
			}
		}
		if (!engine.input[i].ismouse && IsKeyDown(engine.input[i].key)) {
			switch(engine.input[i].id){
				case(interact):{
					break;
				}
				case(hotbar1):{
					break;
				}
				case(hotbar2):{
					break;
				}
				case(hotbar3):{
					break;
				}
				case(hotbar4):{
					break;
				}
				case(hotbar5):{
					break;
				}
				case(hotbar6):{
					break;
				}
				case(toggleinventory):{
					break;
				}
				case(centercamera):{
					engine.camera.target = {engine.current_save->pos.x + 16, engine.current_save->pos.y + 24};
					break;
				}
				default:
				break;
			}
		}
	}
}

void updatePlayer(void) {
	if (Vector2Distance(engine.current_save->pos, engine.current_save->to) > 0.1) {
		travelTarget(&engine.current_save->pos, engine.current_save->to, engine.current_save->stats.move_speed, GetFrameTime());
	}
}

