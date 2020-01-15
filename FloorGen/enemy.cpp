#include <vector>
#include "sound.h"
#include "enemy.h"
#include "mydirect3d.h"
#include "transformation.h"
#include "camera.h"
#include "score.h"
#include "sceneManagement.h"


Enemy::Enemy(MESH_NAME mesh, Transform transform, float moveSpeed) : 
	transform(transform), moveSpeed(moveSpeed), mesh(GetMesh(mesh)) {}

Enemy::Enemy(MESH_DATA* mesh, Transform transform, float moveSpeed) :
	transform(transform), moveSpeed(moveSpeed), mesh(mesh) {}

Enemy::~Enemy() {}





std::vector<Enemy*> g_enemy;

std::vector<int> RaycastFromMousePos();
void SpawnEnemyRandomAt(D3DXVECTOR3 position, float radius);


void InitEnemy()
{
	PlaySound(SOUND_GAMEBGM);
}

void UninitEnemy()
{
	for (auto enemy : g_enemy)
	{
		SAFE_DELETE(enemy);
	}

	StopSound(SOUND_GAMEBGM);

	g_enemy.clear();
}

void UpdateEnemy()
{
	static bool leftMouseClicked = false;
	static float timeBetweenSpawn = 1.0F;
	static float timeTillNextSpawn = 0;

	// spawn enemy
	timeTillNextSpawn += TIME_PER_FRAME;
	if (timeTillNextSpawn >= timeBetweenSpawn)
	{
		timeTillNextSpawn -= timeBetweenSpawn;
		SpawnEnemyRandomAt(GetCamera()->position, 30);
	}

	// check for input and delete enemies hit by raycast
	if (!leftMouseClicked && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		// play gun sound
		PlaySound(SOUND_GUN);

		// left mouse clicked, get index of all enemies hit by ray
		leftMouseClicked = true;
		auto hitIndices = RaycastFromMousePos();

		// loop from back and delete objects hit by the ray
		for (int i = hitIndices.size() - 1; i >= 0; --i)
		{
			g_enemy.erase(g_enemy.begin() + hitIndices[i]);
			PlaySound(SOUND_EXPLOSION);
			AddScore(10);
		}  
	}
	else if(!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		// left mouse released
		leftMouseClicked = false;
	}

	// loop for every enemy
	for (int i = 0; i < g_enemy.size(); ++i)
	{
		// move enemy towards the camera
		D3DXVECTOR3 dir = GetCamera()->position - g_enemy[i]->transform.position;
		D3DXVECTOR3 dirNormalized;
		D3DXVec3Normalize(&dirNormalized, &dir);

		g_enemy[i]->transform.position += dirNormalized * g_enemy[i]->moveSpeed;

		// rotate enemy
		g_enemy[i]->transform.rotation.x++;

		// enemy reached camera, game over
		if (D3DXVec3Length(&dir) < 2.0F)
		{
			SetScene(SCENE_RESULTSCREEN);
		}
	}
}

void DrawEnemy()
{
	auto device = MyDirect3D_GetDevice();

	// draw mesh
	for (int j = 0; j < g_enemy.size(); j++)
	{
		// set world matrix
		auto world = TransformObject(g_enemy[j]->transform.position, g_enemy[j]->transform.scale,
			g_enemy[j]->transform.rotation);
		device->SetTransform(D3DTS_WORLD, &world);

		// draw
		for (DWORD i = 0; i < g_enemy[j]->mesh->numMaterial; i++)
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

std::vector<int> RaycastFromMousePos()
{
	auto pDevice = MyDirect3D_GetDevice();
	auto indices = std::vector<int>();

	// get the current transform matrices
	D3DXMATRIX matProjection, matView, matWorld, matInverse;
	pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	pDevice->GetTransform(D3DTS_VIEW, &matView);

	// use the mouse coordinates to get the mouse angle
	POINT mousePos;
	GetCursorPos(&mousePos);
	float xAngle = (((2.0f * mousePos.x) / SCREEN_WIDTH) - 1.0f) / matProjection(0, 0);
	float yAngle = (((-2.0f * mousePos.y) / SCREEN_HEIGHT) + 1.0f) / matProjection(1, 1);

	// loop for every enemy
	for (int i = 0; i < g_enemy.size(); ++i)
	{
		// set world transform
		matWorld = TransformObject(g_enemy[i]->transform.position, g_enemy[i]->transform.scale,
			g_enemy[i]->transform.rotation);

		// find the inverse matrix
		D3DXMatrixInverse(&matInverse, NULL, &(matWorld * matView));

		// convert origin and direction into model space
		D3DXVECTOR3 origin, direction;
		D3DXVec3TransformCoord(&origin, &D3DXVECTOR3{0, 0, 0}, &matInverse);
		D3DXVec3TransformNormal(&direction, &D3DXVECTOR3{xAngle, yAngle, 1}, &matInverse);
		D3DXVec3Normalize(&direction, &direction);

		// cast ray
		BOOL hit;
		D3DXIntersect(g_enemy[i]->mesh->mesh, &origin, &direction, &hit, NULL, NULL, NULL, NULL, NULL, NULL);
		if (hit)
			indices.emplace_back(i);
	}

	return indices;
}

void SpawnEnemyRandomAt(D3DXVECTOR3 position, float radius)
{
	// init position
	D3DXVECTOR3 pos{radius, 0, 0};

	// rotate final pos by random z and random y degree
	D3DXMATRIX mRotZ, mRotY;
	D3DXMatrixRotationZ(&mRotZ, D3DXToRadian(rand() % 46));
	D3DXMatrixRotationY(&mRotY, D3DXToRadian(rand() % 361));
	D3DXVec3TransformCoord(&pos, &pos, &(mRotZ * mRotY));

	// offset to final position
	pos += position;

	// get random rotation
	D3DXVECTOR3 rot(rand() % 361, rand() % 361 , rand() % 361);

	// spawn random enemy
	g_enemy.emplace_back(new Enemy(GetRandomMesh(), Transform(pos, rot), 0.2F));
}
