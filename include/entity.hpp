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
