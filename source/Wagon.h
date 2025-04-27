#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Object;
class Engine;
class Mesh;
class SplinePath;

class Wagon
{
	Object* pObject = nullptr;
	SplinePath* pSplinePath = nullptr;

	static constexpr float mSpeed = 0.5f;
	int mDestinationIndex = -1;
	glm::vec3 mDestinationPosition{};
	glm::vec3 mForwardVector{};

	float mStartYaw{};
	float mDestYaw{};
	float mRotationSpeed = 0.1f;
	float mCurrentProgress = 0.0f;
	bool mIsRotating = false;

public:
	Wagon();
	~Wagon();
	void Init(Engine* engine, Mesh* mesh, const glm::vec3& startPos, int destinationIndex, const glm::vec3& destinationPosition, SplinePath* path);
	void Update(float deltaTime);

private:
	void UpdateMovement(float deltaTime);
	void UpdateRotation(float deltaTime);
	bool DestinationIsFront(const glm::vec3& vecToDest) const;
	void CalculateOffsetAndSetPosition(const glm::vec3& vecToDest, float deltaTime);
	void StartRotation();

};