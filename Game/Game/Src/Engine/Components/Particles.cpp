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
	particlePool.clear();
	particlePool.resize(maxParticles);
}

void Particles::Emit(int num, Vector3 direction)
{
	while (num)
	{
		Particle& particle = particlePool[particleIdx];
		particle.isActive = true;

		particle.position = GetEntity()->GetTransform().position + positionOffset;
		// Set random rotation between [-PI/2, PI/2]
		particle.rotation = (Random::Get().Float() * PI) - PI / 2;
		particle.rotationDelta = Random::Get().Float() * 0.5f;

		particle.lifeSpent = 0.0f;

		particle.alpha = 1.0f;
		particle.color = particleStartColor;
		particle.alphaDelta = Random::Get().Float() * 0.2f;

		particle.speed = Random::Get().Float();

		if (particleType == EXPLOSION)
		{
			// Explosion particles go away from center
			particle.velocityDir.x = Random::Get().Float();
			particle.velocityDir.y = Random::Get().Float();
			if (Random::Get().Float() > 0.5f) particle.velocityDir.x = -particle.velocityDir.x;
			if (Random::Get().Float() > 0.5f) particle.velocityDir.y = -particle.velocityDir.y;
			particle.velocityDir.Normalize();

			particle.lineLength = 2.0f;
			particle.lineDelta = Random::Get().Float() * 0.1f;
		}
		else if (particleType == PROPULSION)
		{
			// Propulsion particles go opposite to the direction of motion
			// Relying on the programmer to provide correct direcition.
			particle.velocityDir = direction;
			particle.velocityDir.Normalize();

			particle.lineLength = 0.5f;
			particle.lineDelta = Random::Get().Float() * 0.05f;
		}

		--num;
		particleIdx = (++particleIdx) % particlePool.size();
	}
}

void Particles::Update(float deltaTime)
{
	// Update the existing particles
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
		Vector3::Lerp(particle.color, particleEndColor, (deltaTime / 500.0f));

		particle.lineLength -= particle.lineDelta;
		if (particleType == EXPLOSION)
			particle.lineLength = std::max(0.5f, particle.lineLength);
		else if (particleType == PROPULSION)
			particle.lineLength = std::max(0.2f, particle.lineLength);
	}
}

void Particles::Render()
{
	// View matrix
	Matrix4x4 mView = RenderSystem::Get().GetViewMatrix();

	// Projection matrix
	Matrix4x4 mProj = RenderSystem::Get().GetProjectionMatrix();

	for (Particle& particle : particlePool)
	{
		if (!particle.isActive)
			continue;

		std::vector<Vector3> points;
		if (particleType == EXPLOSION)
		{
			Vector3 lineStart, lineEnd;
			ComputeLineVertices(particle.position, particle.lineLength, particle.rotation, lineStart, lineEnd);
			points.push_back(lineStart);
			points.push_back(lineEnd);
		}
		else if (particleType == PROPULSION)
		{
			Triangle tri;
			ComputeTriangleVertices(particle.position, particle.lineLength, particle.rotation, tri);

			for (Vector3& vec : tri.points)
				points.push_back(vec);
		}

		// Transform the points
		for (Vector3& point : points)
		{
			// Transform to view space
			point = mView * point;
			// Projection
			point = mProj * point;
			// Normalize
			point /= point.w;
			// Scale
			point += 1.0f;
			point.x *= 0.5f * (float)APP_INIT_WINDOW_WIDTH;
			point.y *= 0.5f * (float)APP_INIT_WINDOW_HEIGHT;
		}

		// Render the line / triangle
		if (particleType == EXPLOSION)
		{
			App::DrawLine(points[0].x, points[0].y, points[1].x, points[1].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
		}
		else if (particleType == PROPULSION)
		{
			App::DrawLine(points[0].x, points[0].y, points[1].x, points[1].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
			App::DrawLine(points[0].x, points[0].y, points[2].x, points[2].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
			App::DrawLine(points[1].x, points[1].y, points[2].x, points[2].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
		}
	}
}

void Particles::ComputeLineVertices(const Vector3& center, float length, float rotation, Vector3& edge1, Vector3& edge2)
{
	// Half-lengths in x and y
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

void Particles::ComputeTriangleVertices(const Vector3& center, float length, float rotation, Triangle& tri)
{
	// Angles for each vertex of the equilateral triangle
	float angle1 = rotation;
	float angle2 = angle1 + (2.0f * PI) / 3.0f;
	float angle3 = angle2 + (2.0f * PI) / 3.0f;

	// Half-lengths in x and y
	float halfLengthX = 0.5f * length;
	float halfLengthY = 0.5f * length * std::sqrt(3.0f);

	// Calculate the coordinates of the triangle
	tri.points[0].x = center.x + halfLengthX * std::cos(angle1);
	tri.points[0].y = center.y + halfLengthY * std::sin(angle1);
	tri.points[0].z = center.z;

	tri.points[1].x = center.x + halfLengthX * std::cos(angle2);
	tri.points[1].y = center.y + halfLengthY * std::sin(angle2);
	tri.points[1].z = center.z;

	tri.points[2].x = center.x + halfLengthX * std::cos(angle3);
	tri.points[2].y = center.y + halfLengthY * std::sin(angle3);
	tri.points[2].z = center.z;
}
