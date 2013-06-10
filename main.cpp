#include "intrusive_state_machine.hpp"
#include <iostream>
#include <cstdlib>

//// Class For Sample IntrusiveStateMachine
class Tester
{
public:
	typedef IntrusiveStateMachine<Tester>::StateFuncObject StateFunc;

	Tester( ) 
		:m_state_machine(*this, &Tester::Hello)
		,m_counter(0) 
	{
		m_state_machine.Add(
			IntrusiveStateMachine<Tester>::Behavior( )
				.Exec(&Tester::ChangeMind)
				.From(&Tester::Hello)
				.To(&Tester::Bye)
		);
	} 

	void Update( ) { m_state_machine.Transit( ); }

	bool IsEnd() const { return m_state_machine.Is(NULL); }


private:
	StateFunc Hello( )
	{
		std::cout << "Hello!" << std::endl; 
		++m_counter;
		if (m_counter < 10) { return &Tester::Hello; }

		return &Tester::Bye;
	}

	StateFunc Bye( )
	{
		std::cout << "Bye..." << std::endl; 

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

