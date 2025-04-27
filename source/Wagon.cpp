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
	UpdateMovement(deltaTime);
	UpdateRotation(deltaTime);
}

void Wagon::UpdateMovement(float deltaTime)
{
	glm::vec3 vecToDestination = Math::CalculateDirectionVecToDest(mDestinationPosition, pObject->getPosition());
	bool bDestinationIsForward = DestinationIsFront(vecToDestination);

	// Since we correct the rotation of the Wagon towards the destination point, when the scalar product will be less than zero,
	// it means that we have passed the point and it is necessary to assign the next one.
	if (bDestinationIsForward)
	{
		CalculateOffsetAndSetPosition(vecToDestination, deltaTime);
	}
	else
	{
		// We go through the points in front of the Wagon to find the one in front,
		// in case the speed is so high that we pass the nearest point.
		int indexOffset = 0;
		while (!bDestinationIsForward)
		{
			mDestinationIndex = mDestinationIndex + indexOffset;
			mDestinationPosition = pSplinePath->GetNextPoint(mDestinationIndex);
			vecToDestination = Math::CalculateDirectionVecToDest(mDestinationPosition, pObject->getPosition());
			bDestinationIsForward = DestinationIsFront(vecToDestination);
			++indexOffset;
		}
		CalculateOffsetAndSetPosition(vecToDestination, deltaTime);
		StartRotation();
	}
}

void Wagon::UpdateRotation(float deltaTime)
{
	if (!mIsRotating) return;

	mCurrentProgress += mRotationSpeed * deltaTime;

	if (mCurrentProgress >= 1.0f)
	{
		mCurrentProgress = 1.0f;
		mIsRotating = false; 
	}

	glm::quat startQuat = glm::quat(glm::vec3{ 0.0f, mStartYaw, 0.0f });
	glm::quat destQuat = glm::quat(glm::vec3{ 0.0f, mDestYaw, 0.0f });
	glm::quat currentQuat = glm::slerp(startQuat, destQuat, mCurrentProgress);

	pObject->setRotation(currentQuat);
}


bool Wagon::DestinationIsFront(const glm::vec3& vecToDest) const
{
	return glm::dot(vecToDest, mForwardVector) > 0;
}

void Wagon::CalculateOffsetAndSetPosition(const glm::vec3& vecToDest, float deltaTime)
{
	glm::vec3 offset = vecToDest * deltaTime * mSpeed;
	pObject->setPosition(pObject->getPosition() + offset);
	mForwardVector = vecToDest;
}


void Wagon::StartRotation()
{
	mStartYaw = Math::CalculateYawFromVector(mForwardVector);;
	mDestYaw = Math::CalculateYawFromVector(Math::CalculateDirectionVecToDest(mDestinationPosition, pObject->getPosition()));

	float deltaYaw = mDestYaw - mStartYaw;
	if (std::abs(deltaYaw) < 0.0000001) return;

	if (deltaYaw > glm::pi<float>())
	{
		deltaYaw -= 2.0f * glm::pi<float>();
	}
	else if (deltaYaw < -glm::pi<float>())
	{
		deltaYaw += 2.0f * glm::pi<float>();
	}
	mDestYaw = mStartYaw + deltaYaw;

	mCurrentProgress = 0.0f;
	mIsRotating = true;
}


