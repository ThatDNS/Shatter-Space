// @file: Particles.cpp
//
// @brief: Cpp file for Particles class.
// Responsible for generating and rendering particles.

#include "stdafx.h"
#include "App/app.h"
#include "Engine/Components/Particles.h"
#include "Engine/Components/Entity.h"
#include "Engine/Math/Random.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Systems/RenderSystem.h"

Particles::Particles()
{
	type = ParticlesC;
	particlePool.resize(maxParticles);
}

void Particles::Emit(int num)
{
	while (num)
	{
		Particle& particle = particlePool[particleIdx];
		particle.isActive = true;

		particle.position = GetEntity()->GetTransform().position;
		// Set random rotation between [-PI/2, PI/2]
		particle.rotation = (Random::Get().Float() * PI) - PI / 2;
		particle.rotationDelta = Random::Get().Float() * 0.5f;

		particle.lifeSpent = 0.0f;

		particle.alpha = 1.0f;
		particle.color = { 0.0f, 1.0f, 0.0f };
		particle.alphaDelta = Random::Get().Float() * 0.2f;

		if (particleType == EXPLOSION)
		{
			particle.speed = Random::Get().Float();
			// Explosion particles go away from center
			particle.velocityDir.x = Random::Get().Float();
			particle.velocityDir.y = Random::Get().Float();
			if (Random::Get().Float() > 0.5f) particle.velocityDir.x = -particle.velocityDir.x;
			if (Random::Get().Float() > 0.5f) particle.velocityDir.y = -particle.velocityDir.y;
			particle.velocityDir.Normalize();

			particle.lineLength = 2.0f;
			particle.lineDelta = Random::Get().Float() * 0.1f;
		}

		--num;
		particleIdx = --particleIdx % particlePool.size();
	}
}

void Particles::Update(float deltaTime)
{
	// Update the existing particles
	if (particleType == EXPLOSION)
	{
		for (Particle& particle : particlePool)
		{
			if (!particle.isActive)
				continue;

			// Update life
			particle.lifeSpent += deltaTime;
			if (particle.lifeSpent > particle.lifeTime)
			{
				particle.isActive = false;
				continue;
			}

			// Move particle as per the velocity
			particle.position += particle.velocityDir * particle.speed * (deltaTime / 100.0f);

			// Rotate particle
			particle.rotation += particle.rotationDelta * (deltaTime / 100.0f);

			// Update color
			particle.alpha -= particle.alphaDelta * (deltaTime / 100.0f);
			if (particle.color.x > 0.0f) particle.color.x = particle.alpha;
			if (particle.color.y > 0.0f) particle.color.y = particle.alpha;
			if (particle.color.z > 0.0f) particle.color.z = particle.alpha;

			if (particleType == EXPLOSION)
			{
				particle.lineLength -= particle.lineDelta;
				particle.lineLength = std::max(0.1f, particle.lineLength);
			}
		}
	}
}

void Particles::Render()
{
	if (particleType == EXPLOSION)
	{
		for (Particle& particle : particlePool)
		{
			if (!particle.isActive)
				continue;

			Vector3 lineStart, lineEnd;
			ComputeLineEdges(particle.position, particle.lineLength, particle.rotation, lineStart, lineEnd);

			// ---------- Render the line ----------

			// View matrix
			Matrix4x4 mView = RenderSystem::Get().GetViewMatrix();

			// Projection matrix
			Matrix4x4 mProj = RenderSystem::Get().GetProjectionMatrix();

			// Transform to view space
			lineStart = mView * lineStart;
			lineEnd = mView * lineEnd;
			// Projection
			lineStart = mProj * lineStart;
			lineEnd = mProj * lineEnd;
			// Normalize
			lineStart /= lineStart.w;
			lineEnd /= lineEnd.w;

			// Scale
			lineStart += 1.0f;
			lineStart.x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			lineStart.y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
			lineEnd += 1.0f;
			lineEnd.x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			lineEnd.y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;

			// Draw the line
			App::DrawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, particle.color.x, particle.color.y, particle.color.z);
		}
	}
}

void Particles::ComputeLineEdges(const Vector3& center, float length, float rotation, Vector3& edge1, Vector3& edge2)
{
	float halfLengthX = 0.5f * length * std::cos(rotation);
	float halfLengthY = 0.5f * length * std::sin(rotation);

	// Endpoints
	edge1.x = center.x - halfLengthX;
	edge1.y = center.y - halfLengthY;
	edge1.z = center.z;

	edge2.x = center.x + halfLengthX;
	edge2.y = center.y + halfLengthY;
	edge2.z = center.z;
}
