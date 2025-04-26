#include "CarriageMovementComponent.h"

using namespace glm;

CarriageMovementComponent::CarriageMovementComponent(glm::vec3 startPos, glm::vec3 startRot)
	:
	mPosition(startPos),
	mRotation(startRot)
{
}

void CarriageMovementComponent::Update(float deltaTime, glm::vec3 direction)
{
	vec3 offset = direction * deltaTime * mSpeed;
	mPosition += offset;

	
}
