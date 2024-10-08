#include "ECS.h"
#include "../Logger/Logger.h"

// Allocating memory for the static variable
int IComponent::nextId = 0;

int Entity::GetId() const
{
	return id;
	System;
}

void System::AddEntityToSystem(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other)
		{
			return entity == other;
		}), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}

Entity Registry::CreateEntity()
{
	int entityId;

	entityId = numEntities++;

	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);

	// Make sure the entityComponentSignatures vector can accomodate the new entity
	if (entityId >= entityComponentSignatures.size())
	{
		entityComponentSignatures.resize(entityId + 1);
	}

	Logger::Log("Entity created with id = " + std::to_string(entityId));
	return entity;
}

void Registry::AddEntityToSystems(Entity entity)
{
	const auto entityId = entity.GetId();

	const auto& entityComponentSignature = entityComponentSignatures[entityId];
	
	for (auto& system : systems)
	{
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested)
		{
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::Update()
{
	// Add the entities that are waiting to be created to the active System
	for (auto entity : entitiesToBeAdded)
	{
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();


	// ToDo: Remove the entities that are waiting to be killed from the active system
}
