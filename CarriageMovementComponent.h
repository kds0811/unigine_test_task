#pragma once
#include <glm/vec3.hpp>

class CarriageMovementComponent
{
	static constexpr float mSpeed = 10.0f;
	glm::vec3 mPosition{};
	glm::vec3 mRotation{};

public:
	CarriageMovementComponent() = default;
	CarriageMovementComponent(glm::vec3 startPos, glm::vec3 startRot);


	void Update(float deltaTime, glm::vec3 direction);

};