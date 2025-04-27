#include "framework/engine.h"
#include "framework/utils.h"
#include "SplinePath.h"
#include "Train.h"
#include "Rails.h"

using namespace std;
using namespace glm;

/*
* Coordinate system:
* x - right
* y - up
* z - backward
*/


int main()
{
	// initialization
	Engine* engine = Engine::get();
	engine->init(1600, 900, "UNIGINE Test Task");

	// set up camera
	Camera& cam = engine->getCamera();
	cam.Position = glm::vec3(0.0f, 12.0f, 17.0f);
	cam.Yaw = -90.0f;
	cam.Pitch = -45.0f;
	cam.UpdateCameraVectors();

	// create shared meshes
	Mesh plane_mesh = createPlane();
	Mesh sphere_mesh = createSphere();
	Mesh cubeMesh = createCube();

	// create background objects
	Object* plane = engine->createObject(&plane_mesh);
	plane->setColor(0.2f, 0.37f, 0.2f); // green
	plane->setPosition(0, -0.5f, 0);
	plane->setRotation(-90.0f, 0.0f, 0.0f);
	plane->setScale(20.0f);

	// create control points
	std::vector<glm::vec3> ControlPoints{};
	ControlPoints.reserve(8);
	ControlPoints.push_back(glm::vec3{ 0.0f, -0.375f,  7.0f });
	ControlPoints.push_back(glm::vec3{ -6.0f, -0.375f,  5.0f });
	ControlPoints.push_back(glm::vec3{ -8.0f, -0.375f,  1.0f });
	ControlPoints.push_back(glm::vec3{ -4.0f, -0.375f, -6.0f });
	ControlPoints.push_back(glm::vec3{ 0.0f, -0.375f, -7.0f });
	ControlPoints.push_back(glm::vec3{ 1.0f, -0.375f, -4.0f });
	ControlPoints.push_back(glm::vec3{ 4.0f, -0.375f, -3.0f });
	ControlPoints.push_back(glm::vec3{ 8.0f, -0.375f,  7.0f });

	// create spline
	SplinePath splinePath(ControlPoints);

	// create rails and sleepers
	Rails rails(engine, &cubeMesh, &splinePath);

	// create train and wagons
	Train train{engine, &cubeMesh, &splinePath };

	// main loop
	while (!engine->isDone())
	{
		engine->update();
		train.Update(engine->getDeltaTime()); // update moving and rotating train
		engine->render();
		engine->swap();
	}

	engine->shutdown();
	return 0;
}
