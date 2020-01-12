#include "common.h"
#include "crosshair.h"
#include "texture.h"
#include "sprite.h"


static Sprite crosshair;


void InitCrosshair()
{
	crosshair = Sprite(Texture_GetTexture(TEXTURE_INDEX_CROSSHAIR),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_CROSSHAIR) / 2, 
					Texture_GetHeight(TEXTURE_INDEX_CROSSHAIR) / 2, 0));
}

void UninitCrosshair()
{

}

void UpdateCrosshair()
{
	crosshair.rotZ++;
}

void DrawCrosshair()
{
	SpriteDraw(crosshair);
}
