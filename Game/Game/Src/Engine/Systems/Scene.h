// @file: Scene.h
//
// @brief: Header file for the Scene class. All entities are part of a Scene.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-11-29

#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

class Entity;

/**
 * @class Scene
 *
 * Scene class contains and manages all the entities within it.
 */
class Scene final
{
private:
	std::string guid = "";
	STRCODE uid = 0;
	std::string name = "";

	std::list<Entity*> entitiesToBeAdded;
	std::list<Entity*> entities;
	std::list<Entity*> entitiesToDestroy;

	// Any entity which is part of this becomes dangling
	// Useful in object pooling
	std::list<Entity*> entitiesToUntrack;

protected:
	void Initialize();
	void Load(json::JSON&);

	void PreUpdate();
	void Update();
	void PostUpdate();

	json::JSON Save();

	void Destroy();

public:
	Scene();
	Scene(std::string guid);
	~Scene() = default;

	Entity* CreateEntity();
	Entity* CreateDanglingEntity(bool);
	void AddDanglingEntity(Entity*);

	Entity* FindEntity(std::string& entityGUID);
	Entity* FindEntity(STRCODE entityId);
	std::list<Entity*> FindEntityByName(std::string& entityName);  // entities can have same name
	std::list<Entity*> FindEntityWithComponent(std::string& componentClassName);
	bool RemoveEntity(std::string& entityGUID);
	bool RemoveEntity(STRCODE entityId);
	void UntrackEntity(Entity*);

	// Getters
	const std::string& GetGUID() const { return guid; }
	const STRCODE GetUID() const { return uid; }
	const std::string& GetName() const { return name; }

	friend class SceneManager;
};

#endif // !_SCENE_H_
