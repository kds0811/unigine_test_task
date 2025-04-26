#include "framework/engine.h"
#include "framework/utils.h"
#include "GameTimer.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/spline.hpp>

using namespace std;
using namespace glm;

/*
* Coordinate system:
* x - right
* y - up
* z - backward
*/



std::vector<vec3> GetSplinePoints(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& v4, float step)
{
	std::vector<vec3> result;
	size_t numPoints = static_cast<size_t>(1 / step);
	result.reserve(numPoints + 1);

	float curStep = 0.0f;

	for (size_t i = 0; i < numPoints; i++)
	{
		result.push_back(glm::catmullRom(v1, v2, v3, v4, curStep));
		curStep += step;
	}
	return result;
}

std::vector<vec3> GetAllPathPoints(const std::vector<vec3>& pathPoints)
{
	assert(pathPoints.size() >= 4);

	static constexpr float splinePrecision = 0.05f;
	std::vector<vec3> result;
	size_t numPoints = pathPoints.size() * static_cast<size_t>(1 / splinePrecision);
	result.reserve(numPoints + 1);

	for (size_t i = 0; i < pathPoints.size(); ++i)
	{
		size_t index0 = i;
		size_t index1 = (i + 1) % pathPoints.size();
		size_t index2 = (i + 2) % pathPoints.size();
		size_t index3 = (i + 3) % pathPoints.size();

		const auto splinePoints = GetSplinePoints(pathPoints[index0], pathPoints[index1], pathPoints[index2], pathPoints[index3], splinePrecision);
		result.insert(result.end(), splinePoints.begin(), splinePoints.end());
	}

	return result;
}




int main()
{
	// initialization
	Engine* engine = Engine::get();
	engine->init(1600, 900, "UNIGINE Test Task");

	// set up camera
	Camera& cam = engine->getCamera();
	cam.Position = vec3(0.0f, 12.0f, 17.0f);
	cam.Yaw = -90.0f;
	cam.Pitch = -45.0f;
	cam.UpdateCameraVectors();

	// create shared meshes
	Mesh plane_mesh = createPlane();
	Mesh sphere_mesh = createSphere();

	// create background objects
	Object* plane = engine->createObject(&plane_mesh);
	plane->setColor(0.2f, 0.37f, 0.2f); // green
	plane->setPosition(0, -0.5f, 0);
	plane->setRotation(-90.0f, 0.0f, 0.0f);
	plane->setScale(20.0f);

	// path
	const float path[] = {
		 0.0f, -0.375f,  7.0f, // 1
		-6.0f, -0.375f,  5.0f, // 2
		-8.0f, -0.375f,  1.0f, // 3
		-4.0f, -0.375f, -6.0f, // 4
		 0.0f, -0.375f, -7.0f, // 5
		 1.0f, -0.375f, -4.0f, // 6
		 4.0f, -0.375f, -3.0f, // 7
		 8.0f, -0.375f,  7.0f  // 8
	};
	vector<Object*> points;
	for (int i = 0; i < 8; i++)
	{
		Object* sphere = engine->createObject(&sphere_mesh);
		sphere->setColor(1, 0, 0);
		sphere->setPosition(path[i * 3], path[i * 3 + 1], path[i * 3 + 2]);
		sphere->setScale(0.25f);
		points.push_back(sphere);
	}
	LineDrawer path_drawer(path, points.size(), true);

	Mesh cubeMesh = createCube();
	Object* cube = engine->createObject(&cubeMesh);
	cube->setColor(0.7f, 0.7f, 0.7f);
	vec3 CubePosition = { 0.0f, 0.0f, 0.0f };
	cube->setPosition(0.0f, 0.0f, 0.0f);
	cube->setScale(0.5f);

	std::vector<vec3> positions{};

	positions.push_back(glm::vec3{ 0.0f, -0.375f,  7.0f });
	positions.push_back(glm::vec3{ -6.0f, -0.375f,  5.0f });
	positions.push_back(glm::vec3{ -8.0f, -0.375f,  1.0f });
	positions.push_back(glm::vec3{ -4.0f, -0.375f, -6.0f });
	positions.push_back(glm::vec3{ 0.0f, -0.375f, -7.0f });
	positions.push_back(glm::vec3{ 1.0f, -0.375f, -4.0f });
	positions.push_back(glm::vec3{ 4.0f, -0.375f, -3.0f });
	positions.push_back(glm::vec3{ 8.0f, -0.375f,  7.0f });

	int index = 0;
	float time = 0.0f;
	int point = 1;
	float s = 0.0f;
	float rotDeg = 0.0f;
	GameTimer timer;
	timer.Reset();








	const auto splinePathPoints = GetAllPathPoints(positions);

	LineDrawer splinePathDrawer(splinePathPoints, true);





	// main loop
	while (!engine->isDone())
	{
		timer.Tick();
		time += timer.GetDeltaTime();
		//if (time > 1.0f)
		//{
		//	if (index < 8)
		//	{
		//		vec3 direction = normalize(positions[index] - CubePosition);
		//		float yawRad = glm::atan(-direction.z, direction.x);
		//		glm::quat qt(glm::vec3(glm::radians(0.0f), yawRad, glm::radians(0.0f)));
		//		cube->setRotation(qt);
		//		++index;
		//	}
		//	else
		//	{
		//		index = 0;
		//	}

		//	time = 0.0f;
		//}




		engine->update();
		engine->render();

		//path_drawer.draw();
		splinePathDrawer.draw();
		engine->swap();

	}

	engine->shutdown();
	return 0;
}
