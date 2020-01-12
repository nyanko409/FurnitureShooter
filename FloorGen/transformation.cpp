#include "transformation.h"
#include "mydirect3d.h"




D3DXMATRIX TransformObject(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 rot)
{
	D3DXMATRIX matTranslate, matRotation, matScale;

	// get device
	auto device = MyDirect3D_GetDevice();

	// set translation and scaling matrix
	D3DXMatrixTranslation(&matTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// rotation matrix
	D3DXMATRIX xRot, yRot, zRot;
	D3DXMatrixRotationX(&xRot, D3DXToRadian(rot.x));
	D3DXMatrixRotationY(&yRot, D3DXToRadian(rot.y));
	D3DXMatrixRotationZ(&zRot, D3DXToRadian(rot.z));

	matRotation = xRot * yRot * zRot;

	// calculate and return the world matrix
	return (matScale * matRotation * matTranslate);
}


D3DXMATRIX TransformSprite(D3DXVECTOR3 translate, float rotZ, D3DXVECTOR2 scale, bool rotateAtPosition)
{
	D3DXMATRIX matTranslate, matRotation, matScale;

	// get device
	auto device = MyDirect3D_GetDevice();

	// set translation, rotation and scale matrix
	D3DXMatrixTranslation(&matTranslate, translate.x, translate.y, translate.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, 1.0F);
	D3DXMatrixRotationZ(&matRotation, D3DXToRadian(rotZ));

	// return the rotation matrix
	if (!rotateAtPosition)
		return (matScale * matTranslate * matRotation);
	else
		return (matScale * matRotation * matTranslate);
}
