#ifndef ENGINE_HPP
# define ENGINE_HPP

# define GLSL_VERSION 330
# include <raylib.h>
# include <rlgl.h>
# include <vector>
# include <atomic>

# include <thread>
# include <mutex>
# include <condition_variable>
# include <queue>

# ifdef DEBUG
	# include <iostream>
#endif

enum texture_index_e {
	texture_index_ui = 1,
	texture_index_character = 2,
	texture_index_mods = 3,
	texture_index_blocks = 4,
	texture_index_effect = 5,
	texture_index_entity = 6,
};

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
	float x, y, z;
	char dir;
	int	status;
	unsigned int lvl;
	float xp;
	std::vector<t_item> inventory[36];
	t_stats stats;
	float life;
	float max_life;
} t_player;

typedef struct s_engine {
	std::atomic_int height, width, status;
	Font font;
	Camera2D camera;
	RenderTexture fbo;
} t_engine;

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

void renderMenu(void);
void renderOnline(void);
void renderSolo(void);


#endif
