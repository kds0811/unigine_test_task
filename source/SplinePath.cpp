#include "SplinePath.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>
#include <cassert>
#include "MathHelper.h"


SplinePath::SplinePath(const std::vector<glm::vec3>& pathPoints)
	:
	mControlPoints(pathPoints)
{
	BuildArcLengthTable();
	mSplinePathPoints = GenerateUniformPoints(799); 
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

glm::vec3 SplinePath::GetPrevPoint(size_t currentIndex) const
{
	if (currentIndex == 0)
	{
		return GetSplinePoint(GetNumPoints() - 1);
	}
	return GetSplinePoint((currentIndex - 1) % GetNumPoints());
}


void SplinePath::BuildArcLengthTable()
{
	mArcLengthTable.clear();
	mArcLengthTable.push_back(0.0f);

	float step = 1.0f / mNumSamples;
	glm::vec3 prevPoint = GetPointAtT(0.0f);

	for (size_t i = 1; i <= mNumSamples; ++i)
	{
		float t = i * step;
		glm::vec3 currentPoint = GetPointAtT(t);
		mTotalArcLength += glm::length(currentPoint - prevPoint);
		mArcLengthTable.push_back(mTotalArcLength);
		prevPoint = currentPoint;
	}
}

float SplinePath::FindTByArcLength(float arcLength) const
{
	if (arcLength <= 0.0f) return 0.0f;
	if (arcLength >= mTotalArcLength) return 1.0f;

	size_t low = 0;
	size_t high = mArcLengthTable.size() - 1;

	while (low < high)
	{
		size_t mid = (low + high) / 2;
		if (mArcLengthTable[mid] < arcLength)
		{
			low = mid + 1;
		}
		else
		{
			high = mid;
		}
	}

	float tLow = static_cast<float>(low) / mNumSamples;
	float tHigh = static_cast<float>(low + 1) / mNumSamples;

	float lengthLow = mArcLengthTable[low];
	float lengthHigh = mArcLengthTable[low + 1];
	return glm::mix(tLow, tHigh, (arcLength - lengthLow) / (lengthHigh - lengthLow));
}

std::vector<glm::vec3> SplinePath::GenerateUniformPoints(size_t numPoints) const
{
	std::vector<glm::vec3> points;
	points.reserve(numPoints + 1);
	float step = mTotalArcLength / (numPoints - 1);

	for (size_t i = 0; i < numPoints; ++i)
	{
		float arcLength = i * step;
		points.push_back(GetPointAtArcLength(arcLength));
	}

	return points;
}

glm::vec3 SplinePath::GetPointAtT(float t) const
{
	size_t index0 = ((size_t)((t * mControlPoints.size()) - 1)) % mControlPoints.size();
	size_t index1 = (size_t)(t * mControlPoints.size()) % mControlPoints.size();
	size_t index2 = (size_t)((t * mControlPoints.size()) + 1) % mControlPoints.size();
	size_t index3 = (size_t)((t * mControlPoints.size()) + 2) % mControlPoints.size();

	return glm::catmullRom(
		mControlPoints[index0],
		mControlPoints[index1],
		mControlPoints[index2],
		mControlPoints[index3],
		fmod(t * mControlPoints.size(), 1.0f));
}

glm::vec3 SplinePath::GetPointAtArcLength(float arcLength) const
{
	float t = FindTByArcLength(arcLength);
	return GetPointAtT(t);
}

