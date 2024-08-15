# include <engine.hpp>

extern t_engine engine;

const Rectangle getTextureRec(const u32 idx, const Texture2D &texture) {
	float x = getXpos(idx, texture.width / 32) * 32;
	float y = getYpos(idx, texture.width / 32) * 32;
	return ((const Rectangle){x, y, 32, 32});
}

void renderTextureChunk(const u32 idx, const Texture2D &texture, Vector2 pos) {
	DrawTextureRec(texture, getTextureRec(idx, texture), pos, WHITE);
}

void renderAnimationFrame(t_animation &animationframe) {
			renderTextureChunk(animationframe.frame_idx + animationframe.current_frame, engine.textures->hero[animationframe.texture_idx], {animationframe.pos.x, animationframe.pos.z});
	//switch (type) {
	//	case (text_hero):{
	//		break;
	//	}
	//	case (text_env):{
	//		break;
	//	}
	//	case (text_item):{
	//		break;
	//	}
	//	case (text_mod):{
	//		break;
	//	}
	//	case (text_ui):{
	//		break;
	//	}
	//	default:break;
	//}
}

//return -1 if animation is ended
int updateAnimation(t_animation *animationframe, const Vector3 *pos) {
	animationframe->frame_time += GetFrameTime();

	if (pos) {
		animationframe->pos = *pos;
	}
	if (animationframe->frame_time >= animationframe->max_time) {
		switch(animationframe->loop_type) {
			case(frame_loop_none):{
				animationframe->current_frame++;
				if (animationframe->current_frame >= animationframe->max_frame) {
					return (-1);
				}
				break;
			}
			case(frame_loop_enable):{
				animationframe->current_frame++;
				if (animationframe->current_frame >= animationframe->max_frame) {
					animationframe->current_frame = 0;
				}
				break;
			}
			case(frame_loop_reverse):{
				animationframe->current_frame--;
				if (animationframe->current_frame <= 0) {
					animationframe->current_frame = animationframe->max_frame;
				}
				break;
			}
			case(frame_loop_updown):{
				animationframe->current_frame += animationframe->incr;
				if (animationframe->current_frame >= animationframe->max_frame) {
					animationframe->incr *= -1;
				}
				break;
			}
		}
		animationframe->frame_time = 0;
	}
	return (0);
}

int addAnimationToQueue(const int texture_idx, std::vector<t_animation> &animation_queue, const Vector3 pos, const u32 max_frame, const u32 frame_idx, frame_loop_e looptype) {
	t_animation new_animation;

	new_animation.texture_idx = texture_idx;
	new_animation.pos = pos;
	new_animation.loop_type = looptype;
	new_animation.current_frame = 0;
	new_animation.max_time = 0.4;
	new_animation.frame_time = 0;
	new_animation.max_frame = max_frame;
	new_animation.frame_idx = frame_idx;
	animation_queue.push_back(new_animation);
	return (animation_queue.size() - 1);
}

void deleteAnimationFromQueue(std::vector<t_animation> &animation_queue, u32 idx) {
	animation_queue.erase(animation_queue.begin() + idx);
}

void updatePlayerAnimation(t_animation *player_animation, const player_action_e player_stats) {
	updateAnimation(player_animation, &engine.current_save->pos);
	switch (player_stats) {
		case (player_action_idle): {
			player_animation->frame_idx = 0;
			player_animation->current_frame = 0;
			break;
		}
		case (player_action_moving): {
			player_animation->frame_idx = 6;
			player_animation->current_frame = 0;
			break;
		}
		case (player_action_hurt): {
			player_animation->frame_idx = 50;
			player_animation->current_frame = 0;
			break;
		}
		default:break;
	}
}
