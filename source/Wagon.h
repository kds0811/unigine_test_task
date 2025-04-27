#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Object;
class Engine;
class Mesh;
class SplinePath;

class Wagon
{
	// Pointers that do not own memory. Caching since every frame is needed.
	Object* pObject = nullptr;
	SplinePath* pSplinePath = nullptr;

	// moving fields
	static constexpr float mSpeed = 0.5f;
	int mDestinationIndex = -1;
	glm::vec3 mDestinationPosition{};
	glm::vec3 mForwardVector{};

	// rotation fields
	float mStartYaw = 0.0f;
	float mDestYaw = 0.0f;
	float mRotationSpeed = 0.5f;
	float mCurrentRotationProgress = 0.0f;
	bool mIsRotating = false;

public:
	Wagon();
	~Wagon();
	void Init(Engine* engine, Mesh* mesh, const glm::vec3& startPos, int destinationIndex, const glm::vec3& destinationPosition, SplinePath* path);
	void Update(float deltaTime);

private:
	void UpdatePosition(float deltaTime);
	void UpdateRotation(float deltaTime);
	bool IsDestinationAhead(const glm::vec3& vecToDest) const;
	void MoveTowardsDestination(const glm::vec3& vecToDest, float deltaTime);
	void StartRotation();

};