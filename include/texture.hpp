#ifndef TEXTURE_HPP
# define TEXTURE_HPP

#include "include.hpp"
#include <vector>
#include <raylib.h>
#include "raymath.h"
#include "haven.hpp"

typedef enum {
    text
} e_texture;

typedef struct s_animation {
	u32 max_frame;
	e_texture texture_idx;
	u32 current_frame;
	u32 frame_idx;
	double frame_time;
	frame_loop_e loop_type;
	double max_time;
	int incr = 1;
	Vector2 pos;
} t_animation;

class ATLAS {
    private:
        std::vector<Texture2D> textures;
        std::vector<t_animation> animation_queue;
    public:    
        const Rectangle getTextureRec(const unsigned int idx, e_texture id) {
            float x = getXpos(idx, textures[id].width / 32) * 32;
            float y = getYpos(idx, textures[id].width / 32) * 32;
            return ((const Rectangle){x, y, 32, 32});
        }

        void renderTextureChunk(const u32 idx, e_texture id, Vector2 pos) {
            DrawTextureRec(textures[id], getTextureRec(idx, id), pos, WHITE);
        }

        void renderAnimationFrame(u32 idx) {
            renderTextureChunk(animation_queue[idx].frame_idx + animation_queue[idx].current_frame, animation_queue[idx].texture_idx, {animation_queue[idx].pos.x, animation_queue[idx].pos.y});
        }

        //return -1 if animation is ended
        //need to test if it work
        int updateAnimation(u32 idx, const Vector2 *pos) {
            auto animationframe = &animation_queue[idx];
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

        Texture2D const getTexture(u32 idx) {
            return ((Texture2D const) textures.at(idx));
        };

		void updatePlayerAnimation(u32 idx, const int player_stats, const Vector2 pos) {
			updateAnimation(idx, &pos);
			switch (player_stats) {
				case (0): {
					animation_queue[idx].frame_idx = 0;
					animation_queue[idx].current_frame = 0;
					break;
				}
				case (1): {
					animation_queue[idx].frame_idx = 6;
					animation_queue[idx].current_frame = 0;
					break;
				}
				default:break;
			}
		}

        int addAnimationToQueue(const e_texture texture_idx, const Vector2 pos, const u32 max_frame, const u32 frame_idx, frame_loop_e looptype) {
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

        void deleteAnimationFromQueue(u32 idx) {
            animation_queue.erase(animation_queue.begin() + idx);
        }
};

#endif