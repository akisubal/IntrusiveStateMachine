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
	struct AtTransitBehavior
	{
		StateFunc		prev;
		StateFunc		next;
		AtTransitFunc	behavior;
	};

	IntrusiveStateMachine(Parent& parent, StateFunc init_state)
		: m_parent(parent)
		, m_current_state_func(init_state)
		, m_at_transit_behavior( )
	{}

	void Transit( )
	{
		if (m_current_state_func == NULL) { return; }

		const StateFunc next = (m_parent.*m_current_state_func)( );


		for (
				typename Behavior::iterator itr = m_at_transit_behavior.begin( );
				itr != m_at_transit_behavior.end( );
				++itr
			) 
		{
			if (m_current_state_func != (*itr).prev) { continue; }
			if (next != (*itr).next) { continue; }
			(m_parent.*(*itr).behavior)( );
		}

		

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
