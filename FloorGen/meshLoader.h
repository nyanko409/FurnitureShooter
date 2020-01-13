#pragma once

#include <d3dx9.h>
#include <string>
#include <map>
#include "mydirect3d.h"
#include "common.h"


// add mesh name to this enum
enum MESH_NAME
{
	MESH_TRASHCAN,
	MESH_TOILET,
	MESH_CAT,
	MESH_STARWARS1,
	MESH_OCTOPUS
};

// data struct for unanimated meshes
struct MESH_DATA
{
	std::string meshPath;
	LPD3DXMESH mesh;
	D3DMATERIAL9* pMaterial;
	LPDIRECT3DTEXTURE9* pTexture;
	DWORD numMaterial;
};


// call once to load every mesh into memory
void LoadMesh();

// free mesh from memory
void UninitMesh();

// get unanimted mesh
MESH_DATA* GetMesh(MESH_NAME name);

// return random mesh
MESH_DATA* GetRandomMesh();
