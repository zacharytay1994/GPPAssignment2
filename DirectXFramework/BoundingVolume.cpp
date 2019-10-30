#include "BoundingVolume.h"
#include "TestObject.h"

void BoundingVolume::ResolveCollision(TestObject a, TestObject b)
{
	// calculate relative velocity
	Vec2<float> relative_velocity = b.velocity_ - a.velocity_;

	// calculate relative velocity in terms of normal direction
	float normal_magnitude = relative_velocity * Vec2<float>( 0.0f, 0.0f );
}
