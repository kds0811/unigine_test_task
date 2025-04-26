#pragma once
#include <memory>

class CarriageMovementComponent;

class Carriage
{
	std::unique_ptr<CarriageMovementComponent> mMovementComponent = nullptr;


public:
	Carriage();
	~Carriage();
};