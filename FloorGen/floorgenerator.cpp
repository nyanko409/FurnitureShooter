#include "mydirect3d.h"
#include "floorgenerator.h"



Plane::Plane(int xCount, int zCount, float xLength, float zLength)
{
	data = new PlaneData();
	CreatePlane(xCount, zCount, xLength, zLength);
}

Plane::~Plane()
{
	// free memory
	if (data != nullptr)
	{
		if (data->vBuffer != nullptr) data->vBuffer->Release();
		if (data->iBuffer != nullptr) data->iBuffer->Release();

		if (data->vert != nullptr) delete data->vert;
		if (data->indices != nullptr) delete data->indices;

		delete data;
	}
}

void Plane::CreatePlane(int x, int z, float xLength, float zLength)
{
	// init
	auto pDevice = MyDirect3D_GetDevice();

	float startX = -((xLength * x) / 2), startZ = ((zLength * z) / 2);
	int oneRowVert = 4 + (x - 1) * 2;

	data->vertCount = oneRowVert + (oneRowVert / 2) * (z - 1);
	data->indexCount = oneRowVert * z + (z - 1) * 2;

	// init vertex
	data->vert = new CUSTOM_VERTEX[data->vertCount];

	int curIndex = 0;
	float curU = 0, u = 1.0F / x;
	float curV = 0, v = 1.0F / z;
	int curRow = 0, curColumn = 0;
	for (int i = 0; i <= z; i++)
	{
		for (int j = 0; j <= x; j++)
		{
			data->vert[curIndex] = { startX + curColumn * xLength, 0, startZ - curRow * zLength, D3DVECTOR{0, 1, 0}, D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(curV, curU) };
			
			curU += u;
			curColumn++;
			curIndex++;
		}

		curV += v;
		curU = 0;
		curColumn = 0;
		curRow++;
	}

	// init indices
	data->indices = new short[data->indexCount];

	curIndex = 0;
	int ind1 = x + 1, ind2 = 0;
	for (int i = 0; i < z; i++)
	{
		for (int j = 0; j < oneRowVert / 2; j++)
		{
			data->indices[curIndex++] = ind1++;
			data->indices[curIndex++] = ind2++;
		}

		if (z <= 1) break;
		if (i == z - 1) break;
		data->indices[curIndex++] = ind2 - 1;
		data->indices[curIndex++] = ind1;
	}


	VOID* pVoid;

	// create a vertex buffer interface called v_buffer
	pDevice->CreateVertexBuffer(sizeof(CUSTOM_VERTEX) * data->vertCount,
		0,
		CUSTOM_FVF,
		D3DPOOL_MANAGED,
		&data->vBuffer,
		NULL);

	// create a index buffer interface called i_buffer
	pDevice->CreateIndexBuffer(sizeof(short) * data->indexCount,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&data->iBuffer,
		NULL);


	// lock v_buffer and load the vertices into it
	data->vBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, data->vert, sizeof(CUSTOM_VERTEX) * data->vertCount);
	data->vBuffer->Unlock();

	// lock i_buffer and load the indices into it
	data->iBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, data->indices, sizeof(short) * data->indexCount);
	data->iBuffer->Unlock();
}



// floor
Plane* plane;
Plane* plane2;


void InitPlane()
{
	plane = new Plane(10, 10, 5, 5);
	plane->texuture = Texture_GetTexture(TEXTURE_INDEX_GROUND);
	plane2 = new Plane(2,6,1,1);
}

void UninitPlane()
{
	// free used memory
	delete plane;
}

void DrawPlane()
{
	auto pDevice = MyDirect3D_GetDevice();

	// select the vertex and index buffer to display
	pDevice->SetFVF(CUSTOM_FVF);
	pDevice->SetStreamSource(0, plane->data->vBuffer, 0, sizeof(CUSTOM_VERTEX));
	pDevice->SetIndices(plane->data->iBuffer);

	// set the world transform
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	pDevice->SetTransform(D3DTS_WORLD, &identity);

	// draw
	pDevice->SetTexture(0, plane->texuture);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 
		plane->data->vertCount, 0, plane->data->indexCount - 2);
}
