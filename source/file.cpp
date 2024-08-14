#include <cstdlib>
#include <engine.hpp>
#include <fstream>
#include <raylib.h>
#include <raymath.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include <unordered_map>

enum level_token_e {
	level_token_width	= 1,
	level_token_height	= 2,
	level_token_terrain	= 3,
	level_token_wall	= 4,
	level_token_event	= 5,
	level_token_depth	= 6,
	level_token_tileset	= 7,
};

std::unordered_map<std::string, level_token_e> level_dictionnary{
	{"width", level_token_width},
	{"height", level_token_height},
	{"terrain", level_token_terrain},
	{"wall", level_token_wall},
	{"event", level_token_event},
	{"depth", level_token_depth},
	{"tileset", level_token_tileset},
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
	{"name", player_token_name},
	{"skin", player_token_skin},
};

std::unordered_map<std::string, player_input_e> input_dictionnary{
	{"move", move},
	{"autoattack", autoattack},
	{"interact", interact},
	{"hotbar1", hotbar1},
	{"hotbar2", hotbar2},
	{"hotbar3", hotbar3},
	{"hotbar4", hotbar4},
	{"hotbar5", hotbar5},
	{"hotbar6", hotbar6},
	{"toggleinventory", toggleinventory},
	{"centercamera", centercamera},
};

void writeToLevel(t_level level) {
	std::fstream output;

	output.open(TextFormat("level/%s.map", level.filename), std::ios_base::out | std::ios_base::trunc);

	std::stringstream terrain;
	std::stringstream wall;
	std::stringstream event;
	std::stringstream dim;
	dim << "width:"<< level.dimension.x << ",\nheight:"<< level.dimension.y << ",\ndepth:" << level.dimension.z << ",\ntileset:" << level.tileset << ",\n";
	terrain << "terrain:{\n";
	for (int k = 0; k < level.dimension.y;) {
		for (int i = 0; i < level.dimension.z; i++) {
			for (int j = 0; j < level.dimension.x; j++)
			terrain << level.terrain[linearIndexFromCoordinate({(float)j, (float)i, (float)k}, level.dimension.z, level.dimension.y)] << ", ";
		}
		k++;
		if (k < level.dimension.y)
			terrain << "\n;\n";
	}
	terrain << "\n},\n";
	wall << "wall:{\n";
	event << "event:{\n";
	for (int i = 0; i < level.dimension.x * level.dimension.z; i++) {
		wall << level.wall[i] << ", ";
		event << level.event[i] << ", ";
	}
	wall << "\n},\n";
	event << "\n},\n";
	output << dim.rdbuf() << wall.rdbuf() << event.rdbuf() << terrain.rdbuf();
}

t_level loadLevel(const char *level_name) {
	t_level level = {};
	bool alloc = false;
# ifdef DEBUG
	int step = 0;
# endif
	#ifdef __linux__
	level.filename = strdup(level_name);
	#else
	level.filename = _strdup(level_name);
	#endif
	char *level_data = readFile(level_name);

	if (!level_data) {
#ifdef DEBUG
		std::cout << "COULD NOT READ FILE : " << level.filename << "\n";
#endif
		abort();
	}
	std::vector<t_token> token = tokenizer(level_data, ",\n", 2, level_dictionnary);
	if (!token.size()) {
		#ifdef DEBUG 
			std::cout << "level file Parse Error: " << __LINE__ << " name: " << level_name << ".!\n";
		#endif
		MemFree(level_data);
		abort();
	}
	for (u32 i = 0; i < token.size(); i++) {
		switch (token[i].identifier) {
			case(level_token_width):{
				level.dimension.x = atoi(token[i].value.c_str());
				break;
			}
			case(level_token_height):{
				level.dimension.y = atoi(token[i].value.c_str());
				break;
			}
			case(level_token_depth):{
				level.dimension.z = atoi(token[i].value.c_str());
				break;
			}
			case(level_token_tileset):{
#ifdef __linux__
				level.tileset = strdup(token[i].value.c_str());
#else
				level.tileset = _strdup(token[i].value.c_str());
#endif
				break;
			}
			case(level_token_terrain):{
				for (int layer = 0; layer < level.dimension.y; layer++) {
					const char *layer_tmp = getNextDelim(token[i].value.c_str(), ";", token[i].value.size(), 1);
					if (!layer_tmp){break;}
					std::cout << layer_tmp << "\n";
					for (int k = 0; k < level.dimension.x * level.dimension.z; k++) {
						const char *tmp = stringSpliter(token[i].value.c_str(), ", ", layer_tmp - token[i].value.c_str(), 2);
						if (!tmp){break;}
						level.terrain[layer + (int)(k * level.dimension.y)] = atoi(tmp);
					}
				}
				break;
			}
			case(level_token_wall):{
				for (int k = 0; k < level.dimension.x * level.dimension.y; k++) {
					const char *tmp = stringSpliter(token[i].value.c_str(), ", ", token[i].value.size(), 2);
					if (!tmp){break;}
					level.wall[k] = atoi(tmp);
				}
				break;
			}
			case(level_token_event):{
				for (int k = 0; k < level.dimension.x * level.dimension.y; k++) {
					const char *tmp = stringSpliter(token[i].value.c_str(), ", ", token[i].value.size(), 2);
					if (!tmp){break;}
					level.event[k] = atoi(tmp);
				}
				break;
			}
			default:break;
		};
		if (level.dimension.x && level.dimension.z && level.dimension.y && !alloc) {
			int size = level.dimension.x * level.dimension.z;
			level.terrain = (int *)MemAlloc(size * level.dimension.y * sizeof(int));
			level.event = (int *)MemAlloc(size * sizeof(int));
			level.wall = (int *)MemAlloc(size * sizeof(int));
# ifdef DEBUG
			std::cout << "alloc chunk size level: " << level_name << "size: " << size * 3 + size * level.dimension.y << "\n";
# endif
			alloc = true;
		}
# ifdef DEBUG
		step++;
# endif
	}
	if (level.dimension.x == 0) {
		clearToken(token);
		MemFree(level_data);
		abort();
	}
#ifdef DEBUG
		std::cout << "couldn't load level, token error : " << __LINE__ << ", step : " << step << "\n";
#endif
	clearToken(token);
	MemFree(level_data);
	return (level);
}

std::vector<t_level> loadAllLevel(void) {
	FilePathList level_path;
	std::vector<t_level> levels;

	level_path = LoadDirectoryFiles(GetDirectoryPath("assets/maps/"));

	for (int i = 0; i < level_path.count; i++) {
		levels.push_back(loadLevel(level_path.paths[i]));
	}
	UnloadDirectoryFiles(level_path);
	return (levels);
}

t_player loadPlayerSave(const char *savepath) {
	t_player ret = defaultPlayerInit(Vector3Zero());

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
				ret.status = (player_status_e)std::atoi(token[i].value.c_str());
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
			case (player_token_name): {
				ret.name = token[i].value;
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
	return (ret);
}

std::vector<t_player> loadAllSave(void) {
	std::vector<t_player> player_data;

	FilePathList save_directory = LoadDirectoryFiles(GetDirectoryPath("assets/save/"));

	for (int i = 0; i < save_directory.count; i++) {
		player_data.push_back(loadPlayerSave(save_directory.paths[i]));	
	}
	return (player_data);
}

//if idx =0 then it's a new save
void savePlayerData(t_player player) {
	std::stringstream data;

	data << "name:" << player.name.c_str() \
		<< ",\nlvl:" << player.lvl \
		<< ",\nxp:" << player.xp \
		<< ",\nstatus:" << player.status \
		<< ",\narmor:" << player.stats.armor \
		<< ",\nattack_speed:" << player.stats.attack_speed \
		<< ",\nmove_speed:" << player.stats.move_speed \
		<< ",\ncrit_chance:" << player.stats.crit_chance \
		<< ",\ncrit_dmg:" << player.stats.crit_dmg \
		<< ",\ndmg_reduction:" << player.stats.dmg_reduction \
		<< ",\nhealth_regen:" << player.stats.health_regen \
		<< ",\nmana_regen:" << player.stats.mana_regen \
		<< ",\nmax_life:" << player.stats.max_life \
		<< ",\nlife:" << player.stats.life \
		<< ",\nmana:" << player.stats.mana \
		<< ",\nraw_dmg:" << player.stats.raw_dmg \
		<< ",\nmagic_affinity:" << player.stats.magic_affinity \
		<< ",\nlife_steal:" << player.stats.life_steal \
		<< ",\nskin:" << player.skin << ",\n"\
		;
#ifdef DEBUG
						std::cout << player.skin << "\n";
#endif
	writeFile(TextFormat("save/%s.player", player.name.c_str()), data.str().c_str(), data.str().size());
}

t_textures loadAllTexture(std::unordered_map<std::string, int> &texture_dictionnary) {
	FilePathList textures_directory;
	t_textures textures;

	textures_directory = LoadDirectoryFiles(GetDirectoryPath("assets/textures/env"));
	for (int i = 0; i < textures_directory.count; i++) {
		textures.env.push_back(LoadTexture(textures_directory.paths[i]));
		texture_dictionnary.emplace(GetFileNameWithoutExt(textures_directory.paths[i]), i);
	}
	UnloadDirectoryFiles(textures_directory);

	textures_directory = LoadDirectoryFiles(GetDirectoryPath("assets/textures/hero"));
	for (int i = 0; i < textures_directory.count; i++) {
		textures.hero.push_back(LoadTexture(textures_directory.paths[i]));
		texture_dictionnary.emplace(GetFileNameWithoutExt(textures_directory.paths[i]), i);
	}
	UnloadDirectoryFiles(textures_directory);

	textures_directory = LoadDirectoryFiles(GetDirectoryPath("assets/textures/item"));
	for (int i = 0; i < textures_directory.count; i++) {
		textures.item.push_back(LoadTexture(textures_directory.paths[i]));
		texture_dictionnary.emplace(GetFileNameWithoutExt(textures_directory.paths[i]), i);
	}
	UnloadDirectoryFiles(textures_directory);

	textures_directory = LoadDirectoryFiles(GetDirectoryPath("assets/textures/mob"));
	for (int i = 0; i < textures_directory.count; i++) {
		textures.mob.push_back(LoadTexture(textures_directory.paths[i]));
		texture_dictionnary.emplace(GetFileNameWithoutExt(textures_directory.paths[i]), i);
	}
	UnloadDirectoryFiles(textures_directory);

	textures_directory = LoadDirectoryFiles(GetDirectoryPath("assets/textures/ui"));
	for (int i = 0; i < textures_directory.count; i++) {
		textures.ui.push_back(LoadTexture(textures_directory.paths[i]));
		texture_dictionnary.emplace(GetFileNameWithoutExt(textures_directory.paths[i]), i);
	}
	UnloadDirectoryFiles(textures_directory);

	return (textures);
}

void loadInput(t_input *inputlist) {
	char *inputfile = readFile("assets/config/input.cfg");
	u32 current_input_idx = 0;

	if (!inputfile){return;}
	std::vector<t_token> inputtoken = tokenizer(inputfile, ";", 1, input_dictionnary);

	if (!inputtoken.size()){return;}
	for (int i = 0; i < inputtoken.size(); i++) {
		eraseFromString(inputtoken[i].value, "{}\n", 3);
		const char *nextdelim = getNextDelim(inputtoken[i].value.c_str(), ",", inputtoken[i].value.size(), 1);
		inputlist[inputtoken[i].identifier - 1].key = atoi(inputtoken[i].value.c_str() + 4);
		inputlist[inputtoken[i].identifier - 1].ismouse = (strncmp(nextdelim + 9, "true", 4) == 0) ? true : false;
		inputlist[inputtoken[i].identifier - 1].id = input_dictionnary[inputtoken[i].key];
#ifdef DEBUG
		std::cout << "key: " << inputlist[inputtoken[i].identifier - 1].key << " ismouse: " << inputlist[inputtoken[i].identifier - 1].ismouse << " id: " <<  inputlist[inputtoken[i].identifier - 1].id << "\n";
#endif
	}
	clearToken(inputtoken);

	MemFree(inputfile);
	inputtoken.clear();
}
