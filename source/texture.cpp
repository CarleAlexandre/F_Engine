# include <engine.hpp>

typedef struct textureChunk {
	int height, width;
	Vector2 pos;
	u32 textureId;
} textureChunk;

const Rectangle getTextureRec(const u32 idx, const Texture2D &texture) {
	return ((const Rectangle){getXpos(idx, texture.width), getYpos(idx, texture.width), 32, 32});
}