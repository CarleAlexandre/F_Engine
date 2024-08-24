#ifndef ENTITY_HPP
# define ENTITY_HPP

typedef struct S_mobs {
        Vector2 pos;
        Vector2 topos;
        float damage;
        float life;
        float max_life;
} t_mobs;

typedef struct s_projectil {
        Vector2 pos;
        Vector2 topos;
        bool destroy;
        int damage;
} t_projectil;

typedef struct s_effect {
        Vector2 pos;
        Vector2 topos;
        double time;
        bool destroy;
} t_effect;

typedef struct s_gathering {
        Vector2 pos;
        Vector2 topos;
        int hit_time;
        int harvet_level;
} t_gathering;

class Entity {
    
};

#endif
