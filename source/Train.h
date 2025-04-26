#pragma once
#include <array>
#include <Carriage.h>

class vec3;
class SplinePath;
class Engine;

class Train
{
	static constexpr size_t mNumCarriages = 8;
	static constexpr size_t mCarriageDistance = 2;
	std::array<Carriage, mNumCarriages> mCarriages{};

public:
	Train(Engine* engine,  Mesh* mesh, SplinePath* path);
	~Train();

	void Update(float deltaTime);
};