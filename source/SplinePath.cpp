#include "SplinePath.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>
#include <cassert>


SplinePath::SplinePath(const std::vector<glm::vec3>& pathPoints)
{
	BuildAllSplinePathPoints(pathPoints);
	ResampleSplinePoints(splinePrecision);
}

glm::vec3 SplinePath::GetSplinePoint(size_t index) const
{
	assert(index < mSplinePathPoints.size());
	return mSplinePathPoints[index];
}

glm::vec3 SplinePath::GetNextPoint(size_t currentIndex) const
{
	return GetSplinePoint((currentIndex + 1) % GetNumPoints());
}

glm::vec3 SplinePath::GetRandomPointOnSpline() 
{
	assert(mSplinePathPoints.size() > 0);
	std::uniform_int_distribution<size_t> randIndex{0, mSplinePathPoints.size() - 1};
	return mSplinePathPoints[randIndex(mPrng)];
}

std::array<glm::vec3, NumPointsInSegment> SplinePath::GetSplinePoints(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, float step)
{
	std::array<glm::vec3, NumPointsInSegment> result{};
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

std::vector<float> SplinePath::ComputeArcLengths(const std::vector<glm::vec3>& splinePoints)
{
	std::vector<float> arcLengths;
	arcLengths.reserve(GetNumPoints() + 1);
	arcLengths.push_back(0.0f);
	for (size_t i = 1; i < splinePoints.size(); ++i)
	{
		float segmentLength = glm::distance(splinePoints[i], splinePoints[i - 1]);
		arcLengths.push_back(arcLengths.back() + segmentLength);
	}

	return arcLengths;
}

void SplinePath::ResampleSplinePoints(float desiredDistance)
{
	// Calculate arc lengths for current spline points
	std::vector<float> arcLengths = ComputeArcLengths(mSplinePathPoints);
	float totalLength = arcLengths.back();

	// Generate new points with uniform spacing
	std::vector<glm::vec3> resampledPoints;
	size_t numPoints = static_cast<size_t>(totalLength / desiredDistance) + 1;
	resampledPoints.reserve(numPoints);

	for (size_t i = 0; i < numPoints; ++i)
	{
		float currentArcLength = i * desiredDistance;
		resampledPoints.push_back(GetPositionAtArcLength(mSplinePathPoints, arcLengths, currentArcLength));
	}

	// Replacing old points with new ones
	mSplinePathPoints = std::move(resampledPoints);
}

glm::vec3 SplinePath::GetPositionAtArcLength(const std::vector<glm::vec3>& splinePoints, const std::vector<float>& arcLengths, float currentArcLength)
{
	// Binary search to find the index corresponding to s
	size_t low = 0, high = arcLengths.size() - 1;
	while (low < high)
	{
		size_t mid = (low + high) / 2;
		if (arcLengths[mid] < currentArcLength)
		{
			low = mid + 1;
		}
		else
		{
			high = mid;
		}	
	}

	// Linear interpolation between two neighboring points
	size_t idx = low;
	if (idx == 0)
	{
		return splinePoints[0];
	}
	if (idx >= splinePoints.size())
	{
		return splinePoints.back();
	}

	float interpolationFactor = (currentArcLength - arcLengths[idx - 1]) / (arcLengths[idx] - arcLengths[idx - 1]);
	return glm::mix(splinePoints[idx - 1], splinePoints[idx], interpolationFactor);
}
