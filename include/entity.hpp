#ifndef ENTITY_HPP
# define ENTITY_HPP
typedef enum {
	entity_type_mobs,
	entity_type_bullet,
	entity_type_effect,
	entity_type_gathering,
	entity_type_plant,
}entity_type_e;

class Entity {
    private:
        unsigned int textureId;
        Vector2 pos;
        Vector2 topos;
        Vector2 dim;
        Vector2 animationframe;
        entity_type_e type;
        char y;
        bool detected_player;
        float damage;
        float life;
        float max_life;
    public:
};

#endif
