#include <engine.hpp>
#include <raygui.h>
#include <cstring>

extern t_engine engine;

void drawLevel(t_level &level) {
	Vector2 beg = GetScreenToWorld2D({0,0}, engine. camera);
	Vector2 end = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, engine. camera);

	for (float y = beg.y; y < level.dimension.y && y < end.y; y ++) {
		for (float z = 0; z < level.dimension.z; z += 32){
			for (float x = beg.x; x < level.dimension.x && x < end.x; x += 32) {
				//DrawTextureRec(engine.textures[engine.texture_dictionnary["RA_Ground_Tiles"]], {(float)level.terrain[(int)y][getLinearIndex(x, z, level.dimension.x)], }, {x, z}, WHITE);
			}
		}
	}
}

typedef enum {
	error_dummy = 0,
	error_file_not_found = 1,
	error_file_empty = 2,
	error_file_exist = 3,
	error_file_corrupted = 4,
	error_tileset_not_found = 5,
	error_bad_dimension = 6,
} map_error_e;

typedef struct s_builder_ctx {
	bool file_active;
	int file_action;
	bool tool_active;
	int tool_action;
	char tileset[20];
	Rectangle tile_bound;
	Vector2 tileset_scroll;
	Rectangle layer_bound;
	Vector2 layer_scroll;
	char tileset_input[20];
	Rectangle tile_view;
	Rectangle layer_view;
	Vector2 draw_scroll;
	Rectangle draw_view;
	int selected_layer;
	int selected_tile;
	bool saved;
	char filename[20];
	bool show_properties;
	Rectangle properties_bound;
	bool file_open;
	Rectangle file_bound;
	map_error_e error;
	bool new_file;
	char dim_x[4];
	char dim_z[4];
	char dim_y[3];
	RenderTexture2D draw_buffer;
	bool draw_ready;
	float zoom;
	bool tileset_ok;
} t_builder_ctx;


void addNewLayer(t_level *level, const Vector3 new_dim) {
	int *new_terrain;

	new_terrain = (int*)MemAlloc(sizeof(int) * new_dim.x * new_dim.z * new_dim.y);
	if (!memcpy(new_terrain, level->terrain, (level->dimension.x * level->dimension.z * level->dimension.y)))
		abort();

	MemFree(level->terrain);
	level->dimension = new_dim;
	level->terrain = new_terrain;
}

void clearLevel(t_level *level) {
	memset(level->terrain, 0, sizeof(int) * (level->dimension.x * level->dimension.z * level->dimension.y - 1));
	memset(level->wall, 0, sizeof(int) * (level->dimension.x * level->dimension.z - 1));
	memset(level->event, 0, sizeof(int) * (level->dimension.x * level->dimension.z - 1));
}

t_level CreateNewLevel(const Vector3 dim, const char *filename, const char *tileset) {
	t_level new_level;

	new_level.dimension = dim;
	new_level.wall = (int *)MemAlloc(sizeof(int) * dim.x * dim.z);
	new_level.event = (int *)MemAlloc(sizeof(int) * dim.x * dim.z);
	new_level.terrain = (int *)MemAlloc(sizeof(int) * dim.x * dim.z * dim.y);
	clearLevel(&new_level);
#ifdef __linux__
	new_level.filename = strdup(filename);
	new_level.tileset = strdup(tileset);
#else
	new_level.filename = _strdup(filename);
	new_level.tileset = _strdup(tileset);
#endif
	return (new_level);
}

void freeLevel(t_level *level) {
	clearLevel(level);
	MemFree(level->filename);
	MemFree(level->terrain);
	MemFree(level->event);
	MemFree(level->wall);
	level->dimension = {0, 0, 0};
}

void openFile(t_level *level, const char *filename) {
	freeLevel(level);
	*level = loadLevel(filename);
}

void saveMap(t_level level) {
	writeToLevel(level);
}

//not recommandeed as it realloc new memory and can take some time for big map
void resizeMap(t_level *level, const Vector3 new_dim) {
	int *new_wall;
	int *new_event;
	int *new_terrain;

	new_terrain = (int*)MemAlloc(sizeof(int) * new_dim.x * new_dim.z * new_dim.y);
	if (!memcpy(new_terrain, level->terrain, (level->dimension.x * level->dimension.z * level->dimension.y - 1) * sizeof(int)))
		abort();
	new_event = (int *)MemAlloc(sizeof(int) * new_dim.x * new_dim.z);
	if (!memcpy(new_event, level->event, (level->dimension.x * level->dimension.z - 1) * sizeof(int)))
		abort();
	new_wall = (int *)MemAlloc(sizeof(int) * new_dim.x * new_dim.z);
	if (!memcpy(new_wall, level->wall, (level->dimension.x * level->dimension.z - 1) * sizeof(int)))
		abort();
	freeLevel(level);
	level->dimension = new_dim;
	level->terrain = new_terrain;
	level->wall = new_wall;
	level->event = new_event;
}

int mapBuilder(void) {
	static t_builder_ctx ctx{
		.tileset = "\0",
		.tile_bound = { 0, 20, 100, 0},
		.layer_bound = { 0, 0, 100, 0},
		.tileset_input = "\0",
		.selected_layer = 0,
		.selected_tile = 1,
		.saved = true,
		.filename = "\0",
		.properties_bound = {20, 20, 300, 200},
		.file_open = false,
		.file_bound = {20, 20, 300, 200},
		.new_file = true,
		.dim_x = "\0",
		.dim_z = "\0",
		.dim_y = "\0",
		.draw_ready = false,
		.zoom = 1,
	};

	Texture2D current_tileset;

	int width = GetScreenWidth();
	if (!ctx.new_file) {
		ctx.tileset_ok = engine.texture_dictionnary.contains(engine.level.tileset);
		if (ctx.tileset_ok) {
			current_tileset = engine.textures[engine.texture_dictionnary[engine.level.tileset]];
		}
	}
	if (!ctx.tile_bound.height) {
		ctx.tile_bound.height = (float)(GetScreenHeight() * 0.5) - 20;
		ctx.layer_bound.y = (float)(GetScreenHeight() * 0.5) + 20;
		ctx.layer_bound.height = (float)(GetScreenHeight() * 0.5) - 20;
	}
	
	ctx.layer_bound.width = ctx.tile_bound.width;
	ctx.layer_bound.height = width - ctx.tile_bound.height - 20;

	if (!ctx.file_active) {
		GuiScrollPanel(ctx.tile_bound, NULL, {0, 0, (float)current_tileset.width, (float)current_tileset.height}, &ctx.tileset_scroll, &ctx.tile_view);//tileset
		if (ctx.tileset_ok) {
			BeginScissorMode(ctx.tile_view.x, ctx.tile_view.y, ctx.tile_view.width, ctx.tile_view.height);
				DrawTextureRec(current_tileset, {0, 0, (float)current_tileset.width, (float)current_tileset.height}, {ctx.tileset_scroll.x, ctx.tileset_scroll.y}, WHITE);//DrawTileset
			EndScissorMode();
		}
		//if (GuiTextBox({}, ctx.tileset_input, 20, true)) {
		//	ctx.tileset = &engine.textures[engine.texture_dictionnary[ctx.tileset_input]];
		//	ctx.tileset_active = true;
		//}
	}

	//if (GuiButton({0, ctx.layer_bound.y - 18, (float)(ctx.layer_bound.width * 0.5), 16}, "ADD")) {
	//	int *tab = (int *)MemAlloc(sizeof(int) * ctx.level->dimension.x * ctx.level->dimension.z);
	//}
	//if (!ctx.layer.empty() && GuiButton({(float)(ctx.layer_bound.width * 0.5), ctx.layer_bound.y - 18, (float)(ctx.layer_bound.width * 0.5), 16}, "DEL")) {
	//	MemFree(ctx.layer[ctx.layer.size() - 1]);
	//}
	GuiScrollPanel(ctx.layer_bound, NULL, {ctx.layer_bound.x, ctx.layer_bound.y, ctx.layer_bound.width - 20, (float)(engine.level.dimension.y * 20)}, &ctx.layer_scroll, &ctx.layer_view);
	BeginScissorMode(ctx.layer_view.x, ctx.layer_view.y, ctx.layer_view.width, ctx.layer_view.height);
		ClearBackground(BLACK);
		for (int i = 0; i < engine.level.dimension.y; i++) {
			if (GuiButton({1, ctx.layer_bound.y + 20 * i  + ctx.layer_scroll.y, ctx.layer_bound.width - 21, 20}, TextFormat("Layer %i", i))) {
				ctx.selected_layer = i;
			}
		}
	EndScissorMode();


	//	zone pencil
	GuiScrollPanel({ctx.tile_bound.width, 20, width - ctx.tile_bound.width, (float)GetScreenHeight() - 20}, NULL, {0, 0, engine.level.dimension.x * 32, engine.level.dimension.z * 32}, &ctx.draw_scroll, &ctx.draw_view);

	if (ctx.draw_ready && !ctx.new_file && ctx.tileset_ok) {
		BeginTextureMode(ctx.draw_buffer);
			for (int i = 0; i < engine.level.dimension.x * engine.level.dimension.z; i ++) {
				Vector2 tmp = getVector2Pos(i, engine.level.dimension.x);
				DrawTextureRec(current_tileset, getTextureRec(engine.level.terrain[i], current_tileset), {tmp.x * 32, tmp.y * 32}, WHITE);
			}
		EndTextureMode();
	}
	if (ctx.draw_ready) {
		BeginScissorMode(ctx.draw_view.x, ctx.draw_view.y, ctx.draw_view.width, ctx.draw_view.height);
			//DrawTextureEx(ctx.draw_buffer.texture, {ctx.draw_scroll.x + ctx.tile_bound.width, ctx.draw_scroll.y + 20}, 0, ctx.zoom, WHITE);
			//DrawTexturePro(ctx.draw_buffer.texture, {0, 0, (float)ctx.draw_buffer.texture.width, (float)ctx.draw_buffer.texture.height}, {0, 0, (float)ctx.draw_buffer.texture.width, -(float)ctx.draw_buffer.texture.height}, {ctx.draw_scroll.x + ctx.tile_bound.width, ctx.draw_scroll.y + 20}, 0, WHITE);
			DrawTextureRec(ctx.draw_buffer.texture, {0, 0, (float)ctx.draw_buffer.texture.width, -(float)ctx.draw_buffer.texture.height}, {ctx.draw_scroll.x + ctx.tile_bound.width, ctx.draw_scroll.y + 20}, WHITE);
		EndScissorMode();
	}
	Vector2 mouse_pos = GetMousePosition();
	if (!ctx.new_file && !ctx.error && !ctx.tool_active && !ctx.show_properties && !ctx.file_open && \
		IsMouseInBound({0, 0, width - ctx.tile_bound.width, (float)GetScreenHeight() - 20}, \
			{ctx.tile_bound.width, 20}, mouse_pos)) {
		HideCursor();
		mouse_pos.x -= ctx.tile_bound.width;
		mouse_pos.y -= 20;
		mouse_pos.x += ctx.draw_scroll.x;
		mouse_pos.y += ctx.draw_scroll.y;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			switch (ctx.tool_action) {
				case (0): {
					//at pos on current layer
					//if (ctx.selected_tile != -1) {
					if (IsInBond(mouse_pos, {engine.level.dimension.x, engine.level.dimension.z}, {0, 0})) {
#ifdef DEBUG
						std::cout << "BRUSHIIIIIIIIIING !!\n";
						std::cout << engine.level.terrain[linearIndexFromCoordinate({mouse_pos.x, mouse_pos.y, (float)ctx.selected_layer}, engine.level.dimension.z, engine.level.dimension.y)] << " at: " << mouse_pos.x  << ", " << mouse_pos.y << "\n";
#endif
						engine.level.terrain[linearIndexFromCoordinate({mouse_pos.x, mouse_pos.y, (float)ctx.selected_layer}, engine.level.dimension.z, engine.level.dimension.y)] = 2;
					}
					//}
					break;
				}
				case (1): {
#ifdef DEBUG
					std::cout << "ERAAASINNNNNNNNG !!\n";
					std::cout << engine.level.event[(int)floor(mouse_pos.x + mouse_pos.y * engine.level.dimension.x)] << " at: " << mouse_pos.x  << ", " << mouse_pos.y << "\n";
#endif
					//erase at pos on current layer
					break;
				}
				case (2): {
#ifdef DEBUG
					std::cout << "FEELIIING !!\n";
#endif
					if (ctx.selected_tile != -1) {
						for (int y = 0; y < engine.level.dimension.y; y++){
							for (int x = 0; x < engine.level.dimension.x; x++){
#ifdef DEBUG
								std::cout << engine.level.terrain[linearIndexFromCoordinate({x, y, (float)ctx.selected_layer}, engine.level.dimension.z, engine.level.dimension.y)] << "\n";
#endif
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

	mouse_pos = GetMousePosition();
	//menu bar
	DrawRectangle(0, 0, width, 20, GRAY);
	//GuiTabBar(Rectangle bounds, const char **text, int count, int *active);
	if (GuiDropdownBox({0, 0, 100, 20}, "FILE;SAVE;NEW;OPEN;CLOSE", &ctx.file_action, ctx.file_active))
		ctx.file_active = !ctx.file_active;
	switch (ctx.file_action) {
		case (1): {
			//save map to file
			writeToLevel(engine.level);
			ctx.saved = true;
			ctx.file_action = 0;
			break;
		}
		case (2): {
			//show properties of map
			ctx.new_file = true;
			//ctx.show_properties = true;
			ctx.file_action = 0;
			break;
		}
		case (3): {
			ctx.file_open = true;
			ctx.file_action = 0;
			break;
		}
		case(4): {
			freeLevel(&engine.level);
			ctx.file_action = 0;
			return (1);
			break;
		}
		default: {
			ctx.file_action = 0;
			break;
		}
	}

	if (GuiButton({100, 0, 100, 20}, "SETTING")) {
		ctx.show_properties = true;
	}
	if (GuiDropdownBox({200, 0, 100, 20}, "BRUSH;ERASER;FILL", &ctx.tool_action, ctx.tool_active))
		ctx.tool_active = !ctx.tool_active;
	if (IsCursorHidden()) {
		switch (ctx.tool_action) {
			case (0): {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["BRUSH"]], {0, 0, 32, 32}, {mouse_pos.x, mouse_pos.y}, WHITE);
				ctx.saved = false;
				break;
			}
			case (1): {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["ERASE"]], {0, 0, 32, 32}, {mouse_pos.x, mouse_pos.y}, WHITE);
				ctx.saved = false;
				break;
			}
			case (2): {
				DrawTextureRec(engine.textures[engine.texture_dictionnary["FILL"]], {0, 0, 32, 32}, {mouse_pos.x, mouse_pos.y}, WHITE);
				ctx.saved = false;
				break;
			}
			default:break;
		}
	}

	static Vector2 old;
	static bool move_window = false;

	if (ctx.show_properties) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsMouseInBound(ctx.properties_bound, {ctx.properties_bound.x, ctx.properties_bound.y} , mouse_pos)) {
			move_window = true;
			old = mouse_pos;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && move_window) {
			move_window = false;
		}
		if (move_window) {
			if (Vector2Distance(mouse_pos, old)) {
				ctx.properties_bound.x += mouse_pos.x - old.x;
				ctx.properties_bound.y += mouse_pos.y - old.y;
				old = mouse_pos;
			}
		}
		if (GuiWindowBox(ctx.properties_bound, "Level Properties")) {
			ctx.show_properties = false;
			ctx.properties_bound = {20, 20, 300, 200};
		}
		//height, width, number of layer/depth
		//Name
		//tileset of map 
		//
		if (GuiButton({ctx.properties_bound.x + 220, ctx.properties_bound.y + 150, 30, 20}, "YAY!")) {

		}
		if (GuiButton({ctx.properties_bound.x + 260, ctx.properties_bound.y + 150, 30, 20}, "#152#")) {
			ctx.show_properties = false;
			ctx.properties_bound = {20, 20, 300, 200};
		}
	}

	if (ctx.file_open && !ctx.new_file) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsMouseInBound(ctx.file_bound, {ctx.file_bound.x, ctx.file_bound.y} , mouse_pos)) {
			move_window = true;
			old = mouse_pos;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && move_window) {
			move_window = false;
		}
		if (move_window) {
			if (Vector2Distance(mouse_pos, old)) {
				ctx.file_bound.x += mouse_pos.x - old.x;
				ctx.file_bound.y += mouse_pos.y - old.y;
				old = mouse_pos;
			}
		}
		if (GuiWindowBox(ctx.file_bound, "LoadLevel")) {
			ctx.file_open = false;
			ctx.file_bound = {20, 20, 300, 200};
		}
		//interface
		GuiTextBox({ctx.file_bound.x + 30, ctx.file_bound.y + 70, 200, 24}, ctx.filename, 20, true);
		if (GuiButton({ctx.file_bound.x + 220, ctx.file_bound.y + 140, 30, 20}, "Open")) {
			if (FileExists(TextFormat("level/%s.map", ctx.filename))) {
				ctx.file_open = false;
				openFile(&engine.level, ctx.filename);
				ctx.draw_buffer = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
				ctx.draw_ready = true;
			} else {
				ctx.error = error_file_not_found;
			}
		}
		if (GuiButton({ctx.file_bound.x + 260, ctx.file_bound.y + 140, 30, 20}, "cancel")) {
			ctx.file_open = false;
			ctx.file_bound = {20, 20, 300, 200};
		}
	}

	if (ctx.new_file && !ctx.file_open) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsMouseInBound(ctx.file_bound, {ctx.file_bound.x, ctx.file_bound.y} , mouse_pos)) {
			move_window = true;
			old = mouse_pos;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && move_window) {
			move_window = false;
		}
		if (move_window) {
			if (Vector2Distance(mouse_pos, old)) {
				ctx.file_bound.x += mouse_pos.x - old.x;
				ctx.file_bound.y += mouse_pos.y - old.y;
				old = mouse_pos;
			}
		}
		GuiWindowBox(ctx.file_bound, "New Level");
		bool name = false, dimx = false, dimz = false, dimy = false, tiles = false;
		if (IsMouseInBound({0, 0, 200, 16}, {ctx.file_bound.x + 30, ctx.file_bound.y + 60}, mouse_pos))
			name = true;
		if (IsMouseInBound({0, 0, 50, 16}, {ctx.file_bound.x + 30, ctx.file_bound.y + 90}, mouse_pos))
			dimx = true;
		if (IsMouseInBound({0, 0, 50, 16}, {ctx.file_bound.x + 90, ctx.file_bound.y + 90}, mouse_pos))
			dimz = true;
		if (IsMouseInBound({0, 0, 50, 16}, {ctx.file_bound.x + 150, ctx.file_bound.y + 90}, mouse_pos))
			dimy = true;
		if (IsMouseInBound({0, 0, 200, 16}, {ctx.file_bound.x + 30, ctx.file_bound.y + 120}, mouse_pos))
			tiles = true;
		GuiTextBox({ctx.file_bound.x + 30, ctx.file_bound.y + 60, 200, 16}, ctx.filename, 19, name);
		GuiTextBox({ctx.file_bound.x + 30, ctx.file_bound.y + 90, 50, 16}, ctx.dim_x, 3, dimx);
		GuiTextBox({ctx.file_bound.x + 90, ctx.file_bound.y + 90, 50, 16}, ctx.dim_z, 3, dimz);
		GuiTextBox({ctx.file_bound.x + 150, ctx.file_bound.y + 90, 30, 16}, ctx.dim_y, 2, dimy);
		GuiTextBox({ctx.file_bound.x + 30, ctx.file_bound.y + 120, 200, 16}, ctx.tileset, 19, tiles);
		if (GuiButton({ctx.file_bound.x + 200, ctx.file_bound.y + 150, 60, 20}, "Create")) {
			Vector3 dim = {(float)atoi(ctx.dim_x), (float)atoi(ctx.dim_y), (float)atoi(ctx.dim_z)};
			if (FileExists(TextFormat("level/%s.map", ctx.filename))) {
				ctx.error = error_file_exist;
			} else if (!dim.x || !dim.y || !dim.z){
				ctx.error = error_bad_dimension;
			} else if (!strnlen(ctx.filename, 19)) {
				ctx.error = error_file_empty;
			} else if (!engine.texture_dictionnary.contains(ctx.tileset)) {
				ctx.error = error_tileset_not_found;
				ctx.tileset_ok = false;
			} else {
				engine.level = CreateNewLevel(dim, ctx.filename, ctx.tileset);
				ctx.new_file = false;
				ctx.draw_buffer = LoadRenderTexture(engine.level.dimension.x * 32, engine.level.dimension.y * 32);
				ctx.draw_ready = true;
			}
		}
	}

	if (ctx.new_file && ctx.file_open) {
		ctx.new_file = false;
		ctx.file_open = false;
	}

	if (ctx.error == error_file_not_found) {
		int result = GuiMessageBox(ctx.file_bound, "#152#ERROR!", "this file doesn't exist!!", "OK");
		if (result == 1) {
			ctx.error = error_dummy;
		}
	}
	if (ctx.error == error_file_empty) {
		int result = GuiMessageBox(ctx.file_bound, "#152#ERROR!", "Name Empty!!", "OK");
		if (result == 1) {
			ctx.error = error_dummy;
		}
	}
	if (ctx.error == error_file_corrupted) {
		int result = GuiMessageBox(ctx.file_bound, "#152#ERROR!", "this file is corrupted!!", "OK");
		if (result == 1) {
			ctx.error = error_dummy;
		}
	}
	if (ctx.error == error_file_exist) {
		int result = GuiMessageBox(ctx.file_bound, "#152#ERROR!", "this filename already exist!!", "OK");
		if (result == 1) {
			ctx.error = error_dummy;
		}
	}
	if (ctx.error == error_bad_dimension) {
		int result = GuiMessageBox(ctx.file_bound, "#152#ERROR!", "Level Dimension are not possible!!", "OK");
		if (result == 1) {
			ctx.error = error_dummy;
		}
	}
	if (ctx.error == error_tileset_not_found) {
		int result = GuiMessageBox(ctx.file_bound, "#152#ERROR!", "TileSetNotFound!!", "OK");
		if (result == 1) {
			ctx.error = error_dummy;
		}
	}
	return (0);
}
