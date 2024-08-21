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

namespace PERLIN {
	Vector2 gradients[8] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
    {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
	};

	int perm[256] = {151,160,137,91,90,15
		,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23
		,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33
		,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166
		,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244
		,102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196
		,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123
		,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42
		,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9
		,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228
		,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107
		,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254
		,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
	};

	// Simple hash function
	int hash(int x, int y) {
		return perm[(perm[x % 256] + y) % 256];
	}

	// Dot product of the gradient and distance vector
	float dot(Vector2 g, float x, float y) {
		return g.x * x + g.y * y;
	}

	// Fade function to smooth transitions
	float fade(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	// Linear interpolation
	float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}

	// Perlin noise function with frequency and intensity as arguments
	float perlin(float x, float y, float frequency, float intensity) {
		// Adjust the coordinates based on frequency
		x *= frequency;
		y *= frequency;

		// Grid cell coordinates
		int x0 = (int)floor(x) & 255;
		int y0 = (int)floor(y) & 255;
		int x1 = (x0 + 1) & 255;
		int y1 = (y0 + 1) & 255;

		// Local coordinates within the grid cell
		float sx = x - floor(x);
		float sy = y - floor(y);

		// Compute fade curves
		float u = fade(sx);
		float v = fade(sy);

		// Select gradients based on the hashed coordinates
		Vector2 g00 = gradients[hash(x0, y0) % 8];
		Vector2 g10 = gradients[hash(x1, y0) % 8];
		Vector2 g01 = gradients[hash(x0, y1) % 8];
		Vector2 g11 = gradients[hash(x1, y1) % 8];

		// Compute the dot products between gradients and distance vectors
		float n00 = dot(g00, sx, sy);
		float n10 = dot(g10, sx - 1, sy);
		float n01 = dot(g01, sx, sy - 1);
		float n11 = dot(g11, sx - 1, sy - 1);

		// Interpolate the dot products
		float ix0 = lerp(n00, n10, u);
		float ix1 = lerp(n01, n11, u);
		float value = lerp(ix0, ix1, v);

		// Scale the result by intensity
		return value * intensity;
	}
}

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

		void render(Camera2D camera, ATLAS &atlas) {
			Vector2 beg = GetScreenToWorld2D({0,0}, camera);

			//get mod of 512 pos beg to know hist relative pos to chunk border
			//same for end

			int posx, posy;
			posx = floorf(beg.x / 512);
			posy = floorf(beg.y / 512);
			//(3 by 4 is the max number of chunk rendered on the screen)
			for (int y = posy; y < posy + 3; y++) {
				for (int x = posx; x < posx + 4; x++) {
					for (int k = 0; k < 16; k++) {
						for (int n = 0; n < 16; n++) {
							atlas.renderTextureChunk(render_chunks[y * 1000 + x].tex[k][n], text_tileset, {x * 512 + (float)n * 32, y * 512 + (float)k * 32});
						}
					}
				}
			}
		}

		MAP() {
			chunks.fill({0});
			render_chunks.fill({0});
			for (int k = 0; k < 1000000; k++) {
				t_chunk &chunk = chunks.at(k);
				Vector2 pos = getVector2Pos(k, 1000);
				for (int y = 0; y < 16; y++) {
					for (int x = 0; x < 16; x++) {
						if (GetRandomValue(0, 10) > 5) {
							chunk.terrain[y][x] = tile_grass;
						}
					}
				}
			}
			for (int k = 0; k < 1000000; k++) {
				updateChunkTex(k);
			}
		}
		~MAP() {
		}
};

#endif