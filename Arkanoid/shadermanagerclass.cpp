#include "shadermanagerclass.h"

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{

}

bool ShaderManager::Initialize(ComPtr<ID3D11Device> const device)
{
	defaultShader = std::shared_ptr<DefaultShader>(new DefaultShader);
	plainTexShader = std::shared_ptr<PlainTextureShader>(new PlainTextureShader);
	transShader = std::shared_ptr<TransitionShader>(new TransitionShader);

	if (
		!defaultShader->Initialize(device) |
		!plainTexShader->Initialize(device) |
		!transShader->Initialize(device))
	{
		return false;
	}

	return true;
}

std::shared_ptr<TransitionShader> ShaderManager::getTransitionShader()
{
	return transShader;
}

std::shared_ptr<DefaultShader> ShaderManager::getDefaultShader()
{
	return defaultShader;
}

std::shared_ptr<PlainTextureShader> ShaderManager::getPlainTextureShader()
{
	return plainTexShader;
}
