#pragma once

#include "common.h"

class Camera
{
public:
	enum Direction
	{
		FORWARD, BACKWARD,
		RIGHT, LEFT,
		UP, DOWN
	};

	Camera();
	~Camera();
	
	void Initialize(const D3DXVECTOR3&);
	void Update();

	void Move(const Direction&, const float);
	void Look(const Direction&, const float);
	void Tilt(const float);

	D3DXMATRIX getViewMatrix();
	D3DXVECTOR3 getPosition() const;

private:
	D3DXVECTOR3 position; // camera position
	float yaw, pitch, roll; // rotation around axes
	D3DXVECTOR3 up, look, right; // camera 
};