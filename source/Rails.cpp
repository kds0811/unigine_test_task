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
		const auto splinePoint = path->GetSplinePoint(i);
		const auto direction = Math::CalculateDirectionVecToDest(path->GetNextPoint(i), splinePoint);
		glm::vec3 rightVector{};

		// Spawn right rail
		Object* pRailRight = engine->createObject(mesh);
		assert(pRailRight);
		if (pRailRight)
		{
			pRailRight->setColor(mRailColor);
			pRailRight->setScale(mRailsScale);
			pRailRight->setRotation(Math::CalculateDirectionQuat(direction));
			rightVector = glm::cross(direction, glm::vec3{ 0.0f, 1.0f, 0.0f }); // find the right vector through cross product forward vector and up vector
			glm::vec3 offsetRightVec = rightVector * mRailsWidth;
			glm::vec3 positionRight = { splinePoint.x + offsetRightVec.x, mRailsY, splinePoint.z + offsetRightVec.z }; // new position = old position + offset by X and Z plane coordinates. Y height is fixed.
			pRailRight->setPosition(positionRight);
		}

		// Spawn left rail
		Object* pRailLeft = engine->createObject(mesh);
		assert(pRailLeft);
		if (pRailLeft)
		{
			pRailLeft->setColor(mRailColor);
			pRailLeft->setScale(mRailsScale);
			pRailLeft->setRotation(Math::CalculateDirectionQuat(direction));
			glm::vec3 leftVector = rightVector * -1.0f;  // calculate the left vector, which is opposite to the right vector
			glm::vec3 offsetLeftVec = leftVector * mRailsWidth;
			glm::vec3 positionLeft = { splinePoint.x + offsetLeftVec.x, mRailsY, splinePoint.z + offsetLeftVec.z }; // new position = old position + offset by X and Z plane coordinates. Y height is fixed.
			pRailLeft->setPosition(positionLeft);
		}
	}
}

void Rails::BuildSleepers(Engine* engine, Mesh* mesh, SplinePath* path)
{
	int sleepersWidth = static_cast<int>(path->GetNumPoints()) / mNumSleepers; // the distance can be counted in the number of control points, since we have a fixed distance between the control  points

	for (size_t i = 0; i < path->GetNumPoints(); i += sleepersWidth) // each iteration of the loop we skip a number of points equal to sleepersWidth
	{
		Object* pSleeper = engine->createObject(mesh);
		assert(pSleeper);
		pSleeper->setColor(mSleeperColor);
		pSleeper->setScale(mSleepersScale);

		glm::vec3 direction = Math::CalculateDirectionVecToDest(path->GetNextPoint(i), path->GetSplinePoint(i)); // calculate forward vector
		float directionYaw = Math::CalculateYawFromVector(direction); // calculates the yaw of the forward vector in radians
		float rotatedYaw = directionYaw + glm::radians(90.0f); // rotate yaw 90 degrees
		glm::quat rotatedQuat = glm::quat(glm::vec3{ 0.0f, rotatedYaw, 0.0f });
		pSleeper->setRotation(rotatedQuat); 

		const auto pos = path->GetSplinePoint(i);
		glm::vec3 position{ pos.x, mSleepersY, pos.z }; // change only the height - Y coordinate
		pSleeper->setPosition(position);
	}
}
