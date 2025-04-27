#pragma once
#include <vector>
#include <array>
#include <glm/vec3.hpp>


static constexpr float splinePrecision = 0.01f;
static constexpr size_t NumPointsInSegment = static_cast<size_t>(1 / splinePrecision);

class SplinePath
{
	std::vector<glm::vec3> mSplinePathPoints{};
	double mAveragePointDistance = 0.0f;

public:
	SplinePath(const std::vector<glm::vec3>& pathPoints);
	const std::vector<glm::vec3>& GetSplinePoints() const noexcept { return mSplinePathPoints; }
	glm::vec3 GetSplinePoint(size_t index) const;
	glm::vec3 GetNextPoint(size_t currentIndex) const;
	glm::vec3 GetPrevPoint(size_t currentIndex) const;
	size_t GetNumPoints() const { return mSplinePathPoints.size(); }

private:
	std::array<glm::vec3, NumPointsInSegment> GetSplinePoints(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, float step);
	void BuildAllSplinePathPoints(const std::vector<glm::vec3>& pathPoints);

	void RelaxPoints(int iterations);
};
