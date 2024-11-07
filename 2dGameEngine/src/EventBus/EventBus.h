#pragma once

#include "../Logger/Logger.h"
#include "../EventBus/Event.h"
#include <map>
#include <typeindex>
#include <list>
#include <memory>

class IEventCallback
{
private:
	virtual void Call(Event& e) = 0;
public:
	virtual ~IEventCallback() = default;

	void Execute(Event& e)
	{
		Call(e);
	}
};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback
{
	/*
	* Here we have the callback function pointer that needs to be invoked
	*/
private:
	typedef void (TOwner::* CallbackFunction)(TEvent&);

	TOwner* ownerInstance;
	CallbackFunction callbackFunction;

	virtual void Call(Event& e) override
	{
		std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
	}
	
public:
	EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction)
		: ownerInstance(ownerInstance), callbackFunction(callbackFunction)
	{
	}

	virtual ~EventCallback() override = default;
};

using HandlerList = std::list<std::unique_ptr<IEventCallback>>;

class EventBus
{
private:
	/*
	* This map will contain event as key and a list of callback functions as Value
	* Example: <Key, Value> = <CollisionEvent, [subscriberCallback*, subscriberCallback*] 
	*/
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
	EventBus()
	{
		Logger::Log("EventBus constructor called!");
	}

	~EventBus()
	{
		Logger::Log("EventBus destructor called!");
	}

	//Clears the subscribers list
	void Reset()
	{
		subscribers.clear();
	}

	/*
	* Subscribe to an event type <T>
	* In our implementation, a listener subscribe to an event
	* Example: eventBus->SubscribeToEvent<CollisionEvent>(this, &Game::OnCollision);
	*/
	template <typename TEvent, typename TOwner>
	void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::* callbackFunction)(TEvent&))
	{
		if (!subscribers[typeid(TEvent)].get())
		{
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}
		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);

		// Since the subscriber is a unique pointer, we need to use std::move when we want to push it to a map. We use std::move to change the ownership of an object from one unique_ptr to another unique_ptr.
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}

	/*
	* Emit an event type <T>
	* In our implementation, as soon as something emits an event we go ahead and execute all the listener callback functions
	* Example: eventBus->EmitEvent<CollisionEvent>(player, enemy);
	*/
	template<typename TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args)
	{
		auto handlers = subscribers[typeid(TEvent)].get();
		if (handlers)
		{
			for (auto it = handlers->begin(); it != handlers->end(); it++)
			{
				auto handler = it->get();
				TEvent event(std:: forward<TArgs>(args)...);
				handler->Execute(event);
			}
		}
	}
};