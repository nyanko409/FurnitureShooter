#pragma once

#include "common.h"
#include "meshLoader.h"


class Enemy
{
public:
	Transform transform;
	MESH_DATA* mesh;

	Enemy(MESH_NAME mesh, Transform transform);

	~Enemy();
};






void InitEnemy();
void UninitEnemy();
void UpdateEnemy();
void DrawEnemy();
