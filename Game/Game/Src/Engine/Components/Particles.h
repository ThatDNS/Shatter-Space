// @file: Particles.h
//
// @brief: Header file for Particles class.
// Responsible for generating and rendering particles.

#pragma once
#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "Engine/Components/Renderable.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Triangle.h"

enum ParticleType
{
	EXPLOSION,
	PROPULSION,
	SPEEDLINE,
	STARS
};

class Particles : public Renderable
{
private:
	ParticleType particleType = EXPLOSION;
	const size_t maxParticles = 500;

	// Configurable props
	Vector3 positionOffset{ 0.0f, 0.0f, 0.0f };  // Difference between entity position & particle position
	Vector3 particleStartColor{ 1.0f, 0.0f, 0.0f };
	Vector3 particleEndColor{ 0.0f, 0.0f, 1.0f };

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

		// For explosion, this is line length
		// For propulsion, this is triangle length
		float lineLength = 2.0f;
		float lineDelta = 0.0f;
	};

	size_t particleIdx = 0;
	std::vector<Particle> particlePool;

	// Utility functions
	void ComputeLineVertices(const Vector3& center, float length, float rotation, Vector3& edge1, Vector3& edge2);
	void ComputeTriangleVertices(const Vector3& center, float length, float rotation, Triangle& tri);

	void InitiateExplosionParticle(Particle& particle);
	void InitiatePropulsionParticle(Particle& particle, Vector3& direction);
	void InitiateSpeedlineParticle(Particle& particle);
	void InitiateStarParticle(Particle& particle);

protected:
	// Protected destructor so that only Entity can delete it
	~Particles() = default;

	void Initialize() override { }
	void Update(float) override;
	void Render() override;
	void Destroy() override { }

public:
	Particles();

	void SetPositionOffset(Vector3& offset) { positionOffset = offset; }
	void SetParticleColors(Vector3& startColor, Vector3& endColor) { particleStartColor = startColor; particleEndColor = endColor; }
	
	// Must be done before initialization.
	void SetParticleType(ParticleType p) { particleType = p; }

	// Emit n particles
	void Emit(int num, Vector3 direction = {0.0f, 0.0f, 0.0f});
};

#endif // !_PARTICLES_H_
