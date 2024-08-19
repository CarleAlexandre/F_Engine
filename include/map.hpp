#ifndef MAP_HPP
# define MAP_HPP

#include <vector>

typedef struct s_chunk {
	char terrain[16][16];
	char structure[16][16];
	char event[16][16];
	char light_level[16][16];
} t_chunk;

enum tile_type{
	tile_dirt = 0,
	tile_grass = 1,
};

class MAP {
	private:
		int textureID;
		int *chunk_to_render;
		std::vector<t_chunk> chunks;
	public:
		int getChunk() {
			return(0);
		}
		char get_tile(char square[2][2]) {
			if (square[0][0] == tile_grass) {
				if (square[0][1] == tile_grass) {
					if (square[1][0] == tile_grass) {
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}
				} else {
					if (square[1][0] == tile_grass) {
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}
				}
			} else {
				if (square[0][1] == tile_grass) {
					if (square[1][0] == tile_grass) {
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}
				} else {
					if (square[1][0] == tile_grass) { 
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(1);
						} else {
							return(1);
						}
					}
				}
			}
		}
		void render(){
			Vector2 beg = GetScreenToWorld2D({0,0}, engine. camera);
			Vector2 end = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, engine. camera);

			//for (float y = beg.y; y < level.dimension.y && y < end.y; y ++) {
			//	for (float z = 0; z < level.dimension.z; z += 32){
			//		for (float x = beg.x; x < level.dimension.x && x < end.x; x += 32) {
			//			DrawTextureRec(engine.textures.env[level.tileset], {(float)level.terrain[linearIndexFromCoordinate({x, y, z}, level.dimension.x, level.dimension.z)], }, {x, z}, WHITE);
			//		}
			//	}
			//}
		}
		void logic(){}
};

#endif