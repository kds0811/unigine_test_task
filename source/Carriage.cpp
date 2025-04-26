#include "Carriage.h"
#include <cassert>
#include <framework/object.h>
#include <framework/engine.h>
#include <framework/mesh.h>
#include "SplinePath.h"

Carriage::Carriage()
{}

Carriage::~Carriage() = default;

void Carriage::Init(Engine* engine, Mesh* mesh, const glm::vec3& startPos, int destinationIndex, const glm::vec3& destinationPosition, SplinePath* path)
{
	mDestinationIndex = destinationIndex;
	mDestinationPosition = destinationPosition;
	pSplinePath = path;
	mCurrentDirection = glm::normalize(mDestinationPosition - startPos);


	pObject = engine->createObject(mesh);
	pObject->setColor(0.7f, 0.7f, 0.7f);
	pObject->setScale(0.4f);
	pObject->setPosition(startPos);


	pObject->setRotation(CalculateDirectionQuat(mCurrentDirection));
}

void Carriage::Update(float deltaTime)
{
	glm::vec3 vecToDestination = glm::normalize(mDestinationPosition - pObject->getPosition());
	bool bDestinationIsForward = glm::dot(vecToDestination, mCurrentDirection) > 0;
	if (bDestinationIsForward)
	{
		glm::vec3 offset = vecToDestination * deltaTime * mSpeed;
		pObject->setPosition(pObject->getPosition() + offset);
	}
	else
	{
		mDestinationIndex = (mDestinationIndex + 1) % pSplinePath->GetNumPoints();
		mDestinationPosition = pSplinePath->GetSplinePoint(mDestinationIndex);
		mCurrentDirection = glm::normalize(mDestinationPosition - pObject->getPosition());

		glm::vec3 offset = vecToDestination * deltaTime * mSpeed;
		pObject->setPosition(pObject->getPosition() + offset);
		pObject->setRotation(CalculateDirectionQuat(mCurrentDirection));
	}

}

glm::quat Carriage::CalculateDirectionQuat(const glm::vec3& direction)
{
	float yawRad = glm::atan(-direction.z, direction.x);
	return glm::quat(glm::vec3{ 0.0f, yawRad, 0.0f });
}

