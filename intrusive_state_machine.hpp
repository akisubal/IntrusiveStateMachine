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
		Transition()
			: prev(NULL)
			, next(NULL)
		{}

		Transition& From(StateFunc p)
		{
			prev = p;
			return *this;
		}

		Transition& To(StateFunc n)
		{
			next = n;
			return *this;
		}

		StateFunc		prev;
		StateFunc		next;
	};

	struct Behavior
	{
		Behavior(Transition t = Transition( ), AtTransitFunc b = NULL)
			: transition(t)
			, behavior(b)
		{}



		Transition		transition;
		AtTransitFunc	behavior;

		Behavior& Exec(AtTransitFunc f)
		{
			behavior = f;
			return *this;
		}

		Behavior& From(StateFunc f)
		{
			transition.prev = f;
			return *this;
		}
		Behavior& To(StateFunc f)
		{
			transition.next = f;
			return *this;
		}
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

		void operator( )(Behavior behavior) const 
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

	void Add(Behavior behavior)
	{
		m_at_transit_behavior.push_back(behavior);
	}


	bool Is(StateFunc state_func) const
	{
		return m_current_state_func == state_func;
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
	private:
		StateFunc m_state_func;
	};
};
