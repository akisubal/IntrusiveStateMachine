#include "intrusive_state_machine.hpp"

//// Class For Sample IntrusiveStateMachine
class Tester
{
public:
	typedef 
		ism::IntrusiveStateMachine<Tester>::StateFuncObject StateFunc;
	typedef 
		ism::IntrusiveStateMachine<Tester>::Behavior Behavior;

	Tester() 
		:m_state_machine(*this, &Tester::Sleeping)
		,m_counter(0) 
	{ } 

	void AddBehavior()
	{
		ism::IntrusiveStateMachine<Tester>::Behavior( )
			.Exec(&Tester::ResetCounter)
			.From(ism::IntrusiveStateMachine<Tester>::StateSets(&Tester::Walking).And(&Tester::InAnger))
			.To(&Tester::Sleeping);
	}

	void Update( ) { 
		m_state_machine.Transit( ); }
	bool IsEnd() const { return m_state_machine.Is(NULL); }

	StateFunc Sleeping( )
	{
		++m_counter;
		if (2 < m_counter) { return &Tester::Walking; }
		return &Tester::Sleeping;
	}

	StateFunc Walking( )
	{
		++m_counter;
		if (3 < m_counter) { return &Tester::InAnger; }
	
		return &Tester::Walking;
	}

	StateFunc InAnger( )
	{
		++m_counter;
		if (2 < m_counter) { return &Tester::Walking; }

		return &Tester::InAnger;
	}
	
	bool IsSleeping( ) const 
	{
		return m_state_machine.Is(&Tester::Sleeping);
	}

	bool IsWalking( ) const 
	{
		return m_state_machine.Is(&Tester::Walking);
	}
	
private:
	void ResetCounter( )
	{
		m_counter = 0;
	}

private:
	ism::IntrusiveStateMachine<Tester> m_state_machine;
	int m_counter;
};

TEST(IntrusiveStateMachine, Init)
{
	Tester tester;

	EXPECT_TRUE(tester.IsSleeping( ));
	tester.Update( );
	EXPECT_TRUE(tester.IsSleeping( ));
	tester.Update( );
	EXPECT_TRUE(tester.IsSleeping( ));
	tester.Update( );
	EXPECT_TRUE(tester.IsWalking( ));

}
