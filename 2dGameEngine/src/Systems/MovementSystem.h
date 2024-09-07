#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System
{
public:
	MovementSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update(double deltaTime)
	{
		//  Loop all the entities that the system is interested in
		for (auto entity : GetSystemEntities())
		{
			// Update entity position based on its velocity every frame of the game loop
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const RigidBodyComponent rigidbody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidbody.velocity.x * deltaTime;
			transform.position.y += rigidbody.velocity.y * deltaTime;

			/*
			Logger::Log(
				"Entity id = " + 
				std::to_string(entity.GetId()) + 
				" position is now (" + std::to_string(transform.position.x) + 
				", " + std::to_string(transform.position.y) + 
				")");
			*/
		}
	}
};