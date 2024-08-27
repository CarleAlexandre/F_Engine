#ifndef TEXTURE_HPP
# define TEXTURE_HPP

#include "include.hpp"

typedef enum {
    text_brush = 0,
    text_equipement = 1,
    text_erase = 2,
    text_fill = 3,
    text_hero = 4,
    text_tileset = 5,
	text_reverse_hero = 6,
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
	Vector2 *pos;
} t_animation;

class ATLAS {
    private:
        //std::vector<t_animation> animation_queue;
    public:    
        std::vector<Texture2D> textures;
        std::list<t_animation> animation_queue;
        t_animation player_anim;
        const Rectangle getTextureRec(const unsigned int idx, e_texture id) {
            float x = getXpos(idx, textures[id].width / 32) * 32;
            float y = getYpos(idx, textures[id].width / 32) * 32;
            return ((const Rectangle){x, y, 32, 32});
        }

		void sort() {
			for (auto tmp : animation_queue) {
				if (tmp.pos->y) {

				}
			}
		}

        void renderTextureChunk(const u32 idx, e_texture id, Vector2 pos) {
           	DrawTextureRec(textures[id], getTextureRec(idx, id), pos, WHITE);
        }

        void renderAnimationFrame(char dir, float y) {
			bool player_render = false;
			float current_y = 0;
            if (!animation_queue.empty()) {
                for (auto animation_frame : animation_queue) {
					current_y = animation_frame.pos->y;
					if (y < current_y && player_render == false) {
						renderPlayerAnimation(dir);
						player_render = true;
					}
                    renderTextureChunk(animation_frame.frame_idx + animation_frame.current_frame, animation_frame.texture_idx, *animation_frame.pos);
				}
				if (player_render == false) {
					renderPlayerAnimation(dir);
				}
			}
        }

        void renderPlayerAnimation(char dir) {
			if (dir == WEST) {
				renderTextureChunk(player_anim.frame_idx + player_anim.current_frame, text_reverse_hero, *player_anim.pos);
			} else {
				renderTextureChunk(player_anim.frame_idx + player_anim.current_frame, text_hero, *player_anim.pos);
			}
        }

        //return -1 if animation is ended
        //need to test if it work
        void updateAnimation() {
            for (auto animationframe = animation_queue.begin();animationframe != animation_queue.end();animationframe++) {
                animationframe->frame_time += GetFrameTime();

                if (animationframe->frame_time >= animationframe->max_time) {
                    switch(animationframe->loop_type) {
                        case(frame_loop_none):{
                            animationframe->current_frame++;
                            if (animationframe->current_frame >= animationframe->max_frame) {
                                animation_queue.erase(animationframe);
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
            }
        }

        Texture2D getTexture(u32 idx) {
            return (textures.at(idx));
        };

		void updatePlayerAnimation(const int player_stats, char dir) {
            static int old_stats = 0 ;
			if (player_stats != old_stats) {
				switch (player_stats) {
					case (0): {
						player_anim.frame_idx = 0;
						player_anim.current_frame = 0;
						break;
					}
					case (1): {
						player_anim.frame_idx = 6;
						player_anim.current_frame = 0;
						break;
					}
					case (2): {
						player_anim.frame_idx = 50;
						player_anim.current_frame = 0;
					}
					default:break;
				}
				old_stats = player_stats;
			}
        	player_anim.frame_time += GetFrameTime();
            if (player_anim.frame_time >= player_anim.max_time) {
                player_anim.current_frame ++;
				if (player_anim.current_frame >= player_anim.max_frame) {
					player_anim.current_frame = 0;
				}
                player_anim.frame_time = 0;
            }
		}

        unsigned long long size(void) {
            return (textures.size());
        }

        int addAnimationToQueue(const e_texture texture_idx, Vector2 *pos, const u32 max_frame, const u32 frame_idx, frame_loop_e looptype) {
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
			sort();
            return (animation_queue.size() - 1);
        }

        ATLAS(Vector2 *pos): textures(), animation_queue() {
            FilePathList textFile = LoadDirectoryFiles("assets/textures");
            for (int i = 0; i < textFile.count; i++) {
                textures.push_back(LoadTexture(textFile.paths[i]));
            }
            UnloadDirectoryFiles(textFile);
			auto tmp = LoadImageFromTexture(textures[text_hero]);
			ImageFlipHorizontal(&tmp);
			textures.push_back(LoadTextureFromImage(tmp));
			UnloadImage(tmp);
            player_anim = (t_animation){
                .max_frame = 6,
                .texture_idx = text_hero,
                .current_frame = 0,
                .frame_idx = 0,
                .frame_time = 0,
                .loop_type = frame_loop_enable,
                .max_time = 0.4,
                .pos = pos,
            };
        };
        ~ATLAS(){
            for (int i = 0; i < textures.size(); i++) {
                UnloadTexture(textures[i]);
            }
        };
};

#endif