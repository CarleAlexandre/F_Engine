#include <cstdlib>
#include <engine.hpp>
#include <fstream>
#include <raylib.h>
#include <raymath.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include <unordered_map>

enum TOKEN_IDENTIFIER {
	TOKEN_CMD		= 1,
	TOKEN_ARG		= 2,
	TOKEN_CONTEXT	= 3,
	TOKEN_INT		= 4,
};

enum player_token_e {
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
};

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
};

typedef struct s_token {
	std::string	key;
	std::string value;
	uint32_t	identifier;
} t_token;

char *readFile(const char *filepath) {
	std::ifstream file;
	std::stringstream str;
	char *ret;

	if (FileExists(filepath)) {
		file.open(filepath);
		str << file.rdbuf();
		ret = (char *)MemAlloc(str.str().size() + 1);
		ret[str.str().size()] = 0;
		str.str().copy(ret, str.str().size());
		str.clear();
		//need to free returned value after use;
		file.close();
		return (ret);
	}
	return (NULL);
}

void writeFile(const char *filepath, const char *data, size_t n) {
	std::ofstream file;

	if (FileExists(filepath)) {
		file.open(filepath);
		file.write(data, n);
		file.close();
	}
}

template <typename T>
std::vector<t_token> tokenizer(std::string str, const char *delim, std::unordered_map<std::string, T> &dictionnary) {
	std::vector<t_token> token_list;
	t_token tok;
	const char *tmp;

	for (size_t i = 0; i < str.size(); i++) {
		tmp = strstr(str.c_str(), delim);
		for (int k = 0; str.c_str() + k < tmp; k++) {
			tok.key += *(str.c_str() + k);
			i++;
		}
		tok.identifier = dictionnary[tok.key];
		token_list.push_back(tok);
	}
	return (token_list);
}

t_player loadPlayerSave(u32 slotIdx) {
	t_player ret = defaultPlayerInit(Vector3Zero());
	if (slotIdx == 0) {
		return (ret);
	}
	char *player_data = readFile(TextFormat("save/player/%i.player", slotIdx));
	
	std::vector<t_token> token = tokenizer(player_data, "\n", player_dictionnary);

	for (int i = 0; i < token.size(); i++) {
		switch (token[i].identifier) {
			case (player_token_move_speed):{
				ret.stats.move_speed = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_crit_chance):{
				ret.stats.crit_chance = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_crit_dmg):{
				ret.stats.crit_dmg = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_raw_dmg):{
				ret.stats.raw_dmg = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_dmg_reduction):{
				ret.stats.dmg_reduction = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_armor):{
				ret.stats.armor = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_attack_speed):{
				ret.stats.attack_speed = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_life_steal):{
				ret.stats.life_steal = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_mana):{
				ret.stats.mana = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_magic_affinity):{
				ret.stats.magic_affinity = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_life):{
				ret.stats.life = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_max_life):{
				ret.stats.max_life = std::atoi(token[i].value.c_str());
				break;}
			case (player_token_health_regen):{
				ret.stats.health_regen = std::atoi(token[i].value.c_str());
				break;
			}
			case (player_token_mana_regen):{
				ret.stats.mana_regen = std::atoi(token[i].value.c_str());
				break;
			}
			case (player_token_status):{
				ret.status = std::atoi(token[i].value.c_str());
				break;
			}
			case (player_token_lvl):{
				ret.lvl = std::atoi(token[i].value.c_str());
				break;
			}
			case (player_token_xp):{
				ret.xp = std::atof(token[i].value.c_str());
				break;
			}
			default:
				std::cerr << "Save File Is Corrupted!\n";
				break;
		}
	}
	MemFree(player_data);
	return (ret);
}

void savePlayerData(t_player player, u32 slotIdx) {
	if (slotIdx == 0) {
		return;
	}
	std::stringstream data;

	data << "lvl:" << player.lvl \
		<< "\nxp:" << player.xp \
		<< "\nstatus:" << player.status \
		<< "\narmor:" << player.stats.armor \
		<< "\nattack_speed:" << player.stats.attack_speed \
		<< "\nmove_speed:" << player.stats.move_speed \
		<< "\ncrit_chance:" << player.stats.crit_chance \
		<< "\ncrit_dmg:" << player.stats.crit_dmg \
		<< "\ndmg_reduction:" << player.stats.dmg_reduction \
		<< "\nhealth_regen:" << player.stats.health_regen \
		<< "\nmana_regen:" << player.stats.mana_regen \
		<< "\nmax_life:" << player.stats.max_life \
		<< "\nlife:" << player.stats.life \
		<< "\nmana:" << player.stats.mana \
		<< "\nraw_dmg:" << player.stats.raw_dmg \
		<< "\nmagic_affinity:" << player.stats.magic_affinity \
		<< "\nlife_steal:" << player.stats.life_steal \
		;

	writeFile(TextFormat("save/player/%i.player", slotIdx), data.str().c_str(), data.str().size());
}
