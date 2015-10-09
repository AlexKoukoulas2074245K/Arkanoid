#pragma once

#include "transitionshaderclass.h"
#include "shadermanagerclass.h"

class Transition
{
public:
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR4 color;
	};

	Transition();
	~Transition();

	bool Initialize(ComPtr<ID3D11Device> const, const std::shared_ptr<ShaderManager>);
	void Update();
	void Render(ComPtr<ID3D11DeviceContext> const, const std::shared_ptr<ShaderManager>);

	bool getFinished() const;
	bool getActive() const;
	void setActive(const bool);

private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11InputLayout> transLayout;

	bool active;
	float alphaCounter;
};