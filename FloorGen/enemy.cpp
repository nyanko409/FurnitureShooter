#include <vector>
#include "enemy.h"
#include "mydirect3d.h"
#include "transformation.h"


Enemy::Enemy(MESH_NAME mesh, Transform transform) : transform(transform)
{
	this->mesh = GetMesh(mesh);
}

Enemy::~Enemy()
{

}





std::vector<Enemy*> g_enemy;

int RaycastFromMousePos();


void InitEnemy()
{
	g_enemy.emplace_back(new Enemy(MESH_COIN, Transform()));
	g_enemy.emplace_back(new Enemy(MESH_SKATEBOARD, Transform({20, 0, 0})));
}

void UninitEnemy()
{
	for (auto enemy : g_enemy)
	{
		SAFE_DELETE(enemy);
	}

	g_enemy.clear();
}

void UpdateEnemy()
{
	// check for left mouse input
	if (GetAsyncKeyState(VK_LBUTTON) < 0)
	{
		int hitIndex = RaycastFromMousePos();

		// if hit with something
		if (hitIndex != -1)
		{
			g_enemy.erase(g_enemy.begin() + hitIndex);
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

int RaycastFromMousePos()
{
	auto pDevice = MyDirect3D_GetDevice();

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
		D3DXVECTOR3 origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 direction = D3DXVECTOR3(xAngle, yAngle, 1.0f);

		// set world transform
		matWorld = TransformObject(g_enemy[i]->transform.position, g_enemy[i]->transform.scale,
			g_enemy[i]->transform.rotation);

		// find the inverse matrix
		D3DXMatrixInverse(&matInverse, NULL, &(matWorld * matView));

		// convert origin and direction into model space
		D3DXVec3TransformCoord(&origin, &origin, &matInverse);
		D3DXVec3TransformNormal(&direction, &direction, &matInverse);
		D3DXVec3Normalize(&direction, &direction);

		// cast ray
		BOOL hit;
		D3DXIntersect(g_enemy[i]->mesh->mesh, &origin, &direction, &hit, NULL, NULL, NULL, NULL, NULL, NULL);
		if (hit)
			return i;
	}

	return -1;
}
