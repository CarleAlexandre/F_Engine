#ifndef ITEM_HPP
# define ITEM_HPP

typedef enum {
	item_type_weapon,
	item_type_tool,
	item_type_material,
	item_type_structure,
	item_type_machine,
	item_type_useable,
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
		int damage;
		int durability;
		int applied_block;
	public:
		item_type_e type;
		void use_item(){}
		ITEM(){}
		~ITEM(){}
};

#endif
