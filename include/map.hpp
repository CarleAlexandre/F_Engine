#ifndef MAP_HPP
# define MAP_HPP

#include "include.hpp"

typedef struct s_chunk {
	char terrain[16][16];
	char structure[16][16];
	char event[16][16];
} t_chunk;

typedef struct s_render_chunk {
	char light_level[16][16];
	char tex[16][16];
} t_render_chunk;

enum tile_type{
	tile_dirt = 0,
	tile_grass = 1,
};

class MAP {
	private:
		int textureID;
		int *chunk_to_render;
		std::array<t_chunk, 1000000> chunks;
		std::array<t_render_chunk, 1000000> render_chunks;
		char getTileTexIdx(char square[2][2]) {
			if (square[0][0] == tile_grass) {
				if (square[0][1] == tile_grass) {
					if (square[1][0] == tile_grass) {
						if (square[1][1] == tile_grass) {
							return(6);//6
						} else {
							return(7);//7
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(10);//10
						} else {
							return(9);//9
						}
					}
				} else {
					if (square[1][0] == tile_grass) {
						if (square[1][1] == tile_grass) {
							return(2);//2
						} else {
							return(11);//11 
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(4);//4
						} else {
							return(15);//15
						}
					}
				}
			} else {
				if (square[0][1] == tile_grass) {
					if (square[1][0] == tile_grass) {
						if (square[1][1] == tile_grass) {
							return(5);//5
						} else {
							return(14);//14
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(1);//1
						} else {
							return(8);//8
						}
					}
				} else {
					if (square[1][0] == tile_grass) { 
						if (square[1][1] == tile_grass) {
							return(3);//3
						} else {
							return(0);//0
						}
					}else{
						if (square[1][1] == tile_grass) {
							return(13);//13
						} else {
							return(12);//12
						}
					}
				}
			}
			return (-1);
		}

	public:

		void updateChunkTex(int idx) {
			char square [2][2];

			for (int y = 0; y < 16; y++) {
				for (int x = 0; x < 16; x++) {
					square[0][0] = chunks[idx].terrain[y][x];

					if (x == 15 && idx < 999999) {
						square[0][1] = chunks[idx + 1].terrain[y][0];
						square[1][1] = chunks[idx + 1].terrain[y + 1][0];
					} else {
						square[0][1] = chunks[idx].terrain[y][x + 1];
					}

					if (y == 15 && idx < 999000) {
						square[1][0] = chunks[idx + 1000].terrain[0][x];
						square[1][1] = chunks[idx + 1000].terrain[0][x+1];
					} else {
						square[1][0] = chunks[idx].terrain[y + 1][x];
					}

					if (x == 15 && y == 15 && idx < 998999) {
						square[1][1] = chunks[idx + 1001].terrain[0][0];
					}

					if (x != 15 && y != 15) {
						square[1][1] = chunks[idx].terrain[y + 1][x + 1];
					}
					char tmp = getTileTexIdx(square);
					if (tmp == -1) {
						tmp = 0;
					}
					render_chunks[idx].tex[y][x] = tmp;
				}
			}
		}

		t_chunk getWallChunk(Vector2 pos){
			int x = static_cast<int>(pos.x / 512);
        	int y = static_cast<int>(pos.y / 512);

			return (chunks[x + y * 1000]);
		}

		void render(Camera2D camera, ATLAS &atlas) {
			//Vector2 beg = GetScreenToWorld2D({0,0}, camera);

			//get mod of 512 pos beg to know hist relative pos to chunk border
			//same for end

			// int posx, posy;
			// posx = floorf(beg.x / 512);
			// posy = floorf(beg.y / 512);
			// //(3 by 4 is the max number of chunk rendered on the screen)
			// for (int y = posy; y < posy + 3; y++) {
			// 	for (int x = posx; x < posx + 4; x++) {
			// 		for (int k = 0; k < 16; k++) {
			// 			for (int n = 0; n < 16; n++) {
			// 				if (y >= 0 && x >= 0 || y <= 10000 || x <= 10000) {
			// 					atlas.renderTextureChunk(render_chunks[y * 1000 + x].tex[k][n], text_tileset, {x * 512 + (float)n * 32, y * 512 + (float)k * 32});
			// 				}
			// 			}
			// 		}
			// 	}
			// }
		}

		MAP() {
			// chunks.fill({0});
			// render_chunks.fill({0});
			// for (int k = 0; k < 1000000; k++) {
			// 	t_chunk &chunk = chunks.at(k);
			// 	Vector2 pos = getVector2Pos(k, 1000);
			// 	for (int y = 0; y < 16; y++) {
			// 		for (int x = 0; x < 16; x++) {
			// 			chunk.terrain[y][x] = tile_grass;
			// 		}
			// 	}
			// }
			// for (int k = 0; k < 1000000; k++) {
			// 	updateChunkTex(k);
			// }
		}
		~MAP() {
		}
};

#endif