#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <map>
#include "common.h"

struct LevelLayout
{
	float xOffset, yOffset;
	unsigned int effectType;
	std::vector<std::vector<int>> structure;
};

struct IndexedModel
{
	std::vector<indexType> indices;
	std::vector<Vertex> vertices;
	float maxWidth, maxHeight, maxDepth;
};

namespace details
{
	static std::map<std::string, IndexedModel> bufferedModels;
}

namespace disorganizedData
{
	struct disorganized_Vertex{ float x, y, z; };
	struct disorganized_Texcoord{ float u, v; };
	struct disorganized_Normal{ float nx, ny, nz; };
}

namespace organizedData
{
	struct OBJIndex{ indexType vertexIndex, texcoordIndex, normalIndex; };
}

bool LoadOBJFromFile(const char* const filename, IndexedModel& outModel);
bool LoadLevelFromFile(const char* const filename, LevelLayout& outLayout);