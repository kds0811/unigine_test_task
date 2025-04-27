#include "Rails.h"
#include <framework/object.h>
#include <framework/engine.h>
#include <framework/mesh.h>
#include "SplinePath.h"
#include <cassert>
#include "MathHelper.h"

Rails::Rails(Engine* engine, Mesh* mesh, SplinePath* path)
{
	assert(engine && mesh && path);
	if (!engine || !mesh || !path) return; // Logging or throwing an exception or handling an error

	BuildRails(engine, mesh, path);
	BuildSleepers(engine, mesh, path);
}

void Rails::BuildRails(Engine* engine, Mesh* mesh, SplinePath* path)
{
	for (size_t i = 0; i < path->GetNumPoints(); ++i)
	{
		// Spawn right rail
		Object* pRailR = engine->createObject(mesh);
		assert(pRailR);

		pRailR->setColor(mRailColor);
		pRailR->setScale(mRailsScale);
		const auto direction = Math::CalculateDirectionVecToDest(path->GetNextPoint(i), path->GetSplinePoint(i));
		pRailR->setRotation(Math::CalculateDirectionQuat(direction));

		glm::vec3 rightVector = glm::cross(direction, glm::vec3{ 0.0f, 1.0f, 0.0f });
		glm::vec3 offsetR = rightVector * mRailsWidth;
		glm::vec3 positionR = { path->GetSplinePoint(i).x + offsetR.x, -0.5f, path->GetSplinePoint(i).z + offsetR.z };
		pRailR->setPosition(positionR);

		// Spawn left rail
		Object* pRailL = engine->createObject(mesh);
		assert(pRailL);
		pRailL->setColor(mRailColor);
		pRailL->setScale(mRailsScale);
		pRailL->setRotation(Math::CalculateDirectionQuat(direction));
		glm::vec3 leftVector = rightVector * -1.0f;
		glm::vec3 offsetL = leftVector * mRailsWidth;
		glm::vec3 positionL = { path->GetSplinePoint(i).x + offsetL.x, -0.5f, path->GetSplinePoint(i).z + offsetL.z };
		pRailL->setPosition(positionL);
	}
}

void Rails::BuildSleepers(Engine* engine, Mesh* mesh, SplinePath* path)
{
	int sleepersWidth = path->GetNumPoints() / mNumSleepers;

	for (size_t i = 0; i < path->GetNumPoints(); i += sleepersWidth)
	{
		Object* pSleeper = engine->createObject(mesh);
		assert(pSleeper);

		pSleeper->setColor(mSleeperColor);
		pSleeper->setScale(mSleepersScale);

		glm::vec3 direction = Math::CalculateDirectionVecToDest(path->GetNextPoint(i), path->GetSplinePoint(i));
		float directionYaw = Math::CalculateYawFromVector(direction);
		float rotatedYaw = directionYaw + glm::radians(90.0f);
		glm::quat rotatedQuat = glm::quat(glm::vec3{ 0.0f, rotatedYaw, 0.0f });
		pSleeper->setRotation(rotatedQuat);
		glm::vec3 position{ path->GetSplinePoint(i).x, -0.52f, path->GetSplinePoint(i).z };
		pSleeper->setPosition(position);
	}

}
