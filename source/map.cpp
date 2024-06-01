#include <engine.hpp>
#include <raygui.h>

extern t_engine engine;

void freeLevel(t_level *level) {
	MemFree(level->event);
	MemFree(level->terrain);
	MemFree(level->wall);
	level->dimension = {0, 0, 0};
}

void drawLevel(t_level &level) {
	Vector2 beg = GetScreenToWorld2D({0,0}, engine. camera);
	Vector2 end = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, engine. camera);

	for (float y = beg.y; y < level.dimension.y && y < end.y; y ++) {
		for (float z = 0; z < level.dimension.z; z += 32){
			for (float x = beg.x; x < level.dimension.x && x < end.x; x += 32) {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["RA_Ground_Tiles"]], {(float)level.terrain[(int)y][getLinearIndex(x, z, level.dimension.x)], }, {x, z}, WHITE);
			}
		}
	}
}

typedef struct s_builder_ctx {
	bool file_active;
	int file_action;
	bool tool_active;
	int tool_action;
	bool tileset_active;
	Texture2D *tileset;
	Rectangle tile_bound;
	Vector2 tileset_scroll;
	Rectangle layer_bound;
	Vector2 layer_scroll;
	char tileset_input[20];
	Rectangle tile_view;
	Rectangle layer_view;
	std::vector<int *> layer;
	Vector2 draw_scroll;
	Rectangle draw_view;
	int selected_layer;
	int selected_tile;
	t_level level;
} t_builder_ctx;

void mapBuilder(void) {
	static t_builder_ctx ctx{
		.tileset_active = false,
		.tile_bound = { 0, 20, 100, 0},
		.layer_bound = { 0, 0, 100, 0},
		.tileset_input = "\0",
		.selected_layer = 0,
		.selected_tile = -1,
	};
	int width = GetScreenWidth();

	if (!ctx.tile_bound.height) {
		ctx.tile_bound.height = (float)(GetScreenHeight() * 0.5) - 20;
		ctx.layer_bound.y = (float)(GetScreenHeight() * 0.5) + 20;
		ctx.layer_bound.height = (float)(GetScreenHeight() * 0.5) - 20;
	}
	
	ctx.layer_bound.width = ctx.tile_bound.width;
	ctx.layer_bound.height = width - ctx.tile_bound.height - 20;

	if (!ctx.file_active) {
		GuiScrollPanel(ctx.tile_bound, NULL, {0, 0, 20, 20}, &ctx.tileset_scroll, &ctx.tile_view);//tileset
		if (ctx.tileset_active) {
			BeginScissorMode(ctx.tile_view.x, ctx.tile_view.y, ctx.tile_view.width, ctx.tile_view.height);
				DrawTextureRec(*ctx.tileset, {0, 0, (float)ctx.tileset->width, (float)ctx.tileset->height}, {0, 0}, WHITE);//DrawTileset
			EndScissorMode();
		}
		//if (GuiTextBox({}, ctx.tileset_input, 20, true)) {
		//	ctx.tileset = &engine.textures[engine.texture_dictionnary[ctx.tileset_input]];
		//	ctx.tileset_active = true;
		//}
	}

	if (GuiButton({0, ctx.layer_bound.y - 18, (float)(ctx.layer_bound.width * 0.5), 16}, "ADD")) {
		int *tab = (int *)MemAlloc(sizeof(int) * ctx.level.dimension.x * ctx.level.dimension.z);
		ctx.layer.push_back({0});
	}
	if (!ctx.layer.empty() && GuiButton({(float)(ctx.layer_bound.width * 0.5), ctx.layer_bound.y - 18, (float)(ctx.layer_bound.width * 0.5), 16}, "DEL")) {
		MemFree(ctx.layer[ctx.layer.size() - 1]);
		ctx.layer.pop_back();
	}
	GuiScrollPanel(ctx.layer_bound, NULL, {ctx.layer_bound.x, ctx.layer_bound.y, ctx.layer_bound.width - 20, (float)(ctx.layer.size() * 20)}, &ctx.layer_scroll, &ctx.layer_view);
	BeginScissorMode(ctx.layer_view.x, ctx.layer_view.y, ctx.layer_view.width, ctx.layer_view.height);
		for (int i = 0; i < ctx.layer.size(); i++) {
			if (GuiButton({1, ctx.layer_bound.y + 20 * i  + ctx.layer_scroll.y, ctx.layer_bound.width - 21, 20}, TextFormat("Layer %i", i))) {
				ctx.layer[i] = 0;
			}
		}
	EndScissorMode();

	//	zone pencil
	GuiScrollPanel({ctx.tile_bound.width, 20, width - ctx.tile_bound.width, (float)GetScreenHeight() - 20}, NULL, {0, 0, 0, 0}, &ctx.draw_scroll, &ctx.draw_view);
	Vector2 mouse_pos = GetMousePosition();
	if (!ctx.tool_active && IsMouseInBound({0, 0, width - ctx.tile_bound.width, (float)GetScreenHeight() - 20}, {ctx.tile_bound.width, 20}, mouse_pos)) {
		HideCursor();
		mouse_pos.x += ctx.draw_scroll.x;
		mouse_pos.y += ctx.draw_scroll.y;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			switch (ctx.tool_action) {
				case (0): {
#ifdef DEBUG
					std::cout << "BRUSHIIIIIIIIIING !!\n";
#endif
					//at pos on current layer
					if (ctx.selected_tile != -1) {
						ctx.layer[ctx.selected_layer][(int)floor(mouse_pos.x + mouse_pos.y * ctx.level.dimension.x)] = ctx.selected_tile;
					}
					break;
				}
				case (1): {
#ifdef DEBUG
					std::cout << "ERAAASINNNNNNNNG !!\n";
#endif
					ctx.layer[ctx.selected_layer][(int)floor(mouse_pos.x + mouse_pos.y * ctx.level.dimension.x)] = -1;
					//erase at pos on current layer
					break;
				}
				case (2): {
#ifdef DEBUG
					std::cout << "FEELIIING !!\n";
#endif
					if (ctx.selected_tile != -1) {
						for (int y = 0; y < ctx.level.dimension.y; y++){
							for (int x = 0; x < ctx.level.dimension.x; x++){
								ctx.layer[ctx.selected_layer][x + y * (int)ctx.level.dimension.x] = ctx.selected_tile;
							}
						}
					}
					//fill current layer
					break;
				}
				default:break;
			}
		}
	} else if (IsCursorHidden()) {
		ShowCursor();
	}


	//menu bar
	DrawRectangle(0, 0, width, 20, GRAY);
	//GuiTabBar(Rectangle bounds, const char **text, int count, int *active);
	if (GuiDropdownBox({0, 0, 100, 20}, "FILE;SAVE;NEW;OPEN;CLOSE", &ctx.file_action, ctx.file_active))
		ctx.file_active = !ctx.file_active;
	if (!ctx.file_active) {
		ctx.file_action = 0;
	} else  {
		switch (ctx.file_action) {
			case (1): {
				break;
			}
			case (2): {
				break;
			}
			case (3): {
				break;
			}
			case(4): {
				break;
			}
			default:break;
		}
	}

	MouseCursor mscurs;
	if (GuiButton({100, 0, 100, 20}, "SETTING")) {

	}
	if (GuiDropdownBox({200, 0, 100, 20}, "BRUSH;ERASER;FILL", &ctx.tool_action, ctx.tool_active))
		ctx.tool_active = !ctx.tool_active;
	if (IsCursorHidden()) {
		switch (ctx.tool_action) {
			case (0): {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["BRUSH"]], {0, 0, 32, 32}, {mouse_pos.x, mouse_pos.y}, WHITE);
				break;
			}
			case (1): {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["ERASE"]], {0, 0, 32, 32}, {mouse_pos.x, mouse_pos.y}, WHITE);
				break;
			}
			case (2): {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["FILL"]], {0, 0, 32, 32}, {mouse_pos.x, mouse_pos.y}, WHITE);
				break;
			}
			default:break;
		}
	}
	//GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode);
	//GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode);
}
