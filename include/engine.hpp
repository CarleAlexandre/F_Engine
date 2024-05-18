#ifndef ENGINE_HPP
# define ENGINE_HPP

# define GLSL_VERSION 330
# include <raylib.h>
# include <rlgl.h>
# include <raymath.h>
# include <vector>
# include <atomic>

# include <thread>
# include <mutex>


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
bool IsMouseInBound(Rectangle bound, Vector2 pos, Vector2 mousePos);



class Button {
	private:
	Rectangle	boundaries;
	Vector2		pos;
	std::string text;
	void	*(*fun)(void *arg);
	public:

	void ButtonLogic(int n, Vector2 mouse_pos, void *arg) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			for (int i = 0; i < n; i++) {
				if (IsMouseInBound(boundaries, pos, mouse_pos)) {
					fun(arg);
				}
			}
		}
	}

	void ButtonRender(int n, const Font font, std::vector<Texture2D> &textAtlas, const Vector2 mousePos) {
		for (u32 k = 0; k < n; k++) {
			if (IsMouseInBound(boundaries, pos, mousePos)) {
				DrawTextureRec(textAtlas[texture_index_ui], boundaries, pos, WHITE);
			} else {
				DrawTextureRec(textAtlas[texture_index_ui], boundaries, pos, WHITE);
			}
			DrawTextEx(font, text.c_str(),
					(Vector2){pos.x + 10, static_cast<float>(
							pos.y + boundaries.height * 0.5 - 6
							)}, 22, 0, WHITE);
		}
	}

	Button(){}
	~Button(){}
};

#endif
