#ifndef INVENTORY_HPP
# define INVENTORY_HPP

#include "item.hpp"
#include "texture.hpp"

class TOOL_BAR {
	private:
		int current_item;
		bool gotonext;
		bool gotoprev;
		Vector2 pos;
		Vector2 pos1, topos1;
		Vector2 pos2, topos2;
	public:

		void render() {
			BeginScissorMode(pos.x, pos.y, 80, 60);
				// DrawTexturePro(, {}, {0, 0, 40, 40}, pos1, 0, WHITE);
				// DrawTexturePro(, {}, {0, 0, 40, 40}, pos2, 0, WHITE);
				DrawRectangleRec({pos1.x, pos1.y,40,40}, WHITE);
				DrawRectangleRec({pos2.x, pos2.y,40,40}, WHITE);
			EndScissorMode();
		}
		void update() {
			if (gotonext || gotoprev) {
				pos1 = Vector2MoveTowards(pos1, topos1, 10);
				pos2 = Vector2MoveTowards(pos2, topos2, 10);
				if (abs(pos1.x - topos1.x) <= 0.1 || abs(pos2.x - topos2.x) <= 0.1) {
					gotonext = false;
					gotoprev = false;
				}
			}
		}
		void getNext() {
			topos1.x = pos.x + 20;
			topos1.y = pos.y + 10;
			topos2.x = pos.x + 80;
			topos2.y = pos.y + 10;
			pos1 = {pos.x - 40, pos.y + 10};
			pos2 = {pos.x + 20, pos.y + 10};
			gotonext = true;
			gotoprev = false;
			current_item++;
		}
		void getPrev() {
			topos1.x = pos.x - 40;
			topos1.y = pos.y + 10;
			topos2.x = pos.x + 20;
			topos2.y = pos.y + 10;
			pos1 = {pos.x + 20, pos.y + 10};
			pos2 = {pos.x + 80, pos.y + 10};
			gotoprev = true;
			gotonext = false;
			current_item--;
		}

		TOOL_BAR() {
			pos.x = GetScreenWidth() - 140;
			pos.y = GetScreenHeight() - 100;
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
	
		void render(Texture2D text) {
			if (isopen) {
				DrawTexture(text, pos.x, pos.y, WHITE);
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
			isopen = false;
		}
		~INVENTORY() {
		}
};

#endif