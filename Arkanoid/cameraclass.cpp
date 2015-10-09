#include "cameraclass.h"

Camera::Camera()
{

}

Camera::~Camera()
{
	LOG("Deleting Camera...\n");
}

void Camera::Initialize(const D3DXVECTOR3& pos)
{
	position = pos;
	yaw = 3.14f;
	pitch = 0.0f;
	roll = 0.0f;
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

void Camera::Update()
{

}

void Camera::Move(const Camera::Direction& dir, const float amount)
{
	switch (dir)
	{
	case FORWARD:
		position += look * amount;
		break;

	case BACKWARD:
		position += look * -amount;
		break;
		
	case RIGHT:
		position += right * amount;
		break;

	case LEFT:
		position += right * -amount;
		break;

	case UP:
		position += up * amount;
		break;

	case DOWN:
		position += up * -amount;
		break;
	}
}

void Camera::Look(const Camera::Direction& dir, const float amount)
{
	switch (dir)
	{
	case RIGHT:
		yaw += amount;
		break;

	case LEFT:
		yaw -= amount;
		break;

	case UP:
		pitch -= amount;
		break;

	case DOWN:
		pitch += amount;
		break;
	}
}

D3DXVECTOR3 Camera::getPosition() const
{
	return position;
}


D3DXMATRIX Camera::getViewMatrix()
{	
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	D3DXMATRIX yawMatrix;
	D3DXMatrixRotationAxis(&yawMatrix, &up, yaw);
	D3DXVec3TransformCoord(&look, &look, &yawMatrix);
	D3DXVec3TransformCoord(&right, &right, &yawMatrix);

	D3DXMATRIX pitchMatrix;
	D3DXMatrixRotationAxis(&pitchMatrix, &right, pitch);
	D3DXVec3TransformCoord(&look, &look, &pitchMatrix);
	D3DXVec3TransformCoord(&up, &up, &pitchMatrix);

	D3DXMATRIX rollMatrix;
	D3DXMatrixRotationAxis(&rollMatrix, &look, roll);
	D3DXVec3TransformCoord(&right, &right, &rollMatrix);
	D3DXVec3TransformCoord(&up, &up, &rollMatrix);

	D3DXMATRIX viewMatrix;
	D3DXMatrixIdentity(&viewMatrix);

	viewMatrix._11 = right.x;  viewMatrix._12 = up.x;  viewMatrix._13 = look.x;
	viewMatrix._21 = right.y;  viewMatrix._22 = up.y;  viewMatrix._23 = look.y;
	viewMatrix._31 = right.z;  viewMatrix._32 = up.z;  viewMatrix._33 = look.z;

	viewMatrix._41 = -D3DXVec3Dot(&position, &right);
	viewMatrix._42 = -D3DXVec3Dot(&position, &up);
	viewMatrix._43 = -D3DXVec3Dot(&position, &look);

	return viewMatrix;
}