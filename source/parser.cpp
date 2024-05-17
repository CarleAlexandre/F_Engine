# include <engine.hpp>
# include <cstring>

enum TOKEN_IDENTIFIER {
	TOKEN_CMD		= 1,
	TOKEN_ARG		= 2,
	TOKEN_CONTEXT	= 3,
	TOKEN_INT		= 4,
};

typedef struct s_token {
	std::string	str;
	uint32_t	identifier;
	bool		isNewLine;
} t_token;

std::vector<t_token> tokenizer(std::string &str, char *delim) {
	std::vector<t_token> token_list;
	t_token tok;
	const char *tmp;

	for (size_t i = 0; i < str.size(); i++) {
		tmp = strstr(str.c_str(), delim);
		for (int k = 0; str.c_str() + k < tmp; k++) {
			tok.str += *(str.c_str() + k);
			i++;
		}
		token_list.push_back(tok);
	}
	return (token_list);
}


