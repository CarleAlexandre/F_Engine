#ifndef MAP_HPP
# define MAP_HPP

#include "include.hpp"

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
		}
		
		// bool isWall(Vector2 pos) {
		// 	Vector2 tmp = getBlockPos(pos);
		// 	tmp.x /= 32;
		// 	tmp.y /= 32;
		// 	//static double time = 0;
		// 	//time += GetFrameTime();
		// 	//if (time >= 3) {
		// 	//	std::cout << tmp.x << ", " << tmp.y << ". getblockposcoord ! \n";
		// 	//	std::cout << pos.x << ", " << pos.y << ". current pos ! \n";
		// 	//	std::cout << engine.level.map[tmp.x + tmp.y * engine.level.width].pos.x << ", " << engine.level.map[tmp.x + tmp.y * engine.level.width].pos.y << "\n";
		// 	//	time = 0;
		// 	//}
		// 	if (tmp.x > dimension.x || tmp.y > dimension.y || tmp.x < 0 || tmp.y < 0) {
		// 		std::cerr << "Error, Coordinate is OutOfBound!!\n";
		// 		return (true);
		// 	}
		// 	if (level.wall[(int)(tmp.x + tmp.y * level.dimension.x)]) {
		// 		return (true);
		// 	}
		// 	return (false);
		// }

		// Rectangle GetNearestWallBound(Vector2 pos, Rectangle *bound) {
		// 	float minDist1 = FLT_MAX;
		// 	float minDist2 = FLT_MAX;

		// 	bound[0] = {0};
		// 	bound[1] = {0};

		// 	int idx = (pos.x / 32 - 1) + (pos.y / 32 - 1) * level.dimension.x;
		// 	int hi = (pos.x / 32 + 1) + (pos.y / 32 + 1) * level.dimension.x;
		// 	for (int i = idx; i < hi; i++) {
		// 		if (level.wall[i]) {
		// 			Vector2 tmp = {i / level.dimension.x * 32, (float)(i % (int)level.dimension.y) * 32};
		// 			float dist = Vector2Distance(pos, tmp);
		// 			if (dist < minDist1 && level.wall[i]) {
		// 				minDist2 = minDist1;
		// 				bound[1] = bound[0];
		// 				minDist1 = dist;
		// 				bound[0] = {tmp.x, tmp.y, 32, 32};
		// 			} else if (dist < minDist2) {
		// 				minDist2 = dist;
		// 				bound[1] = {tmp.x, tmp.y, 32, 32};
		// 			}
		// 		}
		// 	}
		// 	return (bound[1]);
		// }

		// void CorrectWallCollision(t_player *player, const t_level &level, Rectangle *collision) {
		// 	double deltaTime = GetFrameTime();
		// 	Rectangle wall[2];
		// 	collision[0] = (Rectangle){0, 0, 0, 0};
		// 	collision[1] = (Rectangle){0, 0, 0, 0};

		// 	GetNearestWallBound({player->pos.x, player->pos.y}, level, wall);
		// 	if (CheckCollisionRecs(player->hitbox, wall[0]) || CheckCollisionRecs(player->hitbox, wall[1])) {
		// 		if (NORTH & player->dir) {
		// 			player->pos.y += player->stats.move_speed * deltaTime;
		// 		} else if (SOUTH & player->dir) {
		// 			player->pos.y -= player->stats.move_speed * deltaTime;
		// 		}
		// 		if (EAST & player->dir) {
		// 			player->pos.x -= player->stats.move_speed * deltaTime;
		// 		} else if (WEST & player->dir) {
		// 			player->pos.x += player->stats.move_speed * deltaTime;
		// 		}
		// 		player->hitbox.x = player->pos.x - player->hitbox.width * 0.5;
		// 		player->hitbox.y = player->pos.y + player->hitbox.height * 0.5;
		// 		collision[0] = GetCollisionRec(player->hitbox, wall[0]);
		// 		collision[1] = GetCollisionRec(player->hitbox, wall[1]);
		// 	}
		// }

		//GetNearestEntities(){}

		//void SpellCollision(t_engine &engine, Rectangle *collision) {
		//	double deltaTime = GetFrameTime();
		//	Rectangle wall[2];
		//	*collision = (Rectangle){0, 0, 0, 0};

		//	GetNearestEntities(engine.entities, engine.level.map, wall);
		//		//*collision = wall;
		//	if (CheckCollisionRecs(player.hitbox, wall[0]) || CheckCollisionRecs(player.hitbox, wall[1])) {
		//		collision[0] = GetCollisionRec(player.hitbox, wall[0]);
		//		collision[1] = GetCollisionRec(player.hitbox, wall[1]);


		//	}
		//}
		//check spell radius, if entity is in it, it take damage
		//do another function for the auto attacks where i check if entities hitbox is under mouse if yes then launch autoattacks
		void render(Camera2D camera){
			Vector2 beg = GetScreenToWorld2D({0,0}, camera);
			Vector2 end = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);

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