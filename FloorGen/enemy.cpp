#include "enemy.h"
#include "mydirect3d.h"


Enemy::Enemy(MESH_NAME mesh, Transform transform) : transform(transform)
{
	this->mesh = GetMesh(mesh);
}

Enemy::~Enemy()
{

}





Enemy* g_enemy;

void RaycastFromMousePos();


void InitEnemy()
{
	g_enemy = new Enemy(MESH_COIN, Transform());
}

void UninitEnemy()
{
	SAFE_DELETE(g_enemy);
}

void UpdateEnemy()
{

}

void DrawEnemy()
{
	auto device = MyDirect3D_GetDevice();

	// draw mesh
	for (DWORD i = 0; i < g_enemy->mesh->numMaterial; i++)
	{
		device->SetMaterial(&g_enemy->mesh->pMaterial[i]);

		if (g_enemy->mesh->pTexture[i] != NULL)
			device->SetTexture(0, g_enemy->mesh->pTexture[i]);
		else
			device->SetTexture(0, NULL);

		g_enemy->mesh->mesh->DrawSubset(i);
	}
}

void RaycastFromMousePos()
{
	auto pDevice = MyDirect3D_GetDevice();

	// get the current transform matrices
	D3DXMATRIX matProjection, matView, matWorld, matInverse;
	pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	pDevice->GetTransform(D3DTS_WORLD, &matWorld);

	// use the mouse coordinates to get the mouse angle
	POINT mousePos;
	GetCursorPos(&mousePos);
	float xAngle = (((2.0f * mousePos.x) / SCREEN_WIDTH) - 1.0f) / matProjection(0, 0);
	float yAngle = (((-2.0f * mousePos.y) / SCREEN_HEIGHT) + 1.0f) / matProjection(1, 1);

	D3DXVECTOR3 origin, direction;
	origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	direction = D3DXVECTOR3(xAngle, yAngle, 1.0f);

	// find the inverse matrix
	D3DXMatrixInverse(&matInverse, NULL, &(matWorld * matView));

	// convert origin and direction into model space
	D3DXVec3TransformCoord(&origin, &origin, &matInverse);
	D3DXVec3TransformNormal(&direction, &direction, &matInverse);
	D3DXVec3Normalize(&direction, &direction);

	// detect picking
	BOOL hit;
	D3DXIntersect(g_enemy->mesh->mesh, &origin, &direction, &hit, NULL, NULL, NULL, NULL, NULL, NULL);
	if (hit)
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	else
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
