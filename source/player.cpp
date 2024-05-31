# include <engine.hpp>

extern t_engine engine;

void updateInput(void) {
	for (int i = 0; i < MAX_INPUT; i++) {
		if (engine.input[i].ismouse && IsMouseButtonDown(engine.input[i].key)) {
			switch(engine.input[i].id){
				case(move):{
					Vector2 to;
					to = GetScreenToWorld2D(GetMousePosition(), engine.camera);
					engine.current_save->to.x = to.x - 16;
					engine.current_save->to.z = to.y - 24;
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
					engine.camera.target = {engine.current_save->pos.x + 16, engine.current_save->pos.z + 24};
					break;
				}
				default:
				break;
			}
		}
	}
}

int updatePlayer(void) {
	static player_action_e last_action = player_action_default;
	int ret = 0;
	Vector2 from = {engine.current_save->pos.x, engine.current_save->pos.z};
	Vector2 to = {engine.current_save->to.x, engine.current_save->to.z};

	if (Vector2Distance(from, to) > 0.1) {
		travelTarget(&from, to, engine.current_save->stats.move_speed, GetFrameTime());
		engine.current_save->pos.x = from.x;
		engine.current_save->pos.z = from.y;
		if (last_action != player_action_moving) {
			engine.current_save->action = player_action_moving;
			last_action = engine.current_save->action;
			ret = 1;
		}
	} else {
		if (last_action != player_action_idle) {
			engine.current_save->action = player_action_idle;
			last_action = engine.current_save->action;
			ret = 1;
		}
	}
	return (ret);
}
