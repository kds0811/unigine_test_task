#pragma once
#include <array>
#include <Wagon.h>

class vec3;
class SplinePath;
class Engine;

class Train
{
	static constexpr int mNumWagons = 5;
	static constexpr int mWagonsDistance = 15;
	std::array<Wagon, mNumWagons> mWagons{};

public:
	Train(Engine* engine,  Mesh* mesh, SplinePath* path);
	~Train();

	void Update(float deltaTime);
};