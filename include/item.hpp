#ifndef ITEM_HPP
# define ITEM_HPP

typedef enum {
	item_type_weapon,
	item_type_tool,
	item_type_material,
	item_type_structure,
	item_type_consumable,
}item_type_e;

enum tool{
	tool_shovel,
	tool_axe,
	tool_hoe,
	tool_sprinkler,
	tool_wrench,
	tool_lockpick,
};

enum weapon{
	weapon_battle_axe,
	weapon_lance,
	weapon_bow,
	weapon_bowgun,
	weapon_wonder_weapon,
	weapon_fist,
	weapon_magic_staff,
};

enum consumable {
	consumable_bandage,
	consumable_kit,
	consumable_food,
	consumable_drugs,
	consumable_potion,
	consumable_music_instrument,
	consumable_trap,
	consumable_bomb,
	consumable_scroll,
};

enum structure {
	machine_pc,
	machine_door,
	structure_wall,
	structure_brick,
	structure_fence,
};

enum material {
	material_wood,
	material_sand,
	material_stone,
	material_cement,
	material_iron,
	material_silver,
	material_mithril,
	material_orichalc,
};

class ITEM {
	private:
	public:
		int textureid;
		item_type_e type;
		ITEM(){}
		~ITEM(){}
};

class Structure:ITEM {
	private:
	public:
		void place_structure(){}
		Structure(){}
		~Structure(){}
};


class Weapon:ITEM {
	private:
		int damage;
		int durability;
		int applied_block;
	public:
		void primary(){}
		Weapon(){}
		~Weapon(){}
};


class Tool:ITEM {
	private:
		int damage;
		int durability;
		int applied_block;
	public:
		Tool(){}
		~Tool(){}
};

class Consumable:ITEM {
	private:
		int damage;
		int durability;
		int applied_block;
	public:
		int effect;
		Consumable(){}
		~Consumable(){}
};


#endif
