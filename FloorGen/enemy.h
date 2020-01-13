#pragma once

#include "common.h"
#include "meshLoader.h"


class Enemy
{
public:
	Transform transform;
	MESH_DATA* mesh;
	float moveSpeed;

	Enemy(MESH_NAME mesh, Transform transform, float moveSpeed);
	Enemy(MESH_DATA* mesh, Transform transform, float moveSpeed);

	~Enemy();
};






void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
void DrawEnemy();
