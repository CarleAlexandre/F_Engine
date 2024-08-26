#ifndef ENTITY_HPP
# define ENTITY_HPP

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
	
		Mob() {

		}
		~Mob() {

		}
};

class Zombie : public Mob {
	private:
	public:
		Zombie(const Vector2 position) {
			pos = position;
		}
		~Zombie() {

		}
};

class Cow : public Mob {
	private:
	public:
		Cow(const Vector2 position) {
			pos = position;
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

        }
        void updateProjectil() {

        }
        void updateEffect() {

        }
        void updateGathering() {

        }
        void update() {
			static double time = 0;
			time += GetFrameTime();
			if (time > 0.2) {
				updateMobs();
            	updateProjectil();
            	updateEffect();
            	updateGathering();
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

        Entity() {

        };
        ~Entity() {

        };
};

#endif
