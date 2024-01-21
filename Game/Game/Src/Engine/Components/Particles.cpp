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
		particle.lifeSpent = 0.0f;

		particle.alpha = 1.0f;
		particle.color = particleStartColor;
		particle.alphaDelta = Random::Get().Float() * 0.2f;

		if (particleType == EXPLOSION)
			InitiateExplosionParticle(particle);
		else if (particleType == PROPULSION)
			InitiatePropulsionParticle(particle, direction);
		else if (particleType == SPEEDLINE)
			InitiateSpeedlineParticle(particle);
		else if (particleType == STARS)
			InitiateStarParticle(particle);

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
		else if (particleType == SPEEDLINE)
		{
			points.push_back(Vector3{ particle.position.x, particle.position.y, particle.position.z + positionOffset.z });
			points.push_back(Vector3{ particle.position.x, particle.position.y, particle.position.z + positionOffset.z + particle.lineLength });
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
		if (particleType == EXPLOSION || particleType == SPEEDLINE)
		{
			App::DrawLine(points[0].x, points[0].y, points[1].x, points[1].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
		}
		else if (particleType == PROPULSION)
		{
			App::DrawLine(points[0].x, points[0].y, points[1].x, points[1].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
			App::DrawLine(points[0].x, points[0].y, points[2].x, points[2].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
			App::DrawLine(points[1].x, points[1].y, points[2].x, points[2].y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
		}
		else if (particleType == STARS)
		{
			// Star positions are already in projection space
			float x = particle.position.x;
			float y = particle.position.y;
			float cV = std::acosf(PI / 3.0f);
			float sV = std::asinf(PI / 3.0f);
			float l = particle.lineLength / 2.0f;

			App::DrawLine(x - l,      y,          x + l,      y, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
			App::DrawLine(x - l * cV, y - l * sV, x + l * cV, y + l * sV, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
			App::DrawLine(x - l * cV, y + l * sV, x + l * cV, y - l * sV, particle.color.x * particle.alpha, particle.color.y * particle.alpha, particle.color.z * particle.alpha);
		}
	}
}

void Particles::InitiateExplosionParticle(Particle& particle)
{
	particle.position = GetEntity()->GetTransform().position + positionOffset;
	// Set random rotation between [-PI/2, PI/2]
	particle.rotation = (Random::Get().Float() * PI) - PI / 2;
	particle.rotationDelta = Random::Get().Float() * 0.5f;

	// Explosion particles go away from center
	particle.velocityDir.x = Random::Get().Float();
	particle.velocityDir.y = Random::Get().Float();
	if (Random::Get().Float() > 0.5f) particle.velocityDir.x = -particle.velocityDir.x;
	if (Random::Get().Float() > 0.5f) particle.velocityDir.y = -particle.velocityDir.y;
	particle.velocityDir.Normalize();

	particle.speed = Random::Get().Float();

	particle.lineLength = 2.0f;
	particle.lineDelta = Random::Get().Float() * 0.1f;
}

void Particles::InitiatePropulsionParticle(Particle& particle, Vector3& direction)
{
	particle.position = GetEntity()->GetTransform().position + positionOffset;
	// Set random rotation between [-PI/2, PI/2]
	particle.rotation = (Random::Get().Float() * PI) - PI / 2;
	particle.rotationDelta = Random::Get().Float() * 0.5f;

	// Propulsion particles go opposite to the direction of motion
	// Relying on the programmer to provide correct direcition.
	particle.velocityDir = direction;
	particle.velocityDir.Normalize();
	particle.speed = Random::Get().Float();

	particle.lineLength = 0.5f;
	particle.lineDelta = Random::Get().Float() * 0.05f;
}

void Particles::InitiateSpeedlineParticle(Particle& particle)
{
	// Short lived. No velocity
	particle.lifeTime = 400.0f;
	particle.speed = 0.0f;

	// Lines must appear on the edge of the screen
	// Hardcoding as this is the only legit use of speedlines
	float distFromCenter = Random::Get().Float() * 30.0f + 40.0f;
	float entityZ = GetEntity()->GetTransform().position.z;
	float theta = Random::Get().Float() * (PI / 2.0f);
	Vector3 pos{ distFromCenter * std::cosf(theta), distFromCenter * std::sinf(theta), entityZ };
	if (Random::Get().Float() < 0.5f) pos.x = -pos.x;
	if (Random::Get().Float() < 0.5f && std::abs(pos.x) > 0.8f * distFromCenter) pos.y = -pos.y;
	particle.position = pos;

	// No rotation
	particle.rotation = 0.0f;
	particle.rotationDelta = 0.0f;

	// length b/w [70, 100]
	particle.lineLength = Random::Get().Float() * 30.0f + 70.0f;
	particle.lineDelta = 0.0f;

	particle.alphaDelta = 0.0f;
}

void Particles::InitiateStarParticle(Particle& particle)
{
	// Short lived. No velocity
	particle.lifeTime = 2000.0f;
	particle.speed = 0.0f;

	// Appear randomly on the screen
	float xpos = Random::Get().Float() * APP_VIRTUAL_WIDTH;
	float ypos = Random::Get().Float() * APP_VIRTUAL_HEIGHT;
	particle.position = Vector3(xpos, ypos, 0.0f);

	// No rotation
	particle.rotation = 0.0f;
	particle.rotationDelta = 0.0f;

	particle.lineLength = Random::Get().Float();
	particle.lineDelta = 0.0f;
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
