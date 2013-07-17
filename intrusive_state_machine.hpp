#pragma once

#include <vector>
#include <algorithm>
#include <cstdlib>
#include "mathematical_set.hpp"

namespace ism
{


template <class Parent>
class IntrusiveStateMachine
{
public:
	class StateFuncObject;
	typedef StateFuncObject (Parent::*StateFunc)();

	typedef void (Parent::*AtTransitFunc)( );


	class StateSets
	{
	public:
		StateSets()
			: m_set(MathematicalSet<StateFunc>::EmptySet( ) )
		{}

		StateSets(StateFunc s) : m_set( )
		{
			Add(s);
		}

		void Add(StateFunc s)
		{
			m_set.Add(s);
		}
		StateSets& And(StateFunc s)
		{
			Add(s);
			return *this;
		}

		bool Contains(StateFunc s) const {
			return m_set.Contains(s);
		}

	private:
		MathematicalSet<StateFunc> m_set;
	};


	struct Condition
	{
		Condition()
			: prev()
			, next()
		{}

		Condition& From(Condition p)
		{
			prev = p;
			return *this;
		}

		Condition& To(Condition n)
		{
			next = n;
			return *this;
		}

		StateSets	prev;
		StateSets	next;
	};

	struct Behavior
	{
		Behavior(Condition c = Condition( ), AtTransitFunc b = NULL)
			: condition(c)
			, behavior(b)
		{}



		Condition		condition;
		AtTransitFunc	behavior;

		Behavior& Exec(AtTransitFunc f)
		{
			behavior = f;
			return *this;
		}

		Behavior& From(StateSets c)
		{
			condition.prev = c;
			return *this;
		}
		Behavior& To(StateSets c)
		{
			condition.next = c;
			return *this;
		}
	};
	


	IntrusiveStateMachine(Parent& parent, StateFunc init_state)
		: m_parent(parent)
		, m_current_state_func(init_state)
		, m_at_transit_behavior( )
	{}



	struct ApplyBehavior
	{
		ApplyBehavior(Parent& p, StateFunc c, StateFunc n)
			: parent(p)
			, current(c)
			, next(n)
		{}

		void operator( )(Behavior behavior) const 
		{
			if (! behavior.condition.prev.Contains(current)) { return; }
			if (! behavior.condition.next.Contains(next)) { return; }

			(parent.*behavior.behavior)( );
		}

		Parent&		parent;
		StateFunc	current;
		StateFunc	next;
	};

	void Transit( )
	{
		if (m_current_state_func == NULL) { return; }

		const StateFunc next = (m_parent.*m_current_state_func)( );

		std::for_each(
			m_at_transit_behavior.begin( ), 
			m_at_transit_behavior.end( ), 
			ApplyBehavior(m_parent, m_current_state_func, next)
			);
		

		m_current_state_func = next;
	}

	void Add(Behavior b)
	{
		m_at_transit_behavior.push_back(b);
	}


	bool Is(StateFunc s) const
	{
		return m_current_state_func == s;
	}





private:
	Parent&		m_parent;
	StateFunc	m_current_state_func;
	std::vector<Behavior> m_at_transit_behavior;







public:
	class StateFuncObject
	{
	public:
		StateFuncObject(StateFunc s) : m_state_func(s) {}
		operator StateFunc( ){ return m_state_func; }

		bool operator==(const StateFuncObject& s) const 
		{
			return m_state_func == s.m_state_func;
		}
	private:
		StateFunc m_state_func;
	};
};


}
