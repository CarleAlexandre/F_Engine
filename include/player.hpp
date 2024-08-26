#ifndef PLAYER_HPP
# define PLAYER_HPP

#include "inventory.hpp"

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
	move			= 1,
	autoattack		= 2,
	interact		= 3,
	hotbarprev		= 4,
	hotbaruse		= 5,
	hotbarnext		= 6,
	hotbar4			= 7,
	hotbar5			= 8,
	hotbar6			= 9,
	toggleinventory	= 10,
	centercamera	= 11,
} player_input_e;

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

std::unordered_map<std::string, player_input_e> input_dictionnary{
	{"move", move},
	{"autoattack", autoattack},
	{"interact", interact},
	{"hotbarprev", hotbarprev},
	{"hotbaruse", hotbaruse},
	{"hotbarnext", hotbarnext},
	{"hotbar4", hotbar4},
	{"hotbar5", hotbar5},
	{"hotbar6", hotbar6},
	{"toggleinventory", toggleinventory},
	{"centercamera", centercamera},
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

typedef struct s_input {
	int key;
	bool ismouse;
	player_input_e id;
}t_input;

class PLAYER {
	private:
		std::string name;
		Vector2 to;
		unsigned int lvl;
		float xp;
		player_status_e	status;
		t_stats stats;
		Rectangle hitbox;
		Rectangle frame;
		t_input	input[MAX_INPUT];
	public:
		player_action_e action;
		Vector2 pos;
		INVENTORY inv;
		int skin;
		char dir;
		void updateInput(Camera2D &camera) {
			for (int i = 0; i < MAX_INPUT; i++) {
				if (input[i].ismouse && IsMouseButtonDown(input[i].key)) {
					switch(input[i].id){
						case(move):{
							Vector2 topos;
							topos = GetScreenToWorld2D(GetMousePosition(), camera);
							to.x = topos.x - 16;
							to.y = topos.y - 24;
							if (to.x > pos.x) {
								dir = EAST;
							} else {
								dir = WEST;
							}
							break;
						}
						case(autoattack):{
							break;
						}
						default:
						break;
					}
				}
				if (!input[i].ismouse && IsKeyDown(input[i].key)) {
					static double interval = 0.1;
					interval += GetFrameTime();
					switch(input[i].id){
						case(interact):{
							break;
						}
						case(hotbarprev):{
							inv.tool_bar.getPrev();
							break;
						}
						case(hotbaruse):{
							break;
						}
						case(hotbarnext):{
							inv.tool_bar.getNext();
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
								inv.toggle();
								interval = 0;
							}
							break;
						}
						case(centercamera):{
							camera.target = {pos.x + 16, pos.y + 24};
							break;
						}
						default:
						break;
					}
				}
			}
		}

		void update(void) {
			static player_action_e last_action = player_action_default;

			if (Vector2Distance(pos, to) > 0.1) {
				pos = Vector2MoveTowards(pos, to, stats.move_speed);
				if (last_action != player_action_moving) {
					action = player_action_moving;
					last_action = action;
				}
			} else {
				if (last_action != player_action_idle) {
					action = player_action_idle;
					last_action = action;
				}
			}
			inv.tool_bar.update();
		}

		void loadSave(const char *savepath) {
			char *player_data = readFile(savepath);
			
			std::vector<t_token> token = tokenizer(player_data, ",\n", 2, player_dictionnary);

			if (token.size() == 0){
				MemFree(player_data);
				return ;
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
						break;
					}
					default:
						break;
				}
			}
			clearToken(token);
			MemFree(player_data);
		}

		void loadInput() {
			char *inputfile = readFile("assets/config/input.cfg");
			unsigned int current_input_idx = 0;

			if (!inputfile){return;}
			std::vector<t_token> inputtoken = tokenizer(inputfile, ";", 1, input_dictionnary);

			if (!inputtoken.size()){return;}
			for (int i = 0; i < inputtoken.size(); i++) {
				eraseFromString(inputtoken[i].value, "{}\n", 3);
				const char *nextdelim = getNextDelim(inputtoken[i].value.c_str(), ",", inputtoken[i].value.size(), 1);
				input[inputtoken[i].identifier - 1].key = atoi(inputtoken[i].value.c_str() + 4);
				input[inputtoken[i].identifier - 1].ismouse = (strncmp(nextdelim + 9, "true", 4) == 0) ? true : false;
				input[inputtoken[i].identifier - 1].id = input_dictionnary[inputtoken[i].key];
			}
			clearToken(inputtoken);
			MemFree(inputfile);
			inputtoken.clear();
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
				<< ",\nskin:" << ",\n"\
				;
			writeFile(TextFormat("save/%s.player", name.c_str()), data.str().c_str(), data.str().size());
		}

	PLAYER(Vector2 spawn) {
		pos = spawn;
		to = spawn;
		dir = NORTH;
		action = player_action_default;
		status = player_status_well;
		lvl = 1;
		xp = 0.0f;
		inv = INVENTORY();
		stats = {
			.move_speed = 0.6,
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
		};
		hitbox = {spawn.x + 6, spawn.y - 8, 12, 12};
		frame = (Rectangle){0, 0, 32, 32};
		name = "default";
		loadInput();
	}
	~PLAYER(){};
};

#endif