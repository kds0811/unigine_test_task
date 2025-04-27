#pragma once
#include <array>
#include <Wagon.h>

class vec3;
class SplinePath;
class Engine;

// a class for creating and storing a certain number of wagons
class Train
{
	static constexpr int mNumWagons = 5;
	static constexpr int mWagonsDistance = 10; // distance between wagons, measured in number of control points, as the distance between points is the same
	std::array<Wagon, mNumWagons> mWagons{}; // array because the number of wagons doesn't change.

public:
	Train(Engine* engine,  Mesh* mesh, SplinePath* path);
	~Train();

	void Update(float deltaTime);
};