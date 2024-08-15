#ifndef INVENTORY_HPP
# define INVENTORY_HPP

#include <raygui.h>
#include <vector>
#include "item.hpp"


class TOOL_BAR {
	private:
		int current_item;
		bool gotonext;
		bool gotoprev;
	public:

		void render() {
			if (gotonext) {

			} else if (gotoprev) {

			} else {

			}
		}
		void update() {

		}
		void getNext() {
			gotonext = true;
			gotoprev = false;
			current_item++;
		}
		void getPrev() {
			gotoprev = true;
			gotonext = false;
			current_item--;
		}

		TOOL_BAR() {

		}
		~TOOL_BAR(){}
};

class INVENTORY {
	private:
		Rectangle View;
		Vector2 Scroll;
		std::vector<ITEM> consumable;
		std::vector<ITEM> structure;
		std::vector<ITEM> material;
		std::vector<ITEM> tool;
		std::vector<ITEM> weapon;
		int current_tab;
		Vector2 pos;
		bool isopen;
	public:
		TOOL_BAR tool_bar;
	
		void render(Texture2D texture) {
			if (isopen) {
				DrawTexture(texture, pos.x, pos.y, WHITE);
				switch (current_tab) {
					case (1): {
						break;
					}
					case (2): {

						break;
					}
					case (3): {

						break;
					}
					case (4): {

						break;
					}
					case (5): {
						
						break;
					}
					default: break;
				}
			}
			tool_bar.render();
		}
		void addItem(ITEM &item) {
			switch (item.type){
				case (item_type_structure):{
					structure.push_back(item);
				}
				case (item_type_material):{
					material.push_back(item);
				}
				case (item_type_tool):{
					tool.push_back(item);
				}
				case (item_type_consumable):{
					consumable.push_back(item);
				}
				case (item_type_weapon):{
					weapon.push_back(item);
				}
			}
		}
		void toggle() {
			isopen = !isopen;
		}

		INVENTORY() : tool_bar(){
			current_tab = 1;
			pos = {100, 100};
		}
		~INVENTORY() {
		}
};

#endif