#ifndef ENTITY_HPP
# define ENTITY_HPP

#include "texture.hpp"
#include "pathfinding.hpp"

typedef enum {
        zombie_type,
        slime_type,
        cow_type,
		pig_type,
} e_mob_type;

typedef struct s_projectil {
        Vector2 pos;
        Vector2 topos;
        bool destroy;
        int damage;
} t_projectil;
//idea for random projectil, use a quaternion to calculat a fonction 

typedef struct s_effect {
        Vector2 pos;
        Vector2 topos;
        double time;
        bool destroy;
		bool move;
} t_effect;

typedef struct s_gathering {
        Vector2 pos;
        Vector2 topos;
        int hit_time;
        int harvet_level;
} t_gathering;

class Mob {
	private:
	public:
        Vector2 pos;
        Vector2 topos;
        float damage;
        float life;
        float max_life;
		float range;
		Vector2 hitbox;

		void updatePos(Pathfinding &pf, MAP *map) {
			if (Vector2Distance(pos, topos) > 0.1)
				pos = Vector2MoveTowards(pos, topos, 0.2);
		};

		void virtual update(Pathfinding &pf, MAP *map){}

		Mob() {

		}
		~Mob() {

		}
};

class Zombie : public Mob {
	private:
	public:

		void update(Pathfinding &pf, MAP *map) {
			updatePos(pf, map);
		}

		Zombie(const Vector2 position) {
			pos = position;
			topos = pos;
			damage = 1;
			life = 100;
			max_life = 100;
			range = 10;
			hitbox = {16, 16};
		}
		~Zombie() {

		}
};

class Cow : public Mob {
	private:
	public:

		void update(Pathfinding &pf, MAP *map) {
			updatePos(pf, map);
		}
		Cow(const Vector2 position) {
			pos = position;
			topos = pos;
			damage = 0;
			life = 100;
			max_life = 100;
			range = 0;
			hitbox = {16, 16};
		}
		~Cow() {

		}
};

class Entity {
    private:
        std::vector<Mob *> mobs;
        // std::vector<t_projectil> projectil;
        // std::vector<t_effect> effect;
        // std::vector<t_gathering> gathering;
		double updateTime = 0;
    public:
        void spawnMobs(Vector2 pos, int type, ATLAS &atlas) {
			switch (type) {
				case (cow_type) : {
			    	mobs.push_back(new Cow(pos));
					atlas.addAnimationToQueue(text_hero, &mobs.back()->pos, 0, 0, frame_loop_enable);
					break;
				}
				case (zombie_type) : {
					mobs.push_back(new Zombie(pos));
					atlas.addAnimationToQueue(text_hero, &mobs.back()->pos, 0, 0, frame_loop_enable);
					break;
				}
				default:break;
			}
        }
        // void spawnProjectil() {
        //     projectil.push_back((t_projectil){}); 
        // }
        // void spawnEffect() {
        //     effect.push_back((t_effect){});
        // }

        void updateMobs(Pathfinding &pf, MAP *map) {
			for (int i = 0; i < mobs.size(); i++) {
				mobs[i]->update(pf, map);
			}
        }
        // void updateProjectil() {

        // }
        // void updateEffect() {

        // }
        // void updateGathering() {

        // }

        void update(Pathfinding &pf, MAP *map) {
			updateTime += GetFrameTime();
			if (updateTime > 0.2) {
				updateMobs(pf, map);
            	// updateProjectil();
            	// updateEffect();
            	// updateGathering();
				updateTime = 0;
			}
			// for (int i = 0; i < projectil.size(); i++) {
			// 	projectil[i].pos = Vector2MoveTowards(projectil[i].pos, projectil[i].topos, 0.2);
			// }
			// for (int i = 0; i < effect.size(); i++) {
			// 	if (effect[i].move == true && Vector2Distance(effect[i].pos, effect[i].topos) > 0.1)
			// 		effect[i].pos = Vector2MoveTowards(effect[i].pos, effect[i].topos, 0.2);
			// }
        }

        Entity() {
			mobs = std::vector<Mob *>();
        };
        ~Entity() {

        };
};

#endif
