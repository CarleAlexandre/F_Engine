#ifndef PLAYER_HPP
# define PLAYER_HPP

#include "inventory.hpp"
#include <raymath.h>
#include <unordered_map>

std::unordered_map<std::string, player_token_e> player_dictionnary{
	{"status", player_token_status},
	{"lvl", player_token_lvl},
	{"xp", player_token_xp},
	{"move_speed", player_token_move_speed},
	{"crit_chance", player_token_crit_chance},
	{"crit_dmg", player_token_crit_dmg},
	{"raw_dmg", player_token_raw_dmg},
	{"dmg_reduction", player_token_dmg_reduction},
	{"armor", player_token_armor},
	{"attack_speed", player_token_attack_speed},
	{"life_steal", player_token_life_steal},
	{"mana", player_token_mana},
	{"magic_affinity", player_token_magic_affinity},
	{"life", player_token_life},
	{"max_life", player_token_max_life},
	{"health_regen", player_token_health_regen},
	{"mana_regen", player_token_mana_regen},
	{"name", player_token_name},
	{"skin", player_token_skin},
};


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

class PLAYER {
	private:
		std::string name;
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
		void updateInput(void) {
			for (int i = 0; i < MAX_INPUT; i++) {
				if (engine.input[i].ismouse && IsMouseButtonDown(engine.input[i].key)) {
					switch(engine.input[i].id){
						case(move):{
							Vector2 to;
							to = GetScreenToWorld2D(GetMousePosition(), engine.camera);
							engine.current_save->to.x = to.x - 16;
							engine.current_save->to.z = to.y - 24;
							break;
						}
						case(autoattack):{
							break;
						}
						default:
						break;
					}
				}
				if (!engine.input[i].ismouse && IsKeyDown(engine.input[i].key)) {
					static double interval = 0.1;
					interval += GetFrameTime();
					switch(engine.input[i].id){
						case(interact):{
							break;
						}
						case(hotbar1):{
							engine.current_save->inv.tool_bar.getPrev();
							break;
						}
						case(hotbar2):{
							break;
						}
						case(hotbar3):{
							engine.current_save->inv.tool_bar.getNext();
							break;
						}
						case(hotbar4):{
							break;
						}
						case(hotbar5):{
							break;
						}
						case(hotbar6):{
							break;
						}
						case(toggleinventory):{
							if (interval >= 0.1) {
								engine.current_save->inv.toggle();
								interval = 0;
							}
							break;
						}
						case(centercamera):{
							engine.camera.target = {engine.current_save->pos.x + 16, engine.current_save->pos.z + 24};
							break;
						}
						default:
						break;
					}
				}
			}
		}

		int update(void) {
			static player_action_e last_action = player_action_default;
			int ret = 0;
			Vector2 from = {engine.current_save->pos.x, engine.current_save->pos.z};
			Vector2 to = {engine.current_save->to.x, engine.current_save->to.z};

			if (Vector2Distance(from, to) > 0.1) {
				travelTarget(&from, to, engine.current_save->stats.move_speed, GetFrameTime());
				engine.current_save->pos.x = from.x;
				engine.current_save->pos.z = from.y;
				if (last_action != player_action_moving) {
					engine.current_save->action = player_action_moving;
					last_action = engine.current_save->action;
					ret = 1;
				}
			} else {
				if (last_action != player_action_idle) {
					engine.current_save->action = player_action_idle;
					last_action = engine.current_save->action;
					ret = 1;
				}
			}
			engine.current_save->inv.tool_bar.update();
			return (ret);
		}

		void loadSave(const char *savepath) {
			if (savepath == 0) {
				return (ret);
			}
			char *player_data = readFile(savepath);
			
			std::vector<t_token> token = tokenizer(player_data, ",\n", 2, player_dictionnary);

			if (token.size() == 0){
		#ifdef DEBUG 
			std::cout << "Player file Parse Error: " << savepath << ".!\n";
		#endif
				MemFree(player_data);
				return (ret);
			}
			for (int i = 0; i < token.size(); i++) {
				switch (token[i].identifier) {
					case (player_token_move_speed):{
						stats.move_speed = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_crit_chance):{
						stats.crit_chance = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_crit_dmg):{
						stats.crit_dmg = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_raw_dmg):{
						stats.raw_dmg = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_dmg_reduction):{
						stats.dmg_reduction = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_armor):{
						stats.armor = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_attack_speed):{
						stats.attack_speed = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_life_steal):{
						stats.life_steal = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_mana):{
						stats.mana = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_magic_affinity):{
						stats.magic_affinity = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_life):{
						stats.life = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_max_life):{
						stats.max_life = std::atoi(token[i].value.c_str());
						break;}
					case (player_token_health_regen):{
						stats.health_regen = std::atoi(token[i].value.c_str());
						break;
					}
					case (player_token_mana_regen):{
						stats.mana_regen = std::atoi(token[i].value.c_str());
						break;
					}
					case (player_token_status):{
						status = (player_status_e)std::atoi(token[i].value.c_str());
						break;
					}
					case (player_token_lvl):{
						lvl = std::atoi(token[i].value.c_str());
						break;
					}
					case (player_token_xp):{
						xp = std::atof(token[i].value.c_str());
						break;
					}
					case (player_token_name): {
						name = token[i].value;
						break;
					}
					case (player_token_skin): {
						ret.skin = atoi(token[i].value.c_str());
		#ifdef DEBUG
								std::cout << ret.skin << "\n";
		#endif
						break;
					}
					default:
		#ifdef DEBUG
						std::cerr << "Save File Is Corrupted!\n";
		#endif
						break;
				}
			}
			clearToken(token);
			MemFree(player_data);
		}

		//if idx =0 then it's a new save
		void savePlayerData() {
			std::stringstream data;

			data << "name:" << name.c_str() \
				<< ",\nlvl:" << lvl \
				<< ",\nxp:" << xp \
				<< ",\nstatus:" << status \
				<< ",\narmor:" << stats.armor \
				<< ",\nattack_speed:" << stats.attack_speed \
				<< ",\nmove_speed:" << stats.move_speed \
				<< ",\ncrit_chance:" << stats.crit_chance \
				<< ",\ncrit_dmg:" << stats.crit_dmg \
				<< ",\ndmg_reduction:" << stats.dmg_reduction \
				<< ",\nhealth_regen:" << stats.health_regen \
				<< ",\nmana_regen:" << stats.mana_regen \
				<< ",\nmax_life:" << stats.max_life \
				<< ",\nlife:" << stats.life \
				<< ",\nmana:" << stats.mana \
				<< ",\nraw_dmg:" << stats.raw_dmg \
				<< ",\nmagic_affinity:" << stats.magic_affinity \
				<< ",\nlife_steal:" << stats.life_steal \
				<< ",\nskin:" << skin << ",\n"\
				;
		#ifdef DEBUG
								std::cout << skin << "\n";
		#endif
			writeFile(TextFormat("save/%s.player", name.c_str()), data.str().c_str(), data.str().size());
		}

	PLAYER(Vector2 spawn) {
		t_player default_player = (t_player){
			.pos = spawn,
			.dir = NORTH,
			.status = player_status_well,
			.lvl = 1,
			.xp = 0.0f,
			.inv = INVENTORY(),
			.stats = {
				.move_speed = 10,
				.crit_chance = 0.0f,
				.crit_dmg = 1.2f,
				.raw_dmg = 0.0f,
				.dmg_reduction = 0.0f,
				.armor = 30,
				.attack_speed = 0.6f,
				.life_steal = 0.0f,
				.mana = 300,
				.magic_affinity = 1.0f,
				.life = 600,
				.max_life = 600,
			},
			.hitbox = {spawn.x + 6, spawn.y - 8, 12, 12},
			.frame = (Rectangle){0, 0, 32, 32},
			.name = "default",
			.animation_idx = 0,
		};
		return (default_player);
	}
	~PLAYER();
};

#endif