#pragma once
#include "materialclass.h"
#include <map>

class MaterialFactory
{
public:
	MaterialFactory();
	~MaterialFactory();

	std::shared_ptr<Material> getMaterial(ComPtr<ID3D11Device> const, const char* const);

private:
	bool isKeyPresent(const char* const);

private:
	std::map<std::string, std::shared_ptr<Material>> existingMaterials;
};