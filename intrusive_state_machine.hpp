#include <vector>
#include <algorithm>
#include <cstdlib>

template <class Parent>
class IntrusiveStateMachine
{
public:
	class StateFuncObject;
	typedef StateFuncObject (Parent::*StateFunc)();

	typedef void (Parent::*AtTransitFunc)( );

	struct Transition
	{
		Transition(StateFunc p, StateFunc n)
			: prev(p)
			, next(n)
		{}

		StateFunc		prev;
		StateFunc		next;
	};

	struct AtTransitBehavior
	{
		AtTransitBehavior(Transition t, AtTransitFunc b)
			: transition(t)
			, behavior(b)
		{}
		Transition		transition;
		AtTransitFunc	behavior;
	};

	IntrusiveStateMachine(Parent& parent, StateFunc init_state)
		: m_parent(parent)
		, m_current_state_func(init_state)
		, m_at_transit_behavior( )
	{}


	struct ApplyAtTransitionFunc
	{
		ApplyAtTransitionFunc(Parent& p, StateFunc c, StateFunc n)
			: parent(p)
			, current(c)
			, next(n)
		{}

		void operator( )(AtTransitBehavior behavior) const 
		{
			if (current != behavior.transition.prev) { return; }
			if (next != behavior.transition.next) { return; }

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
			ApplyAtTransitionFunc(m_parent, m_current_state_func, next)
			);
		

		m_current_state_func = next;
	}


	bool Is(StateFunc state_func) const
	{
		return m_current_state_func == state_func;
	}

	void AddAtTransitBehavior(AtTransitBehavior new_behavior)
	{
		m_at_transit_behavior.push_back(new_behavior);
	}





private:
	Parent&		m_parent;
	StateFunc	m_current_state_func;
	typedef std::vector<AtTransitBehavior> Behavior;
	Behavior m_at_transit_behavior;







public:
	class StateFuncObject
	{
	public:
		StateFuncObject(StateFunc s) : m_state_func(s) {}
		operator StateFunc( ){ return m_state_func; }
	private:
		StateFunc m_state_func;
	};
};
