#pragma once
#include <glm/vec3.hpp>

class Engine;
class Mesh;
class SplinePath;

class Rails
{
	static constexpr float mRailsWidth = 0.1f;
	static constexpr int mNumSleepers = 100;
	const glm::vec3 mRailsScale{ 0.075f, 0.05f, 0.03f };
	const glm::vec3 mSleepersScale{ 0.5f, 0.05f, 0.08f };
	const glm::vec3 mRailColor{ 0.1f, 0.1f, 0.1f };
	const glm::vec3 mSleeperColor{ 0.4f, 0.4f, 0.4f };


public:
	Rails(Engine* engine, Mesh* mesh, SplinePath* path);


private:
	void BuildRails(Engine* engine, Mesh* mesh, SplinePath* path);
	void BuildSleepers(Engine* engine, Mesh* mesh, SplinePath* path);
};