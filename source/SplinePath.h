#pragma once
#include <vector>
#include <array>
#include <glm/vec3.hpp>

class SplinePath
{
    std::vector<glm::vec3> mControlPoints{};
    std::vector<glm::vec3> mSplinePathPoints{};
    double mAveragePointDistance = 0.0f;
    std::vector<float> mArcLengthTable; 
    float mTotalArcLength = 0.0f;
    int mNumSamples = 1000;

public:
    SplinePath(const std::vector<glm::vec3>& pathPoints);
    const std::vector<glm::vec3>& GetSplinePoints() const noexcept { return mSplinePathPoints; }
    glm::vec3 GetSplinePoint(size_t index) const;
    glm::vec3 GetNextPoint(size_t currentIndex) const;
    glm::vec3 GetPrevPoint(size_t currentIndex) const;
    size_t GetNumPoints() const { return mSplinePathPoints.size(); }

private:
    void BuildArcLengthTable();
    float FindTByArcLength(float arcLength) const;
    std::vector<glm::vec3> GenerateUniformPoints(size_t numPoints) const;
    glm::vec3 GetPointAtT(float t) const;
    glm::vec3 GetPointAtArcLength(float arcLength) const;

};
