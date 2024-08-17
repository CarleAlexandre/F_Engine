#ifndef PLAYER_HPP
# define PLAYER_HPP

#include "inventory.hpp"
#include <raymath.h>

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

t_player defaultPlayerInit(const Vector3 spawn);
std::vector<t_player> loadAllSave(void);
void savePlayerData(t_player player);


class PLAYER {
	private:
		char dir;
		Vector2 pos;
		Vector2 to;
		unsigned int lvl;
		float xp;
		int skin;
		int animation_idx;
		player_status_e	status;
		player_action_e action;
		t_stats stats;
		Rectangle hitbox;
		Rectangle frame;
		INVENTORY inv;
	public:
	PLAYER();
	~PLAYER();
};

#endif