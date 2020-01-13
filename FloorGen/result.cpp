#include <Windows.h>
#include <string>
#include "common.h"
#include "sceneManagement.h"
#include "result.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"


static Sprite g_result;


void InitResultScreen()
{
	// init title sprite
	g_result = Sprite(Texture_GetTexture(TEXTURE_INDEX_RESULT), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_RESULT) / 2, Texture_GetHeight(TEXTURE_INDEX_RESULT) / 2, 0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));
}

void UpdateResultScreen()
{
	// switch to game screen when key is pressed
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		SetScene(SCENE_TITLESCREEN);
	}
}

void DrawResultScreen()
{
	// draw title sprite
	SpriteDraw(g_result);
}

void FinalizeResultScreen()
{

}
