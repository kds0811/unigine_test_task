#include "Carriage.h"
#include "CarriageMovementComponent.h"

Carriage::Carriage()
{
	mMovementComponent = std::make_unique<CarriageMovementComponent>();
}

Carriage::~Carriage() = default;
