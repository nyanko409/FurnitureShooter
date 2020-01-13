#pragma once

#include "common.h"
#include "texture.h"


struct PlaneData
{
	LPDIRECT3DVERTEXBUFFER9 vBuffer;
	LPDIRECT3DINDEXBUFFER9 iBuffer;
	CUSTOM_VERTEX* vert;
	short* indices;
	int vertCount = 0;
	int indexCount = 0;
};

class Plane
{
public:
	PlaneData* data;				// holds vertex data for the plane
	LPDIRECT3DTEXTURE9 texuture;	// pointer to the texture to display
	Transform transform;

	// constructor for creating new plane
	Plane(Transform transform, int zCount, int xCount, float xLength, float zLength);
	Plane() {}
	// copy constructor
	Plane(Plane& plane);

	// destructor
	~Plane();

private:
	// create plane data
	void CreatePlane(int z, int x, float width, float height);
};



void InitPlane();
void UninitPlane();
void DrawPlane();
