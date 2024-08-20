#ifndef ENGINE_HPP
# define ENGINE_HPP

#include "player.hpp"
#include "map.hpp"
#include "entity.hpp"
#include "config.hpp"
#include "texture.hpp"

typedef enum {
	engine_status_menu = 0,
	engine_status_close = 1,
	engine_status_online = 2,
	engine_status_solo = 3,
	engine_status_setting = 4,
	engine_status_save = 5,
	engine_status_level_editor = 6,
}engine_status_e;

typedef struct s_engine {
	int status;
	Font font;
	Camera2D camera;
	RenderTexture fbo;
} t_engine;

#endif
