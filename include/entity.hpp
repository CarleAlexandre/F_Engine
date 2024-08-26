#ifndef ENTITY_HPP
# define ENTITY_HPP

#include "include.hpp"

typedef enum {
        zombie,
        slime,
        cow,
		pig,
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
		Color color;
	
		void pathFinding() {

		}

		void render() {
			DrawRectangle(pos.x, pos.y, hitbox.x, hitbox.y, color);
		}

		virtual void update() {

		};

		Mob() {

		}
		~Mob() {

		}
};

class Zombie : public Mob {
	private:
	public:

		void update() {

		}

		Zombie(const Vector2 position) {
			pos = position;
			topos = pos;
			damage = 1;
			life = 100;
			max_life = 100;
			range = 10;
			hitbox = {16, 16};
			color = GREEN;
		}
		~Zombie() {

		}
};

class Cow : public Mob {
	private:
	public:

		void update() {

		}
		Cow(const Vector2 position) {
			pos = position;
			topos = pos;
			damage = 0;
			life = 100;
			max_life = 100;
			range = 0;
			hitbox = {16, 16};
			color = BROWN;
		}
		~Cow() {

		}
};

class Entity {
    private:
        std::vector<Mob> mobs;
        std::vector<t_projectil> projectil;
        std::vector<t_effect> effect;
        std::vector<t_gathering> gathering;
		double updateTime = 0;
    public:
		template <typename T>
        void spawnMobs(Vector2 pos, int dmg, int max_life) {
                mobs.push_back();
        }
        void spawnProjectil() {
                projectil.push_back((t_projectil){}); 
        }
        void spawnEffect() {
                effect.push_back((t_effect){});
        }

        void updateMobs() {
			for (auto tmp : mobs) {
				tmp.update();
			}
        }
        void updateProjectil() {

        }
        void updateEffect() {

        }
        void updateGathering() {

        }
        void update() {
			updateTime += GetFrameTime();
			if (updateTime > 0.2) {
				updateMobs();
            	updateProjectil();
            	updateEffect();
            	updateGathering();
				updateTime = 0;
			}
			for (auto tmp : mobs) {
				if (Vector2Distance(tmp.pos, tmp.topos) > 0.1)
				tmp.pos = Vector2MoveTowards(tmp.pos, tmp.topos, 0.2);
			}
			for (auto tmp : projectil) {
				tmp.pos = Vector2MoveTowards(tmp.pos, tmp.topos, 0.2);
			}
			for (auto tmp : effect) {
				if (tmp.move == true && Vector2Distance(tmp.pos, tmp.topos) > 0.1)
					tmp.pos = Vector2MoveTowards(tmp.pos, tmp.topos, 0.2);
			}
        }

		void render() {
			for (auto tmp : mobs) {
				tmp.render();
			}
		}

        Entity() {

        };
        ~Entity() {

        };
};

#endif
