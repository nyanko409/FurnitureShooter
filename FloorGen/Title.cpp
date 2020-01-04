#include <Windows.h>
#include <string>
#include "common.h"
#include "sceneManagement.h"
#include "Title.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"


static Sprite g_title;


void InitTitleScreen()
{
	// init title sprite
	g_title = Sprite(Texture_GetTexture(TEXTURE_INDEX_GROUND), D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_GROUND) / 2, Texture_GetHeight(TEXTURE_INDEX_GROUND) / 2 ,0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateTitleScreen()
{
	// switch to game screen when key is pressed
	if (Keyboard_IsPress(DIK_SPACE))
	{
		SetScene(SCENE_GAMESCREEN);
	}
}

void DrawTitleScreen()
{
	// draw title sprite
	SpriteDraw(g_title);
}

void FinalizeTitleScreen()
{

}