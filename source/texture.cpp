# include <engine.hpp>

typedef struct textureChunk {
	int height, width;
	Vector2 pos;
	u32 textureId;
} textureChunk;

const Rectangle getTextureRec(const u32 idx, const Texture2D &texture) {
	return ((const Rectangle){getXpos(idx, texture.width), getYpos(idx, texture.width), 32, 32});
}

void renderTextureChunk(const u32 idx, const Texture2D &texture, Vector2 pos) {
	DrawTextureRec(texture, getTextureRec(idx, texture), pos, WHITE);
}

//return -1 if animation is ended
int updateAnimation(t_animation *animationframe) {
	animationframe->frametime += GetFrameTime();

	if (animationframe->maxtime <= animationframe->frametime) {
		switch(animationframe->looptype) {
			case(frame_loop_none):{
				animationframe->currentframe++;
				if (animationframe->currentframe >= animationframe->maxframe) {
					animationframe->frametime = 0;
					return (-1);
				}
				break;
			}
			case(frame_loop_enable):{
				animationframe->currentframe++;
				if (animationframe->currentframe >= animationframe->maxframe) {
					animationframe->currentframe = 0;
					animationframe->frametime = 0;
				}
				break;
			}
			case(frame_loop_reverse):{
				animationframe->currentframe--;
				if (animationframe->currentframe <= 0) {
					animationframe->currentframe = animationframe->maxframe;
					animationframe->frametime = 0;
				}
				break;
			}
			case(frame_loop_updown):{
				animationframe->currentframe += animationframe->incr;
				if (animationframe->currentframe >= animationframe->maxframe) {
					animationframe->incr *= -1;
					animationframe->frametime = 0;
				}
				break;
			}
		}
	}
	return (0);
}
