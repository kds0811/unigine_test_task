#include "Wagon.h"
#include <cassert>
#include <framework/object.h>
#include <framework/engine.h>
#include <framework/mesh.h>
#include "SplinePath.h"

Wagon::Wagon()
{}

Wagon::~Wagon() = default;

void Wagon::Init(Engine* engine, Mesh* mesh, const glm::vec3& startPos, int destinationIndex, const glm::vec3& destinationPosition, SplinePath* path)
{
	mDestinationIndex = destinationIndex;
	mDestinationPosition = destinationPosition;
	pSplinePath = path;

	pObject = engine->createObject(mesh);
	pObject->setColor(0.7f, 0.7f, 0.7f);
	pObject->setScale(0.4f);
	pObject->setPosition(startPos);
	mForwardVector = glm::normalize(mDestinationPosition - pObject->getPosition());
	pObject->setRotation(CalculateDirectionQuat(mForwardVector));

}

void Wagon::Update(float deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateRotation(deltaTime);
}

void Wagon::UpdateMovement(float deltaTime)
{
	glm::vec3 vecToDestination = CalculateDirectionVecToDest();
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
			vecToDestination = CalculateDirectionVecToDest();
			bDestinationIsForward = DestinationIsFront(vecToDestination);
			++indexOffset;
		}
		CalculateOffsetAndSetPosition(vecToDestination, deltaTime);
	}
}

void Wagon::UpdateRotation(float deltaTime)
{
	glm::vec3 vecToDestination = CalculateDirectionVecToDest();


	//pObject->setRotation(CalculateDirectionQuat(mDestinationDirection));
}

glm::quat Wagon::CalculateDirectionQuat(const glm::vec3& direction)
{
	float yawRad = glm::atan(-direction.z, direction.x);
	return glm::quat(glm::vec3{ 0.0f, yawRad, 0.0f });
}

bool Wagon::DestinationIsFront(const glm::vec3& vecToDest) const
{
	return glm::dot(vecToDest, mForwardVector) > 0;
}

void Wagon::CalculateOffsetAndSetPosition(const glm::vec3& vecToDest, float deltaTime)
{
	glm::vec3 offset = vecToDest * deltaTime * mSpeed;
	pObject->setPosition(pObject->getPosition() + offset);
}

glm::vec3 Wagon::CalculateDirectionVecToDest() const
{
	return glm::normalize(mDestinationPosition - pObject->getPosition());
}

