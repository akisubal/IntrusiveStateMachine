#include <list>
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


	class StateSets
	{
	public:
		StateSets()
			: m_sets( )
		{}

		StateSets(StateFunc s) : m_sets( )
		{
			m_sets.push_back(s);
		}

		void Add(StateFunc s)
		{
			m_sets.push_back(s);
		}
		StateSets& And(StateFunc s)
		{
			Add(s);
			return *this;
		}

		bool Includes(StateFunc s) const {
			return std::find(m_sets.begin( ), m_sets.end( ),  StateFuncObject(s)) != m_sets.end( );
		}


	private:
		std::list<StateFuncObject> m_sets;
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
			if (! behavior.condition.prev.Includes(current)) { return; }
			if (! behavior.condition.next.Includes(next)) { return; }

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

		bool operator==(const StateFuncObject& s) const 
		{
			return m_state_func == s.m_state_func;
		}
	private:
		StateFunc m_state_func;
	};
};
