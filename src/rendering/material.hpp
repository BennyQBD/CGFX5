#pragma once

#include "dataStructures/map.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"

struct MaterialSpec
{
	Map<String, String> textureNames;
	Map<String, float> floats;
	Map<String, Vector3f> vectors;
	Map<String, Matrix> matrices;
};
