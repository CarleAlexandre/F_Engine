#ifndef MAP_HPP
# define MAP_HPP

#include <vector>

typedef struct s_chunk {
	int terrain[16][16];
	int structure[16][16];
	int event[16][16];
} t_chunk;

class MAP {
	private:
		int textureID;

		std::vector<t_chunk> chunks;
	public:
		int getChunk() {
			return(0);
		}
};

#endif