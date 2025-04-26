#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Object;
class Engine;
class Mesh;
class SplinePath;

class Carriage
{
	Object* pObject = nullptr;
	SplinePath* pSplinePath = nullptr;
	static constexpr float mSpeed = 0.5f;
	int mDestinationIndex = -1;
	glm::vec3 mDestinationPosition{};
	glm::vec3 mCurrentDirection{};


public:
	Carriage();
	~Carriage();
	void Init(Engine* engine, Mesh* mesh, const glm::vec3& startPos, int destinationIndex, const glm::vec3& destinationPosition, SplinePath* path);
	void Update(float deltaTime);


private:
	glm::quat CalculateDirectionQuat(const glm::vec3& direction);
};