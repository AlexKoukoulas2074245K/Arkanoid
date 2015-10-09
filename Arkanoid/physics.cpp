#include "physics.h"

int collides(
	const Shape& shape1,
	const Model::Transform& transform1,
	const Model::Dimensions& dims1,
	const Shape& shape2,
	const Model::Transform& transform2,
	const Model::Dimensions& dims2)
{
	// In all the cases dealing with circles, division by 2 for width and height is necessary
	// because the width and height in the Dimensions struct for sphere models
	// are the diameters and we want the radii instead
	if (shape1 == SHAPE_CIRCLE && shape2 == SHAPE_CIRCLE)
	{
		float distance = std::sqrtf(std::powf(transform2.translation.x - transform1.translation.x, 2) + 
									std::powf(transform2.translation.y - transform1.translation.y, 2));
		
		return distance < dims1.width / 2 + dims2.width / 2;
	}
	else if (shape1 == SHAPE_CIRCLE && shape2 == SHAPE_RECTANGLE)
	{
		float distanceX = std::abs(transform1.translation.x - transform2.translation.x);
		float distanceY = std::abs(transform1.translation.y - transform2.translation.y);

		if (distanceX > dims2.width / 2 + dims1.width / 2) return COLL_FAIL;
		if (distanceY > dims2.height / 2 + dims1.height / 2) return COLL_FAIL;
		
		if (distanceX <= dims2.width / 2) return COLL_VER;
		if (distanceY <= dims2.height / 2) return COLL_HOR;
		
		return false;
		// Corner collision check
		/*float cornerDistance = std::powf(distanceX - dims2.width / 2, 2) +
							   std::powf(distanceY - dims2.height / 2, 2);

		return cornerDistance <= std::powf(dims1.width / 2, 2); */
	}
	else if (shape1 == SHAPE_RECTANGLE && shape2 == SHAPE_CIRCLE)
	{
		return collides(shape2, transform2, dims2, shape1, transform1, dims1);
	}
	else // Rect vs Rect
	{
		return transform1.translation.x < transform2.translation.x + dims2.width &&
			   transform1.translation.x + dims1.width > transform2.translation.x &&
			   transform1.translation.y < transform2.translation.y + dims2.height &&
			   transform1.translation.y + dims1.height > transform2.translation.y;
	}
}