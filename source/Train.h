#pragma once
#include <array>
#include <Wagon.h>

class vec3;
class SplinePath;
class Engine;

class Train
{
	static constexpr size_t mNumWagons = 8;
	static constexpr size_t mWagonsDistance = 10;
	std::array<Wagon, mNumWagons> mWagons{};

public:
	Train(Engine* engine,  Mesh* mesh, SplinePath* path);
	~Train();

	void Update(float deltaTime);
};