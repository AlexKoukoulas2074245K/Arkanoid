#include "materialfactoryclass.h"

MaterialFactory::MaterialFactory()
{

}

MaterialFactory::~MaterialFactory()
{

}

bool MaterialFactory::isKeyPresent(const char* const filename)
{
	for (std::map<std::string, std::shared_ptr<Material>>::iterator it = existingMaterials.begin();
		it != existingMaterials.end(); ++it)
	{
		if (it->first.compare(std::string(filename)) == 0)
		{
			return true;
		}
	}

	return false;
}

std::shared_ptr<Material> MaterialFactory::getMaterial(
	ComPtr<ID3D11Device> const device,
	const char* const filename)
{
	if (isKeyPresent(filename))
	{
		return existingMaterials.at(filename);
	}

	std::shared_ptr<Material> newmat(new Material);

	if (!newmat->Initialize(device, filename))
	{
		MessageBox(NULL, "Material Factory failed to create new material", FATALERROR, FATALLAYOUT);
		return nullptr;
	}

	existingMaterials.insert(std::pair<std::string, std::shared_ptr<Material>>(std::string(filename), newmat));

	return newmat;
}