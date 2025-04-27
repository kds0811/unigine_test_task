#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>

namespace Math
{
	//calculates the yaw angle in radians
	static inline float CalculateYawFromVector(const glm::vec3& vec)
	{
		float yaw = glm::atan(-vec.z, vec.x); // -z because the base z-axis points backward;

		// Angle normalization in the range 0 - 2PI
		yaw = std::fmod(yaw, 2.0f * glm::pi<float>());
		if (yaw < 0.0f)
		{
			yaw += 2.0f * glm::pi<float>();
		}
		return yaw;
	}

	// Quaternion is created only with yaw angle because the rotation is in the X-Z plane around the Y axis.
	static inline glm::quat CalculateDirectionQuat(const glm::vec3& direction)
	{
		return glm::quat(glm::vec3{ 0.0f, CalculateYawFromVector(direction), 0.0f });
	}

	// Calculates the normalized direction vector
	static inline glm::vec3 CalculateDirectionVecToDest(const glm::vec3& destination, const glm::vec3& position)
	{
		return glm::normalize(destination - position);
	}
}

