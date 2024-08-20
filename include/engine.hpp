#ifndef ENGINE_HPP
# define ENGINE_HPP

# define GLSL_VERSION 330
# include <raylib.h>
# include <rlgl.h>
# include <raymath.h>

#include "inventory.hpp"
#include "player.hpp"
#include "map.hpp"
#include "entity.hpp"
#include "config.hpp"
#include "texture.hpp"

typedef enum {
	entity_type_mobs,
	entity_type_bullet,
	entity_type_effect,
	entity_type_gathering,
	entity_type_plant,
}entity_type_e;

typedef enum {
	engine_status_menu = 0,
	engine_status_close = 1,
	engine_status_online = 2,
	engine_status_solo = 3,
	engine_status_setting = 4,
	engine_status_save = 5,
	engine_status_level_editor = 6,
}engine_status_e;

typedef enum {
	tile_tool_use_shovel,
	tile_tool_use_pickaxe,
	tile_tool_use_axe,
	tile_tool_use_hoe,
	tile_tool_use_wrench,
	tile_tool_use_lockpick,
} tile_tool_use;

typedef struct s_engine {
	int status;
	Font font;
	Camera2D camera;
	RenderTexture fbo;
	std::vector<t_animation> animation_queue;
	u32 level_idx;
} t_engine;

#endif
