#include "intrusive_state_machine.hpp"
#include <iostream>
#include <cstdlib>

class Tester
{
public:
	Tester( ) 
		:m_state_machine(*this, &Tester::Hello)
		,m_counter(0) 
	{
		const IntrusiveStateMachine<Tester>::AtTransitBehavior behavior ( 
				IntrusiveStateMachine<Tester>::Transition (
					&Tester::Hello, 
					&Tester::Bye 
				), 
				&Tester::ChangeMind
			);

		m_state_machine.AddAtTransitBehavior(behavior);
	} 

	void Update( ) { m_state_machine.Transit( ); }

	bool IsEnd() const { return m_state_machine.Is(NULL); }



private:
	IntrusiveStateMachine<Tester>::StateFuncObject Hello( )
	{
		std::cout << "Hello" << std::endl; 
		++m_counter;
		if (m_counter < 10) { return &Tester::Hello; }

		return &Tester::Bye;
	}

	IntrusiveStateMachine<Tester>::StateFuncObject Bye( )
	{
		std::cout << "Bye!" << std::endl; 

		return NULL;
	}

	void ChangeMind( )
	{
		std::cout << "...Are you Tired?" << std::endl;
	}



private:
	IntrusiveStateMachine<Tester> m_state_machine;
	int m_counter;
};

int main(int argc,  char* argv[]) {
	Tester tester;
	while (! tester.IsEnd( )) {
		tester.Update( );
	}
 
	return 0;
}

