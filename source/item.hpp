#ifndef ITEM_HPP
# define ITEM_HPP

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

enum useable {
	useable_bandage,
	useable_kit,
	useable_food,
	useable_drugs,
	useable_potion,
	useable_music_instrument,
	useable_trap,
	useable_bomb,
	useable_scroll,
};

enum machine {
	machine_pc,
	machine_door,
};

enum structure {
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
		void use_item();
		ITEM();
		~ITEM();
};

class Weapon:ITEM {
	private:
	public:
		int damage;
		int durability;
		Weapon();
		~Weapon();
};

class Tool:Weapon {
	private:
	public:
		int applied_block;
		Tool();
		~Tool();
};

class Structure:ITEM {
	private:
	public:
		Structure();
		~Structure();
};

class Machine:Structure {
	private:
	public:
		Machine();
		~Machine();
};

#endif
