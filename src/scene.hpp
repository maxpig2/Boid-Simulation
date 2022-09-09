
#pragma once

//std
#include <vector>
#include <string>

// glm
#include <glm/glm.hpp>

// project
#include "cgra/cgra_mesh.hpp"
#include "cgra/cgra_shader.hpp"

struct Sphere {
	glm::vec3 centre;
	float radius = 1;
};



// boid class (forward declaration)
class Boid;

class Scene {
private:
	// opengl draw data
	GLuint m_color_shader = 0;
	GLuint m_aabb_shader = 0;
	GLuint m_skymap_shader = 0;
	cgra::gl_mesh m_simple_boid_mesh;
	cgra::gl_mesh m_boid_mesh;
	cgra::gl_mesh m_predator_mesh;
	cgra::gl_mesh m_sphere_mesh;
	//cgra::gl_mesh m_round_sphere_mesh;

	// draw status
	bool m_show_aabb = true;
	bool m_show_axis = false;
	bool m_show_skymap = false;

	// scene data
	glm::vec3 m_bound_hsize = glm::vec3(20);
	std::vector<Boid> m_boids;


	//-------------------------------------------------------------
	// [Assignment 3] :
	// Create variables for keeping track of the boid parameters
	// such as min and max speed etc. These paramters can either be
	// public, or private with getter functions.
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

public:

	Scene();

	std::string wrapMode = "force";
	// functions that load the scene
	void loadCore();
	void loadCompletion();
	void loadChallenge();

	// called every frame, with timestep in seconds
	void update(float timestep);

	// called every frame, with the given projection and view matrix
	void draw(const glm::mat4 &proj, const glm::mat4 &view);

	// called every frame (to fill out a ImGui::TreeNode)
	void renderGUI();

	// returns a const reference to the boids vector
	const std::vector<Boid> & boids() const { return m_boids; }

	// returns the half-size of the bounding box (centered around the origin)
	glm::vec3 bound() const { return m_bound_hsize; }

	// YOUR CODE GOES HERE
	// ...
	float AlignmentWeight = 1.0;
	float CohesionWeight = 1.0;
	float AvoidanceWeight = 1.0;
	float NeighbourDistance = 5.0;
	float confinementWeight = 3;

	float boidMaxAcc = 25;
	float predatorMaxAcc = 50;

	float avoidanceFromPredators = 1;
	float predatorEatDistance = 5.0;

	//glm::vec3 sphere = glm::vec3(0, 0, 0);

	static const int numberOfSpheres = 3;
	Sphere Spheres[numberOfSpheres];
	
	
	float MinimumSpeed = 1.0;
	float MaximumSpeed = 10.0;
	float predatorVelocityInfluence = 5;
	float predatorAccelerationInfluence = 50;
	float boidSize = 0.5;


	int totalBoidCount = 300;
	int totalPredatorCount = 0;
	int totalPredatorCountBuffer = 0;
	int numberOfFlocks = 2;
	float sphereAvoidanceWeight = 25;
	bool drawSpheres = false;
};