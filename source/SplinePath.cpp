#include "SplinePath.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>
#include <cassert>
#include "MathHelper.h"


SplinePath::SplinePath(const std::vector<glm::vec3>& pathPoints)
	:
	mControlPoints(pathPoints)
{
	BuildCumulativeArcLengths();
	mSplinePathPoints = GenerateUniformPoints(mTotalUniformPoints);
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

// Creating a table of lengths of all spline segments
void SplinePath::BuildCumulativeArcLengths()
{
	mCumulativeArcLengths.clear();
	mCumulativeArcLengths.push_back(0.0f); // first length

	double step = 1.0 / static_cast<double>(mArcLengthResolution);
	glm::vec3 prevPoint = GetPointAtNormalizedPosition(0.0);

	for (size_t i = 1; i <= mArcLengthResolution; ++i)
	{
		double t = static_cast<double>(i) * step;
		glm::vec3 currentPoint = GetPointAtNormalizedPosition(t);
		mTotalArcLength += glm::length(currentPoint - prevPoint); // Calculating the total length of the spline
		mCumulativeArcLengths.push_back(mTotalArcLength); // Adding the section length to the arc length table 
		prevPoint = currentPoint;
	}
}

double SplinePath::FindNormalizedPositionByArcLength(double arcLength) const
{
	// handling corner casses
	if (arcLength <= 0.0f) return 0.0f;
	if (arcLength >= mTotalArcLength) return 1.0f;

	// binary search for the closest arcLength value in the ArcLengthTable
	size_t low = 0;
	size_t high = mCumulativeArcLengths.size() - 1;
	while (low < high)
	{
		size_t mid = (low + high) / 2;
		if (mCumulativeArcLengths[mid] < arcLength)
		{
			low = mid + 1;
		}
		else
		{
			high = mid;
		}
	}

	double lowerBound = static_cast<double>(low) / mArcLengthResolution;
	double upperBound = static_cast<double>(low + 1) / mArcLengthResolution;

	double lengthLow = mCumulativeArcLengths[low]; // Arc length value at the lower boundary of the interval
	double lengthHigh = mCumulativeArcLengths[low + 1]; // // Arc length value at the upper boundary of the interval

	// Linear interpolation between lowerBound and upperBound based on the relative position of arcLength
	// inside the interval [lengthLow, lengthHigh]
	// (arcLength - lengthLow) / (lengthHigh - lengthLow) is the relative position of arcLength within the interval
	return glm::mix(lowerBound, upperBound, (arcLength - lengthLow) / (lengthHigh - lengthLow));
}

std::vector<glm::vec3> SplinePath::GenerateUniformPoints(size_t numPoints) const
{
	std::vector<glm::vec3> points; // temp vector
	points.reserve(numPoints + 1);

	double step = mTotalArcLength / static_cast<double>(numPoints);
	for (size_t i = 0; i < numPoints; ++i)
	{
		double arcLength = static_cast<double>(i) * step;
		points.push_back(GetPointAtArcLength(arcLength));
	}
	return points;
}

glm::vec3 SplinePath::GetPointAtNormalizedPosition(double normalizedPosition) const
{
	int targetControlPoint = static_cast<int>(normalizedPosition * static_cast<int>(mControlPoints.size())); // cakculate target control points

	int index0 = (targetControlPoint - 1);
	if (index0 < 0) index0 += static_cast<int>(mControlPoints.size()); // handling corner case when the index is less than 0, we wrap it back to a valid array index

	// processing of other indices, since they are all greater than 0, we loop their index into the correct array index
	int index1 =  targetControlPoint      % static_cast<int>(mControlPoints.size());
	int index2 = (targetControlPoint + 1) % static_cast<int>(mControlPoints.size());
	int index3 = (targetControlPoint + 2) % static_cast<int>(mControlPoints.size());


	// The result of multiplying t by the number of control points is divided modulo 1.0f to leave only the fractional part,
	// since this function takes values [0, 1].
	const float factor = static_cast<float>(fmod(normalizedPosition * mControlPoints.size(), 1.0f));

	return glm::catmullRom(mControlPoints[index0], mControlPoints[index1], mControlPoints[index2], mControlPoints[index3], factor); // We get a specific point on the spline.
}

glm::vec3 SplinePath::GetPointAtArcLength(double arcLength) const
{
	double normalizedPosition = FindNormalizedPositionByArcLength(arcLength);
	return GetPointAtNormalizedPosition(normalizedPosition);
}

