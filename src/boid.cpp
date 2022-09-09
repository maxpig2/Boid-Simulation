
// glm
#include <glm/gtc/random.hpp>

// project
#include "boid.hpp"
#include "scene.hpp"
#include "cgra/cgra_mesh.hpp"


using namespace glm;
using namespace std;


vec3 Boid::color() const {
	if (flockID == 0) {
		return vec3(0, 1, 0);
	}
	if (flockID == 1) {
		return vec3(0, 0, 1);
	}
	if (flockID == 2) {
		return vec3(1, 0, 0);
	}
	if (flockID == 3) {
		return vec3(1, 0, 1);
	}
	if (flockID == 4) {
		return vec3(1, 1, 0);
	}
	if (flockID == 5) {
		return vec3(0, 1, 1);
	}
	if (flockID == 6) {
		return vec3(1, 1, 1);
	}
}



vec3 limit(vec3 v, float max) {
	float mag = length(v);
	if (mag > max)
		v = normalize(v) * vec3(max);
	return v;
}

vec3 limitMin(vec3 v, float min) {
	float mag = length(v);
	if (mag < min && mag != 0)
		v = normalize(v) * vec3(min);
	return v;
}


void Boid::calculateForces(Scene *scene) {
	float MAXACC = scene->boidMaxAcc;
	//-------------------------------------------------------------
	// [Assignment 3] :
	// Calculate the forces affecting the boid and update the
	// acceleration (assuming mass = 1).
	// Do NOT update velocity or position in this function.
	// Core : 
	//  - Cohesion
	//  - Alignment
	//  - Avoidance
	//  - Soft Bound (optional)
	// Completion : 
	//  - Cohesion and Alignment with only boids in the same flock
	//  - Predator Avoidance (boids only)
	//  - Predator Chase (predator only)
	// Challenge : 
	//  - Obstacle avoidance
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	//for (int i = 0; i < scene->boids().size(); i ++) {

	vec3 positionSum = vec3 (0,0,0);
	vec3 avoidance(0,0,0);

	vec3 velocitySum = vec3(0,0,0);
	vec3 predatorSum(0,0,0);
	vec3 acc(0, 0, 0);

	int count = 0;
	int distanceValue = scene->NeighbourDistance;

	for (Boid currentBoid : scene->boids()) {
		float dist = glm::distance(m_position, currentBoid.m_position);

		if (dist < distanceValue && dist != 0 && flockID != 2) {
			if (flockID == currentBoid.flockID) {
				positionSum += currentBoid.m_position;
				velocitySum += currentBoid.m_velocity;
				count++;
			}
			avoidance += (m_position - currentBoid.m_position) / vec3(dist * dist);
		}
		if (currentBoid.flockID == 2 && dist!=0) {
			avoidance += ((m_position - currentBoid.m_position) / vec3(dist * dist)) * vec3(5);
		}
	}

	if (flockID == 2) {
		vec3 nearestBoidPos = vec3(INT_MAX, INT_MAX, INT_MAX);
		int count = 0;
		for (Boid b : scene->boids()) {
			if (b.flockID != 2 && b.flockID != 3) {
				count++;
				if (   ((b.m_velocity.x * m_velocity.x) > 0) 
					|| ((b.m_velocity.y * m_velocity.y) > 0) 
					|| ((b.m_velocity.z * m_velocity.z) > 0)) {
						
					if (glm::distance(m_position, b.m_position) < glm::distance(m_position, nearestBoidPos)) {
							nearestBoidPos = b.m_position;
					}
				}
			}
		}
		if (count != 0) { //Prevents predators from flying away when all boids are eaten
			acc = (nearestBoidPos - m_position) * 2.0f;
		}
	}

	//Boid Avoidance from Predators
	for (int i = 0; i < scene->totalPredatorCount; i++) {
		Boid p = scene->boids()[i];
		float pDist = glm::distance(m_position, p.m_position);

		if (pDist < distanceValue/scene->predatorEatDistance && flockID != 2) {
			flockID = 3;
		}
		if (pDist < distanceValue * 5 && flockID != 2 && flockID != 3) {
			predatorSum += (m_position - p.m_position) * scene->avoidanceFromPredators;
		}
	}



	float predator = 0;
	if (flockID == 2) {
		predator = scene->predatorAccelerationInfluence;
	}

	if (count > 0 && flockID != 2) {
		
		avoidance = scene->AvoidanceWeight * avoidance * vec3(1);
		avoidance = limit(avoidance, MAXACC + predator);
		positionSum = scene->CohesionWeight * ((positionSum / vec3(count)) - m_position);
		positionSum = limit(positionSum, MAXACC + predator);
		velocitySum = scene->AlignmentWeight * velocitySum / vec3(count);
		velocitySum = limit(velocitySum, MAXACC + predator);
		acc = positionSum + velocitySum + avoidance + predatorSum;
		acc *= 10.0f;
	}

	if (scene->wrapMode == ("force")) {
		float confinementWeight = scene->confinementWeight * 100;
		if (m_position.x > scene->bound().x) {
			acc += confinementWeight * vec3(-1, 0, 0);
		}
		if (m_position.x < -scene->bound().x) {
			acc += confinementWeight * vec3(1, 0, 0);
		}

		if (m_position.y > scene->bound().y) {
			acc += confinementWeight * vec3(0, -1, 0);
		}
		if (m_position.y < -scene->bound().y) {
			acc += confinementWeight * vec3(0, 1, 0);
		}

		if (m_position.z > scene->bound().z) {
			acc += confinementWeight * vec3(0, 0, -1);
		}
		if (m_position.z < -scene->bound().z) {
			acc += confinementWeight * vec3(0, 0, 1);
		}
		acc = limit(acc, MAXACC + predator);
	}


	if (scene->drawSpheres == true) {
		int indexOfClosestSphere = -1;
		float minimumT = 100; //How close boids can see spheres
		for (int i = 0; i < scene->numberOfSpheres; i++) {
			Sphere S = scene->Spheres[i];
			glm::vec3 d = normalize(m_velocity);
			float r = S.radius + scene->boidSize; //Plus boid size
			float a = length(d);
			a = a * a;
			glm::vec3 oc = m_position - S.centre;
			float b = 2.0 * dot(oc, d);
			float c = (length(oc) * length(oc)) - (r * r);
			float bac = b * b - 4.0f * a * c;
			if (bac >= 0) {
				bac = sqrtf(bac);
				float t0 = (-b + bac) / (2.0f * a);
				float t1 = (-b - bac) / (2.0f * a);
				if ((t1 > 0 && t0 < 0) || (t1 < 0 && t0>0)) {
					minimumT = 0;
					indexOfClosestSphere = i;
				}
				if (t1 >= 0 && t0 >= 0) {
					if (t0 <= minimumT) {
						minimumT = t0;
						indexOfClosestSphere = i;
					}
					if (t1 <= minimumT) {
						minimumT = t1;
						indexOfClosestSphere = i;
					}
				}
			}

		}
		float sphereAvoidanceWeight = scene->sphereAvoidanceWeight;
		if (indexOfClosestSphere != -1) {
			glm::vec3 sphereCentre = scene->Spheres[indexOfClosestSphere].centre;
			glm::vec3 co = sphereCentre - m_position;
			glm::vec3 d = normalize(m_velocity);
			float oe = dot(co, d);
			glm::vec3 e = m_position + oe * d;
			glm::vec3 ce = normalize(e - sphereCentre);
			glm::vec3 dif = limit(sphereAvoidanceWeight * ce, MAXACC * 2);
			acc += dif;
		}
	}

	m_acceleration = acc;

	
	


}

void Boid::update(float timestep, Scene* scene) {
	float minVelocity = scene->MinimumSpeed;
	float maxVelocity = scene->MaximumSpeed;
	//-------------------------------------------------------------
	// [Assignment 3] :
	// Integrate the velocity of the boid using the timestep.
	// Update the position of the boid using the new velocity.
	// Take into account the bounds of the scene which may
	// require you to change the velocity (if bouncing) or
	// change the position (if wrapping).
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...
	
	float predator = 0;
	if (flockID == 2) {
		predator = scene->predatorVelocityInfluence;
	}
	m_velocity += m_acceleration * timestep;
	m_velocity = limit(m_velocity, maxVelocity + predator);
	m_velocity = limitMin(m_velocity, minVelocity + predator);
	m_position += (m_velocity)*timestep;



	

	if (scene->wrapMode == ("bounce")) {
		if (m_position.x > scene->bound().x || m_position.x < -scene->bound().x) {
			m_velocity.x=-m_velocity.x;
				m_position.x+=m_velocity.x*timestep;
		}
		if (m_position.y > scene->bound().y || m_position.y < -scene->bound().y) {
			m_velocity.y = -m_velocity.y;
			m_position.y += m_velocity.y * timestep;
		}
		if (m_position.z > scene->bound().z || m_position.z < -scene->bound().z) {
			m_velocity.z = -m_velocity.z;
			m_position.z += m_velocity.z * timestep;
		}

	}

	if (scene->wrapMode == ("wrap")) {
		//Wrap Bounding Box
		if (m_position.x > scene->bound().x) {
			m_position.x = m_position.x - scene->bound().x * 2;
		}
		if (m_position.x < scene->bound().x * -1) {
			m_position.x = m_position.x + scene->bound().x * 2;
		}

		if (m_position.y > scene->bound().y) {
			m_position.y = m_position.y - scene->bound().y * 2;
		}
		if (m_position.y < scene->bound().y * -1) {
			m_position.y = m_position.y + scene->bound().y * 2;
		}

		if (m_position.z > scene->bound().z) {
			m_position.z = m_position.z - scene->bound().z * 2;
		}
		if (m_position.z < scene->bound().z * -1) {
			m_position.z = m_position.z + scene->bound().z * 2;
		}
	}


}