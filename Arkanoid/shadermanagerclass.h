#pragma once
#include "defaultshaderclass.h"
#include "plaintexshaderclass.h"
#include "transitionshaderclass.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	bool Initialize(ComPtr<ID3D11Device> const);

	std::shared_ptr<TransitionShader> getTransitionShader();
	std::shared_ptr<DefaultShader> getDefaultShader();
	std::shared_ptr<PlainTextureShader> getPlainTextureShader();
	
private:
	std::shared_ptr<TransitionShader> transShader;
	std::shared_ptr<DefaultShader> defaultShader;
	std::shared_ptr<PlainTextureShader> plainTexShader;
};