#include "Train.h"
#include <glm/vec3.hpp>
#include <SplinePath.h>
#include <cassert>
#include <framework/engine.h>

Train::Train(Engine* engine, Mesh* mesh, SplinePath* path)
{
	assert(engine && mesh&& path);
	if (!engine || !mesh || !path) return; //can throw  exception or log  error depending on the coding standard in the project

	for (size_t i = 0; i < mNumWagons; i++)
	{
		const auto position = path->GetSplinePoint(i * mWagonsDistance);
		const size_t destinationIndex = i * mWagonsDistance + 1;
		const auto destPosition = path->GetSplinePoint(destinationIndex);
		mWagons[i].Init(engine, mesh, position, destinationIndex, destPosition, path);
	}
}

Train::~Train() = default;

void Train::Update(float deltaTime)
{
	for (auto& c : mWagons)
	{
		c.Update(deltaTime);
	}
}
