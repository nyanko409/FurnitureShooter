#include <vector>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "meshLoader.h"

// unanimated mesh data, add path to x file
std::vector<MESH_DATA> g_mesh
{
	{"asset/mesh/medal_gold.x"},
	{"asset/mesh/skateboard.x"},
};


void LoadMesh()
{
	// get device
	auto device = MyDirect3D_GetDevice();
	auto paths = std::vector<std::string>();
	auto tex = std::vector<LPDIRECT3DTEXTURE9>();

	// load every unanimated mesh
	for (int i = 0; i < g_mesh.size(); i++)
	{
		// load mesh from path
		LPD3DXBUFFER matBuffer;
		D3DXLoadMeshFromX(g_mesh[i].meshPath.c_str(), D3DXMESH_SYSTEMMEM, device, NULL, &matBuffer, NULL, &g_mesh[i].numMaterial, &g_mesh[i].mesh);

		// set materials and textures
		D3DXMATERIAL* temp = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
		g_mesh[i].pMaterial = new D3DMATERIAL9[g_mesh[i].numMaterial];
		g_mesh[i].pTexture = new LPDIRECT3DTEXTURE9[g_mesh[i].numMaterial];
		std::string path;

		// loop for every material and set material and texture
		for (DWORD j = 0; j < g_mesh[i].numMaterial; j++)
		{
			g_mesh[i].pMaterial[j] = temp[j].MatD3D;
			g_mesh[i].pMaterial[j].Ambient = g_mesh[i].pMaterial[j].Diffuse;

			if (temp[j].pTextureFilename != NULL)
			{
				path = "asset/texture/_mesh/";
				path += temp[j].pTextureFilename;
			}
			else
				path = "";

			// always load first texture
			if (paths.size() == 0)
			{
				if (FAILED(D3DXCreateTextureFromFileA(device, path.c_str(), &g_mesh[i].pTexture[j])))
					g_mesh[i].pTexture[j] = NULL;
				else
				{
					paths.emplace_back(path);
					tex.emplace_back(g_mesh[i].pTexture[j]);
				}
			}

			// check if the texture is already created, when true point it to the texture
			else
			{
				bool exists = false;
				for (int k = 0; k < paths.size(); ++k)
				{
					if (path == paths[k])
					{
						// texture already exists, point to it
						exists = true;
						g_mesh[i].pTexture[j] = tex[k];
						break;
					}
				}

				// create new from path
				if (!exists)
				{
					if (FAILED(D3DXCreateTextureFromFileA(device, path.c_str(), &g_mesh[i].pTexture[j])))
						g_mesh[i].pTexture[j] = NULL;
					else
					{
						paths.emplace_back(path);
						tex.emplace_back(g_mesh[i].pTexture[j]);
					}
				}
			}
		}
	}
}

MESH_DATA* GetMesh(MESH_NAME name)
{
	return &g_mesh[name];
}

void UninitMesh()
{
	// free unanimated mesh
	for (auto data : g_mesh)
	{
		SAFE_RELEASE(data.mesh);
		SAFE_DELETE_ARRAY(data.pMaterial);
		SAFE_DELETE_ARRAY(data.pTexture);
	}

	g_mesh.clear();
}
