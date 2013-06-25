#include <gtest/gtest.h>
#include "mathematical_set.hpp"

TEST(MathematicalSet,  Empty)
{
	ism::MathematicalSet<int> empty(ism::MathematicalSet<int>::EmptySet( ));
	EXPECT_FALSE(empty.Contains(2));
}


int main(int argc,  char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS( );
}
