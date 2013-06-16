#include "intrusive_state_machine.hpp"
#include <iostream>
#include <cstdlib>

int RollTwelveFacesDice( ) 
{
	return std::rand( ) % 12;
}

//// Class For Sample IntrusiveStateMachine
class Tester
{
public:
	typedef ism::IntrusiveStateMachine<Tester>::StateFuncObject StateFunc;

	Tester( ) 
		:m_state_machine(*this, &Tester::Sleeping)
		,m_counter(0) 
	{
		m_state_machine.Add(
			ism::IntrusiveStateMachine<Tester>::Behavior( )
				.Exec(&Tester::SayGoodMorning)
				.From(&Tester::Sleeping)
				.To(&Tester::Walking)
		);

		m_state_machine.Add(
			ism::IntrusiveStateMachine<Tester>::Behavior( )
				.Exec(&Tester::ResetCounter)
				.From(ism::IntrusiveStateMachine<Tester>::StateSets(&Tester::Walking).And(&Tester::InAnger))
				.To(&Tester::Sleeping)
		);
	} 

	void Update( ) { m_state_machine.Transit( ); }
	bool IsEnd() const { return m_state_machine.Is(NULL); }



private:
	StateFunc Sleeping( )
	{
		std::cout << "zzz..." << std::endl;

		if (RollTwelveFacesDice( ) < m_counter) {
			return &Tester::Walking;
		}

		if (RollTwelveFacesDice( ) < 1) {
			return &Tester::InAnger;
		}

		++m_counter;

		return &Tester::Sleeping;
	}

	StateFunc Walking( )
	{
		std::cout << "It's nice today." << std::endl;
		if (RollTwelveFacesDice( ) < 3) {
			return &Tester::InAnger;
		}
	
		return &Tester::Walking;
	}

	StateFunc InAnger( )
	{
		std::cout << "Gyaaaaa!!!" << std::endl;

		if (RollTwelveFacesDice( ) < 3) {
			return &Tester::Walking;
		}

		if (RollTwelveFacesDice( ) < 1) {
			return &Tester::Sleeping;
		}

		return &Tester::InAnger;
	}

	

	void SayGoodMorning( )
	{ 
		std::cout << "Hi. Good morning" << std::endl;
	}

	void SayGoodNight( )
	{
		std::cout << "...I'm sleepy. Good night." << std::endl;
	}


	void ResetCounter( )
	{
		m_counter = 0;
	}

private:
	ism::IntrusiveStateMachine<Tester> m_state_machine;
	int m_counter;

};






int main(int argc,  char* argv[]) {
	if (argc < 1) { return 0; }
	Tester tester;
	for (int i(0); i < std::atoi(argv[1]); ++i) {
		tester.Update( );
	}
 
	return 0;
}

