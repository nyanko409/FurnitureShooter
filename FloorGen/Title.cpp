#include <Windows.h>
#include <string>
#include <vector>
#include "common.h"
#include "sceneManagement.h"
#include "Title.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "enemy.h"
#include "transformation.h"
#include "meshLoader.h"
#include "font.h"


static Sprite g_title;
static std::vector<Enemy*> g_enemy;


void InitTitleScreen()
{
	// init title sprite
	g_title = Sprite(Texture_GetTexture(TEXTURE_INDEX_TITLE), D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_TITLE) / 2, Texture_GetHeight(TEXTURE_INDEX_TITLE) / 2 ,0),
		0, D3DXVECTOR2(1, 1), D3DCOLOR_RGBA(255, 255, 255, 255));

	// init enemies to show in title screen
	Transform transform{ {-10, 3, 15} };
	g_enemy.emplace_back(new Enemy(MESH_CAT, transform, 0.0F));

	transform = { {0, 3, 15} };
	g_enemy.emplace_back(new Enemy(MESH_OCTOPUS, transform, 0.0F));

	transform = { {10, 3, 15} };
	g_enemy.emplace_back(new Enemy(MESH_TRASHCAN, transform, 0.0F));

	transform = { {-10, -6, 15} };
	g_enemy.emplace_back(new Enemy(MESH_STARWARS1, transform, 0.0F));

	transform = { {0, -6, 15} };
	g_enemy.emplace_back(new Enemy(MESH_TOILET, transform, 0.0F));

	transform = { {10, -6, 15} };
	g_enemy.emplace_back(new Enemy(MESH_BANANA, transform, 0.0F));
}

void UpdateTitleScreen()
{
	// switch to game screen when key is pressed
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		SetScene(SCENE_GAMESCREEN);
	}

	for (auto enemy : g_enemy)
	{
		enemy->transform.rotation.y++;
	}
}

void DrawTitleScreen()
{
	char f[] = "THE SURVIVOR";
	DrawTextTo(RECT{SCREEN_WIDTH / 2 - 250, 120, 100, 50}, f, sizeof(f) / sizeof(char));

	auto device = MyDirect3D_GetDevice();

	for (auto enemy : g_enemy)
	{
		for (int j = 0; j < g_enemy.size(); ++j)
		{
			// set world matrix
			auto world = TransformObject(g_enemy[j]->transform.position, g_enemy[j]->transform.scale,
				g_enemy[j]->transform.rotation);
			device->SetTransform(D3DTS_WORLD, &world);

			// draw
			for (DWORD i = 0; i < g_enemy[j]->mesh->numMaterial; ++i)
			{
				device->SetMaterial(&g_enemy[j]->mesh->pMaterial[i]);

				if (g_enemy[j]->mesh->pTexture[i] != NULL)
					device->SetTexture(0, g_enemy[j]->mesh->pTexture[i]);
				else
					device->SetTexture(0, NULL);

				g_enemy[j]->mesh->mesh->DrawSubset(i);
			}
		}
	}
}

void FinalizeTitleScreen()
{
	for (auto enemy : g_enemy)
	{
		SAFE_DELETE(enemy);
	}

	g_enemy.clear();
}
