#ifndef ENGINE_HPP
# define ENGINE_HPP

# define GLSL_VERSION 330
# include <raylib.h>
# include <rlgl.h>
# include <raymath.h>

# include <atomic>
# include <thread>
# include <mutex>

#include "inventory.hpp"
# include "../HavenLib/include/haven.hpp"

# ifdef DEBUG
	# include <iostream>
#endif

#define MAX_INPUT 11

typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef ptrdiff_t ptr;
typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;

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
	player_status_well = 0,
	player_status_sick = 1,
	player_status_dead = 2,
	player_status_hungry = 3,
	player_status_drunk = 4,
	player_status_unlucky = 5,
	player_status_lucky = 6,
	player_status_cursed = 7,
	player_status_blessed = 8,
	player_status_god = 9,
}player_status_e;

typedef enum {
	player_action_default	= -1,
	player_action_idle		= 0,
	player_action_moving	= 1,
	player_action_hurt		= 2,
	playar_action_interact	= 3,
} player_action_e;

typedef enum {
	player_token_status			= 2,
	player_token_lvl			= 3,
	player_token_xp				= 4,
	player_token_move_speed		= 5,
	player_token_crit_chance	= 6,
	player_token_crit_dmg		= 7,
	player_token_raw_dmg		= 8,
	player_token_dmg_reduction	= 9,
	player_token_armor			= 10,
	player_token_attack_speed	= 11,
	player_token_life_steal		= 12,
	player_token_mana			= 13,
	player_token_magic_affinity	= 14,
	player_token_life			= 15,
	player_token_max_life		= 16,
	player_token_health_regen	= 17,
	player_token_mana_regen		= 18,
	player_token_name			= 19,
	player_token_skin			= 20,
} player_token_e;

typedef enum {
	SOUTH	= 1 << 0,
	NORTH	= 1 << 1,
	EAST	= 1 << 2,
	WEST	= 1 << 3,
}diraction_e;

typedef enum {
	move			= 1,
	autoattack		= 2,
	interact		= 3,
	hotbar1			= 4,
	hotbar2			= 5,
	hotbar3			= 6,
	hotbar4			= 7,
	hotbar5			= 8,
	hotbar6			= 9,
	toggleinventory	= 10,
	centercamera	= 11,
}player_input_e;

typedef enum {
	frame_loop_none = 0,
	frame_loop_enable = 1,
	frame_loop_reverse = 2,
	frame_loop_updown = 3,
} frame_loop_e;

typedef enum {
	tile_tool_use_shovel,
	tile_tool_use_pickaxe,
	tile_tool_use_axe,
	tile_tool_use_hoe,
	tile_tool_use_wrench,
	tile_tool_use_lockpick,
} tile_tool_use;

typedef enum {
	text_hero,
	text_env,
	text_item,
	text_mod,
	text_ui,
} text_type;

typedef struct s_input {
	int key;
	bool ismouse;
	player_input_e id;
}t_input;

typedef struct s_stats {
	float move_speed;
	float crit_chance;
	float crit_dmg;
	float raw_dmg;
	float dmg_reduction;
	float armor;
	float attack_speed;
	float life_steal;
	float mana;
	float magic_affinity;
	float life;
	float max_life;
	float health_regen;
	float mana_regen;
} t_stats;

typedef struct s_thread_queue {
	void (*fun)(void *);
	void *arg;
} t_thread_queue;

typedef struct s_entity {
	unsigned int textureId;
	Vector2 pos;
	Vector2 topos;
	Vector2 dim;
	Vector2 animationframe;
	entity_type_e type;
	char y;
	bool detected_player;
	float damage;
	float life;
	float max_life;
}	t_entity;

typedef struct s_player {
	Vector3 pos;
	Vector3 to;
	char dir;
	player_status_e	status;
	player_action_e action;
	unsigned int lvl;
	float xp;
	INVENTORY inv;
	t_stats stats;
	Rectangle hitbox;
	Rectangle frame;
	std::string name;
	int skin;
	int animation_idx;
} t_player;

typedef struct s_level {
	int *terrain;
	int *wall;
	int *event;
	Vector3 dimension;
	char *filename;
	Texture2D tile_texture;
	char *tileset;
} t_level;

typedef struct s_thread_handle {
	std::thread th;
	std::mutex mtx;
	bool available = true;
} t_thread_handle;

typedef struct s_animation {
	u32 max_frame;
	int texture_idx;
	u32 current_frame;
	u32 frame_idx;
	double frame_time;
	frame_loop_e loop_type;
	double max_time;
	int incr = 1;
	Vector3 pos;
} t_animation;

typedef struct s_texture{
	std::vector<Texture2D> hero;
	std::vector<Texture2D> env;
	std::vector<Texture2D> item;
	std::vector<Texture2D> mob;
	std::vector<Texture2D> ui;
}t_textures;

typedef struct s_engine {
	std::atomic_int status;
	Font font;
	Camera2D camera;
	RenderTexture fbo;
	std::vector<t_player> players;
	t_player *current_save;
	t_textures *textures;
	std::vector<t_level> levels;
	t_input	input[MAX_INPUT];
	std::vector<t_animation> animation_queue;
	u32 level_idx;
} t_engine;

void renderMenu(void);
void renderOnline(void);
void renderSolo(void);
void renderSave(void);
void renderSetting(void);

bool IsMouseInBound(Rectangle bound, Vector2 pos, Vector2 mousePos);
Vector2 getBlockPos(Vector2 pos);
bool isWall(Vector2 pos, const t_level &level);
Rectangle GetNearestWallBound(Vector2 pos, const t_level &level, Rectangle *bound);
void CorrectWallCollision(t_player *player, const t_level &level, Rectangle *collision);

void travelTarget(Vector2 *current, const Vector2 target, const f32 velocity, const f32 deltaTime);
bool IsInBond(Vector2 pos, Vector2 hi, Vector2 low);

t_player defaultPlayerInit(const Vector3 spawn);
std::vector<t_player> loadAllSave(void);
void savePlayerData(t_player player);

const u32 getLinearIndex(const float x, const float y, const int width);
const Vector2 getVector2Pos(const u32 index, const int width);

std::vector<t_level> loadAllLevel(void);
void freeLevel(t_level *level);
t_textures *loadAllTexture();
void drawLevel(t_level &level);
void loadInput(t_input *inputlist);

void updateInput(void);
int updatePlayer(void);

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
