#ifndef INVENTORY_HPP
# define INVENTORY_HPP

#include <raygui.h>
#include <vector>
#include "item.hpp"


class TOOL_BAR {
	private:
		std::vector<ITEM> store;
	public:

		void render() {
			int height = GetScreenHeight();
			int width = GetScreenWidth();
			float tmp = width - 290;
			DrawRectangleRec({(float)(tmp * 0.5), (float)height - 32,  (float)width - tmp, 20}, RED);
		}
		void addItem(ITEM &item, int idx) {
			store[idx] = item;
		}
		void delItem(int idx) {
			store[idx] = ITEM();
		}


		TOOL_BAR() {
			store.reserve(10);
		}
		~TOOL_BAR(){}
};

class INVENTORY {
	private:
		Rectangle View;
		Vector2 Scroll;
		std::vector<ITEM> store;
		int current_tab;
		bool isopen;
	public:
		TOOL_BAR tool_bar;
	
		void render() {
			if (isopen) {
				switch (current_tab) {
					case (1): {
						int column = 5;
						int row = store.size() * 0.2;
						GuiScrollPanel({0, 0, 480, 360}, "Inventory", {}, &Scroll, &View);
						BeginScissorMode(View.x + Scroll.x, View.y + Scroll.y, View.width, View.height);
							
						EndScissorMode();
						//render toolbar
						break;
					}
					case (2): {
						//head shoulder body belt pants shoes arm gloves
						//left ring right ring amulet left earring right earring
						break;
					}
					case (3): {

						break;
					}
					default: break;
				}
			}
			tool_bar.render();
		}
		void addItem(ITEM &item) {
			store.push_back(item);
		}
		void delItem(int idx) {
			store.erase(store.cbegin() + idx);
		}
		void toggle() {
			isopen = !isopen;
		}

		INVENTORY() : tool_bar(){
			current_tab = 1;
		}
		~INVENTORY() {
			store.clear();
		}
};

#endif