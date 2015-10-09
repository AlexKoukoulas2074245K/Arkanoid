#pragma once
#include <math.h>
#include "modelclass.h"

#define COLL_FAIL 0
#define COLL_VER 1
#define COLL_HOR 2

enum Shape
{
	SHAPE_CIRCLE,
	SHAPE_RECTANGLE
};

int collides(
	const Shape& shape1,
	const Model::Transform& transform1,
	const Model::Dimensions& dims1,
	const Shape& shape2,
	const Model::Transform& transform2,
	const Model::Dimensions& dims2);