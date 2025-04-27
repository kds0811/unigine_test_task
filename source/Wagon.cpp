#include "Wagon.h"
#include <cassert>
#include <framework/object.h>
#include <framework/engine.h>
#include <framework/mesh.h>
#include "SplinePath.h"
#include "MathHelper.h"

Wagon::Wagon()
{}

Wagon::~Wagon() = default;

void Wagon::Init(Engine* engine, Mesh* mesh, const glm::vec3& startPos, int destinationIndex, const glm::vec3& destinationPosition, SplinePath* path)
{
	// we do not check incoming pointers because they have already been checked in the previous function

	mDestinationIndex = destinationIndex;
	mDestinationPosition = destinationPosition;
	pSplinePath = path;
	pObject = engine->createObject(mesh);

	assert(pObject);
	if (pObject)
	{
		pObject->setColor(0.7f, 0.7f, 0.7f);
		pObject->setScale(0.4f);
		pObject->setPosition(startPos);
		mForwardVector = Math::CalculateDirectionVecToDest(mDestinationPosition, pObject->getPosition());
		pObject->setRotation(Math::CalculateDirectionQuat(mForwardVector));
	}
}

void Wagon::Update(float deltaTime)
{
	UpdatePosition(deltaTime);
	UpdateRotation(deltaTime);
}

void Wagon::UpdatePosition(float deltaTime)
{
	glm::vec3 vecToDestination = Math::CalculateDirectionVecToDest(mDestinationPosition, pObject->getPosition());

	bool bDestinationIsForward = IsDestinationAhead(vecToDestination); // check if the destination is ahead

	// Since we correct the rotation of the Wagon towards the destination point, when the scalar product will be less than zero,
	// it means that we have passed the point and it is necessary to assign the next one.
	if (bDestinationIsForward)
	{
		MoveTowardsDestination(vecToDestination, deltaTime); // if the destination is ahead, we just move to it.
	}
	else
	{
		// We go through the points in front of the Wagon to find the one in front,
		// in case the speed is so high that we pass the nearest point.
		int indexOffset = 0;
		while (!bDestinationIsForward) // Loop until we find the point ahead. A bug may occur here if the forward vector is incorrect.
		{
			mDestinationIndex = mDestinationIndex + indexOffset;
			mDestinationPosition = pSplinePath->GetNextPoint(mDestinationIndex);
			vecToDestination = Math::CalculateDirectionVecToDest(mDestinationPosition, pObject->getPosition());
			bDestinationIsForward = IsDestinationAhead(vecToDestination);
			++indexOffset;
		}
		MoveTowardsDestination(vecToDestination, deltaTime);
		StartRotation(); // When a new destination point is obtained, start rotation
	}
}

void Wagon::UpdateRotation(float deltaTime)
{
	if (!mIsRotating) return;

	mCurrentRotationProgress += mRotationSpeed * deltaTime;

	if (mCurrentRotationProgress >= 1.0f) // If the value of mCurrentRotationProgress is greater than 1.0, normalize it to 1.0 for Slerp to work correctly
	{
		mCurrentRotationProgress = 1.0f;
		mIsRotating = false; // complete the rotation
	}

	// rotate by Yaw angle only
	glm::quat startQuat = glm::quat(glm::vec3{ 0.0f, mStartYaw, 0.0f });
	glm::quat destQuat = glm::quat(glm::vec3{ 0.0f, mDestYaw, 0.0f });
	glm::quat currentQuat = glm::slerp(startQuat, destQuat, mCurrentRotationProgress);

	pObject->setRotation(currentQuat);
}

bool Wagon::IsDestinationAhead(const glm::vec3& vecToDest) const
{
	return glm::dot(vecToDest, mForwardVector) > 0;
}

void Wagon::MoveTowardsDestination(const glm::vec3& vecToDest, float deltaTime)
{
	glm::vec3 offset = vecToDest * deltaTime * mSpeed;
	pObject->setPosition(pObject->getPosition() + offset);
	mForwardVector = vecToDest;
}

void Wagon::StartRotation()
{
	mStartYaw = Math::CalculateYawFromVector(mForwardVector); // calculating the current yaw angle 
	mDestYaw = Math::CalculateYawFromVector(Math::CalculateDirectionVecToDest(mDestinationPosition, pObject->getPosition())); // target angle yaw calculation

	float deltaYaw = mDestYaw - mStartYaw;
	if (std::abs(deltaYaw) < 0.0000001) return; // If the difference in rotation is not significant, we don't start it.

	mCurrentRotationProgress = 0.0f;
	mIsRotating = true;
}
