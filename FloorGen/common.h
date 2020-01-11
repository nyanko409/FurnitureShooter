#pragma once

#include <d3dx9.h>

/*-----------------------------------------------------------------------
   ’è”’è‹`
------------------------------------------------------------------------*/
#define CUSTOM_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/*
	FULLSCREEN: 1440 x 900 (wide)
	WINDOWED:	1920 x 1080
*/
#define FULLSCREEN		true
#define SCREEN_WIDTH	(1440)
#define SCREEN_HEIGHT	(900)

#define FPS				60
#define TIME_PER_FRAME	(float)1/FPS

#define SAFE_RELEASE(p)			if(p != nullptr) { p->Release(); p = nullptr; } 
#define SAFE_DELETE(p)			if(p != nullptr) { delete p; p = nullptr; } 
#define SAFE_DELETE_ARRAY(p)	if(p != nullptr) { delete[] p; p = nullptr; } 

// custom vertex struct
struct CUSTOM_VERTEX
{
	float x, y, z;
	D3DVECTOR normal;
	DWORD color;
	D3DXVECTOR2 uv;
};

class Transform
{
public:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;

	Transform(D3DXVECTOR3 position = { 0, 0, 0 },
		D3DXVECTOR3 rotation = { 0, 0, 0 }, D3DXVECTOR3 scale = { 1, 1, 1 }) :
		position(position), rotation(rotation), scale(scale) {}
};
