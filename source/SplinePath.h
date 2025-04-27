#pragma once
#include <vector>
#include <array>
#include <glm/vec3.hpp>

class SplinePath
{
    std::vector<glm::vec3> mControlPoints{};
    std::vector<glm::vec3> mSplinePathPoints{};
    std::vector<double> mArcLengthTable;
    double mAveragePointDistance = 0.0f;
    double mTotalArcLength = 0.0f;
    static constexpr int mNumSamples = 1000;
    static constexpr int mSplineTotalPoints = 800;

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
    glm::vec3 GetPointAtT(double t) const;
    glm::vec3 GetPointAtArcLength(double arcLength) const;

};
