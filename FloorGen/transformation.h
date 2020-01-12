#pragma once

#include "common.h"


// used for transforming sprites
D3DXMATRIX TransformSprite(D3DXVECTOR3 translate, float rotZ, D3DXVECTOR2 scale = {1.0F, 1.0F}, bool rotateAtPosition = true);

// used for transforming objects
D3DXMATRIX TransformObject(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot);
