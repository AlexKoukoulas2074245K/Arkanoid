#include "i_gameobjectclass.h"

GameObject::GameObject()
{
	LOG("Game object empty constructor \n");
}

GameObject::GameObject(const GOSpec& spec)
{
	LOG("Game object spec constructor \n");
	LOG((std::to_string(spec.ry) + "\n").c_str());
}