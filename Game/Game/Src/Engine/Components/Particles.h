// @file: Particles.h
//
// @brief: Header file for Particles class.
// Responsible for generating and rendering particles.

#pragma once
#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "Engine/Components/Renderable.h"
#include "Engine/Math/Vector3.h"

enum ParticleType
{
	EXPLOSION,
	PROPULSION
};

class Particles : public Renderable
{
private:
	ParticleType particleType = EXPLOSION;
	const size_t maxParticles = 500;

	struct Particle
	{
		bool isActive = false;

		Vector3 position;
		float rotation = 0.0f;
		float rotationDelta = 0.0f;

		// Particle life (in ms)
		float lifeTime = 1000.0f;
		float lifeSpent = 0.0f;

		// Velocity
		float speed = 0.0f;
		Vector3 velocityDir;

		// Color & Alpha
		Vector3 color;
		float alpha = 1.0f;
		float alphaDelta = 0.0f;

		// Explosion particle specific variables
		float lineLength = 2.0f;
		float lineDelta = 0.0f;
	};

	size_t particleIdx = maxParticles - 1;
	std::vector<Particle> particlePool;

	// Utility functions
	void ComputeLineEdges(const Vector3& center, float length, float rotation, Vector3& edge1, Vector3& edge2);

protected:
	// Protected destructor so that only Entity can delete it
	~Particles() = default;

	void Initialize() override { }
	void Update(float) override;
	void Render() override;
	void Destroy() override { }

public:
	Particles();

	// Emit n particles
	void Emit(int);
};

#endif // !_PARTICLES_H_
