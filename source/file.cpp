#include <cstdlib>
#include <engine.hpp>
#include <fstream>
#include <raylib.h>
#include <raymath.h>
#include <sstream>
#include <cstring>
#include <iostream>

enum TOKEN_IDENTIFIER {
	TOKEN_CMD		= 1,
	TOKEN_ARG		= 2,
	TOKEN_CONTEXT	= 3,
	TOKEN_INT		= 4,
};

enum player_token_e {
	player_token_skin	= 0,
	player_token_stats	= 1,
	player_token_status	= 2,
	player_token_lvl	= 3,
	player_token_xp		= 4,
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

std::vector<t_token> tokenizer(std::string str, const char *delim) {
	std::vector<t_token> token_list;
	t_token tok;
	const char *tmp;

	for (size_t i = 0; i < str.size(); i++) {
		tmp = strstr(str.c_str(), delim);
		for (int k = 0; str.c_str() + k < tmp; k++) {
			tok.key += *(str.c_str() + k);
			i++;
		}
		token_list.push_back(tok);
	}
	return (token_list);
}

t_player loadPlayerSave(u32 slotIdx) {
	char *player_data = readFile(TextFormat("save/player/%i.player", slotIdx));
	t_player ret = defaultPlayerInit(Vector3Zero());
	
	std::vector<t_token> token = tokenizer(player_data, "\n");
	
	for (int i = 0; i < token.size(); i++) {
		switch (token[i].identifier) {
			case (player_token_skin):{
				break;
			}
			case (player_token_stats):{
				
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

