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
	std::atomic_int status;
	Font font;
	Camera2D camera;
	RenderTexture fbo;
	t_textures *textures;
	std::vector<t_level> levels;
	std::vector<t_animation> animation_queue;
	u32 level_idx;
} t_engine;

void renderMenu(void);
void renderOnline(void);
void renderSolo(void);
void renderSave(void);
void renderSetting(void);

int updateAnimation(t_animation *animationframe, const Vector3 *pos);
void renderAnimationFrame(t_animation &animationframe);
void renderTextureChunk(const u32 idx, const Texture2D &texture, Vector2 pos);
const Rectangle getTextureRec(const u32 idx, const Texture2D &texture);
int addAnimationToQueue(const int texture_idx, std::vector<t_animation> &animation_queue, const Vector3 pos, const u32 max_frame, const u32 frame_idx, frame_loop_e looptype);
void deleteAnimationFromQueue(std::vector<t_animation> &animation_queue, u32 idx);
void updatePlayerAnimation(t_animation *player_animation, const player_action_e player_stats);
int mapBuilder(std::vector<Texture2D> textures);
t_level loadLevel(const char *level_name);
void writeToLevel(t_level level);
int linearIndexFromCoordinate(Vector3 dim, int max_x, int max_y);
Vector3 coordinateFromLinearIndex(int idx, float max_x, float max_y);

#endif
