#include "Train.h"
#include <glm/vec3.hpp>
#include <SplinePath.h>
#include <cassert>
#include <framework/engine.h>

Train::Train(Engine* engine, Mesh* mesh, SplinePath* path)
{
	assert(engine && mesh && path);
	if (!engine || !mesh || !path) return; //can throw  exception or log  error

	for (int i = 0; i < mNumWagons; i++)
	{
		const auto position = path->GetSplinePoint(i * mWagonsDistance); // getting a point on the spline to position the wagon
		const int destinationIndex = i * mWagonsDistance + 1; // getting the index of the next point to move
		const auto destPosition = path->GetSplinePoint(destinationIndex); // getting the next waypoint
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
