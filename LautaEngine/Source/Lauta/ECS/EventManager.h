#pragma once
#pragma once

#include <vector>
#include <map>
#include <string>

///////////////////////////////////////////////////EXAMPLE OF USING THE EVENT MANAGER
/*

EventManager *myEventManager = EventManager::Instance();

class GameObject : public Component				//		LETS SAY WE HAVE AN RPG WITH A  
{												//		CHARACTER WE CAN LEVEL UP

public:

	GameObject(){
		auto& dude(manager.addEntity());
		dude.addComponent<StatComponent>(5, 3);
		stats = &dude.GetComponent<StatComponent>();
	}
	void getDudeStats() {
		stats->getStats();
	}											

	void LevelUp()								//		THIS IS OUR EVENT THAT WILL BE TRIGGERED
	{											//		EVENTS CANNOT HAVE PARAMETERS
		stats->changeStats(8, 5);				//		SO THEY CAN ONLY BE:	
	}											//		void function(){}

private:
	StatComponent* stats;
};


int main()
{
	myEventManager->createEvent("LevelUP");		//		CREATE AN EVENT AND GIVE IT AN ID
												//
	GameObject *dude = new GameObject();		//
	dude->getDudeStats();						//		AT THIS POINT, STATS ARE = (5,3)
	//////////////////////////////////////////////		
	//  __
	//  ||	SUBSCRIBE YOUR GAMEOBJECT TO THE
	// _||_ EVENT ID, AND GIVE IT THE FUNCTION
	// \  /	YOU WANT TO TRIGGER
	//	\/
	//
	//////////////////////////////////////////////
	myEventManager->subscribe("LevelUP", dude, &Creature::LevelUp);


	myEventManager->execute("LevelUP");			//		NOW THE LEVEL UP EVENT IS TRIGGERED
												//		AND BECAUSE THE CREATURE WAS SUBBED
	dude->getDudeStats();						//		TO THE LEVELUP FUNCTION
												//		ITS STATS ARE NOW = (8,5)
}
*/

// Abstract Class for EventHandler to notify of a change
class EventHandlerBase {
public:
	virtual void execute() = 0;
};

// Event Handler Class : Handles Callback


/**********************/
template <typename Class>
class EventHandler : public EventHandlerBase {
	// Defining type for function pointer
	typedef void (Class::*_fptr)(void);

public:
	// Object of the Listener
	Class *object;
	// Function for callback
	_fptr function;

	EventHandler(Class *obj, _fptr func) {
		object = obj;
		function = func;
	}

	void execute() {
		(object->*function)();
	}
};

// Class to create a event
class Event {
	// To store all listeners of the event
	typedef std::map<int, EventHandlerBase*> EventHandlerMap;
	EventHandlerMap handlers;
	int count;
public:

	/************************/
	template < typename Class>
	void addListener(Class *obj, void (Class::*func)(void)) {
		handlers[count] = new EventHandler<Class>(obj, func);
		count++;
	}

	void execute() {
		for (EventHandlerMap::iterator it = handlers.begin(); it != handlers.end(); ++it) {
			it->second->execute();
		}
	}

};

class EventManager {
	struct EventType {
		Event *event;
		std::string name;
	};

	std::vector<EventType> _events;

	static EventManager *_Instance;

	EventManager() {};
public:
	static EventManager* Instance() {
		if (!_Instance) {
			_Instance = new EventManager();
		}
		return _Instance;
	}

	void createEvent(std::string name) {
		for (std::vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
			EventType e = *it;
			if (e.name.compare(name) == 0)
				return;
		}
		EventType e;
		e.event = new Event();
		e.name = name;
		_events.push_back(e);
	}

	template <typename Class>
	bool subscribe(std::string name, Class *obj, void (Class::*func)(void)) {
		for (std::vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
			EventType e = *it;
			if (e.name.compare(name) == 0) {
				e.event->addListener(obj, func);
				return true;
			}
		}
		return false;
	}

	void execute(std::string name) {
		for (std::vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
			EventType e = *it;
			if (e.name.compare(name) == 0) {
				e.event->execute();
			}
		}
	}
};


