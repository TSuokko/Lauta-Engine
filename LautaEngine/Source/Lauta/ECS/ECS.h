#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

/****************************************************************************************************************************
	How to use the Entity Component System for your game	|				Example											*
															|																*
	1. Add the Manager to your application					|	Manager mngr;												*
	2. Crete an entity	(/in this example, a UI entity)		|	auto& UI(mngr.addEntity());									*
	3. Create an enum of std::size_t of all the groups  	|	enum groupLabels : std::size_t								*
	   you will be using for the project					|	{															*
															|		groupUI													*
															|		groupMap												*
															|		groupPlayer												*
															|	};															*
	4. Retrieve the group label 							|	auto& ui(manager.getGroup(groupUI);							*
	5. Add Components to the entity							|	UI.addComponent<UIComponent>("UI_Layout", groupUI, 1);		*
	   You can check what the parameters for the Component	|	UI.addComponent<UILabel>(Vector2Di(21, 12),	"hello world",	*
	   are by hovering over the component name				|		"Arial", "textID", Vector2Di(30, 50), white);			*
	6. To retrieve a function of the component, use			|	UI.GetComponent<UILabel>().AddTextToScreenplay("newID",		*
       the GetComponent										|		"Lorem ipsum", "Arial");								*
	7. In your render function, use this for loop to 		|	for(auto& u : ui){											*
	   go through all entities marked with the given label.	|		u->draw();												*
	   														|	}															*
	8. In your update function, remember to refresh and		|	mngr.refresh();												*
	   update the manager									|	mngr.update();												*
															|																*	
*****************************************************************************************************************************/

//In this header we have a Manager which holds entities, which are basically anything shown in game.
//Each entity has components, which give the entity functionality.

class Component;
class Entity;
class Manager;

//Will make code much more readable
using ComponentID = std::size_t;
using Group = std::size_t;

//When the inline function is called whole code of the inline function gets inserted
//Everytime we call this function, we are giving the next ID number
inline ComponentID getComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

//When we pass a component, we will give it a component ID
//Rather than having a new (for example position) component having it's own ID,
//All position components will refer to one single ID number
//Will generate a new lastID and put that in our typeID
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

//check if entity has component attached
constexpr std::size_t maxComponents = 32;
//Groups are used for example for render layers / collision layers
constexpr std::size_t maxGroups = 32;

//if we need to know if entity has selection of components, 
//we can compare if it has it or not
using ComponentBitSet = std::bitset<maxComponents>;
//array of component pointers 
using ComponentArray = std::array<Component*, maxComponents>;

using GroupBitSet = std::bitset<maxGroups>;

class Component {

public:

	Entity* entity;

	//virtual functions in case if we need to overwrite them
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	//deconstructor
	virtual ~Component() {};

};

class Entity {

private:

	Manager& m_manager;
	//keep track if entity is active. 
	//if false we can remove it fom the game
	bool m_active = true;

	//List of all the components the entity is holding
	std::vector<std::unique_ptr<Component>> m_components;

	ComponentArray m_compArray;
	ComponentBitSet m_compBitSet;
	GroupBitSet m_groupBitSet;

public:

	Entity(Manager& manager) : m_manager(manager) {};
	//Have the entity loop through all its components
	//and call its update 
	void update()
	{
		for (auto& c : m_components) c->update();
		
	}
	//and draw methods
	void draw()
	{
		for (auto& c : m_components) c->draw();
	}

	//check activity
	bool isActive() { return m_active; }

	bool hasGroup(Group group)
	{
		return m_groupBitSet[group];
	}

	void addGroup(Group group);

	void deleteGroup(Group group)
	{
		m_groupBitSet[group] = false;
	}
	
	//on any given component, we can call the destroy function to remove entities
	void destroy() { m_active = false; }

	//check if entity has matching component attached
	template <typename T> bool hasComponent() const
	{
		return m_compBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... T_Arguments>

	T& addComponent(T_Arguments&&... m_Arguments)
	{
		//This will give the component (arguments included) its unique ID
		T* c(new T(std::forward<T_Arguments>(m_Arguments)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		m_components.emplace_back(std::move(uPtr));
		//when we add a component, it will always be put in the same position in the array
		m_compArray[getComponentTypeID<T>()] = c;
		m_compBitSet[getComponentTypeID<T>()] = true;
		//once we have a added the component to the vector and bitarray
		//we can call the init fucntion of our component, which will get references to other init fucions if needed
		c->init();

		return *c;
	}

	//returns a pointer from a TypeID-position in our component array
	template<typename T> T& GetComponent() const
	{
		auto ptr(m_compArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	//example code for setting a position component in an entity:
	//GameObject.getComponent<PositionComponent>().setXpos(25)
};

class Manager
{
private:

	//Keep a list of all our entities
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:

	//update all of our entities
	void update() 
	{
		for (auto& e : m_entities) e->update();
	}
	//draw all of our entities
	void draw()
	{
		for (auto& e : m_entities) e->draw();
	}

	//move through entities and remove the ones that are not there
	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* entity)
			{
				return !entity->isActive() || !entity->hasGroup(i);
			}),
				std::end(v));
		}

		m_entities.erase(std::remove_if(std::begin(m_entities), 
			std::end(m_entities),[](const std::unique_ptr<Entity> &m_Entity)
		{
			return !m_Entity->isActive();
		}),
			std::end(m_entities));
	}

	void addToGroup(Entity* entity, Group group)
	{
		groupedEntities[group].emplace_back(entity);
	}

	void removeFromGroup(Entity* entity, Group group)
	{
		for (auto it = groupedEntities[group].begin(); it != groupedEntities[group].end();)
		{
			if (*it == entity)
			{
				it = groupedEntities[group].erase(it);
			}
		}
	}

	std::vector<Entity*>& getGroup(Group group)
	{
		return groupedEntities[group];
	}

	//adding an entity to our Manager
	Entity& addEntity()
	{
		//create new entity pointer
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		//add this entity to our entity list
		m_entities.emplace_back(std::move(uPtr));

		return *e;
	}
};