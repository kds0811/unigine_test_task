#include "SplinePath.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>
#include <cassert>


SplinePath::SplinePath(const std::vector<glm::vec3>& pathPoints)
{
	BuildAllSplinePathPoints(pathPoints);
}

glm::vec3 SplinePath::GetSplinePoint(size_t index) const
{
	assert(index < mSplinePathPoints.size());
	return mSplinePathPoints[index];
}

glm::vec3 SplinePath::GetRandomPointOnSpline()
{
	assert(mSplinePathPoints.size() > 0);
	std::uniform_int_distribution<size_t> randIndex{0, mSplinePathPoints.size() - 1};
	return mSplinePathPoints[randIndex(mPrng)];
}

std::array<glm::vec3, NumPointsInSegment> SplinePath::GetSplinePoints(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, float step)
{
	std::array<glm::vec3, NumPointsInSegment> result;
	float curStep = 0.0f;

	for (size_t i = 0; i < NumPointsInSegment; ++i)
	{
		result[i] = glm::catmullRom(v1, v2, v3, v4, curStep);
		curStep += step;
	}
	return result;
}

void  SplinePath::BuildAllSplinePathPoints(const std::vector<glm::vec3>& pathPoints)
{
	assert(pathPoints.size() >= 4);
	size_t numPoints = pathPoints.size() * NumPointsInSegment;
	mSplinePathPoints.reserve(numPoints + 1);

	for (size_t i = 0; i < pathPoints.size(); ++i)
	{
		size_t index0 = i;
		size_t index1 = (i + 1) % pathPoints.size();
		size_t index2 = (i + 2) % pathPoints.size();
		size_t index3 = (i + 3) % pathPoints.size();

		const auto splinePoints = GetSplinePoints(pathPoints[index0], pathPoints[index1], pathPoints[index2], pathPoints[index3], splinePrecision);
		mSplinePathPoints.insert(mSplinePathPoints.end(), splinePoints.begin(), splinePoints.end());
	}
}
