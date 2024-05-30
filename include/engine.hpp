#ifndef ENGINE_HPP
# define ENGINE_HPP

# define GLSL_VERSION 330
# include <raylib.h>
# include <rlgl.h>
# include <raymath.h>

# include <atomic>
# include <thread>
# include <mutex>

# include <vector>
# include <unordered_map>

# ifdef DEBUG
	# include <iostream>
#endif

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

enum entity_type_e {
	entity_type_mobs,
	entity_type_bullet,
	entity_type_effect,
	entity_type_gathering,
	entity_type_plant,
};

enum item_type_e {
	item_type_weapon,
	item_type_tool,
	item_type_material,
	item_type_structure,
	item_type_machine,
	item_type_useable,
};

enum engine_status_e {
	engine_status_menu = 0,
	engine_status_close = 1,
	engine_status_online = 2,
	engine_status_solo = 3,
	engine_status_setting = 4,
	engine_status_save = 5,
};

enum player_status_e {
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
};

enum diraction_e {
	SOUTH	= 1 << 0,
	NORTH	= 1 << 1,
	EAST	= 1 << 2,
	WEST	= 1 << 3,
};

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

typedef struct s_item {
	item_type_e id;
	unsigned int texture_index;
} t_item;

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
	Vector2 pos;
	char dir;
	int	status;
	unsigned int lvl;
	float xp;
	char y;
	std::vector<t_item> inventory;
	u32 inventory_size;
	t_stats stats;
	Rectangle hitbox;
	Rectangle frame;
	std::string name;
	int skin;
} t_player;


typedef struct s_level {
	int *terrain;
	int *wall;
	int *event;
	Vector2 dimension;
} t_level;

typedef struct s_thread_handle {
	std::thread th;
	std::mutex mtx;
	bool available = true;
} t_thread_handle;

typedef struct s_token {
	std::string	key;
	std::string value;
	uint32_t	identifier;
} t_token;

typedef struct s_engine {
	std::atomic_int height, width, status;
	Font font;
	Camera2D camera;
	RenderTexture fbo;
	std::vector<t_player> players;
	t_player *current_save;
	std::vector<Texture2D> textures;
	std::vector<t_level> levels;
	std::unordered_map<std::string, int> texture_dictionnary;
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

i32 gcd(i32 a, i32 b);
f32 rsqrt(f32 num);
f64 pow(f64 number, long power);
f32 smoothStep(f32 t);
f32 clamp(f32 value, f32 lo, f32 hi);
void travelTarget(Vector2 *current, const Vector2 target, const f32 velocity, const f32 deltaTime);
bool IsInBond(Vector2 pos, Vector2 hi, Vector2 low);

t_player defaultPlayerInit(const Vector3 spawn);
std::vector<t_player> loadAllSave(void);
void savePlayerData(t_player player);

const float getXpos(const u32 idx, const int width);
const float getYpos(const u32 idx, const int width);
const u32 getLinearIndex(const float x, const float y, const int width);
const Vector2 getVector2Pos(const u32 index, const int width);

std::vector<t_level> loadAllLevel(void);
void freeLevel(t_level *level);
std::vector<Texture2D> loadAllTexture(std::unordered_map<std::string, int> &texture_dictionnary);
void drawLevel(t_level &level);

#endif
