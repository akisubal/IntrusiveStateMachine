#include "mathematical_set.hpp"
namespace ism
{

TEST(MathematicalSet,  Empty)
{
	MathematicalSet<int> empty(MathematicalSet<int>::EmptySet( ));
	EXPECT_FALSE(empty.Contains(2));
}

TEST(MathematicalSet,  Universal)
{
	MathematicalSet<int> universal(MathematicalSet<int>::UniversalSet( ));
	EXPECT_TRUE(universal.Contains(2));
}

TEST(MathematicalSet, IncludeSetAdd)
{
	MathematicalSet<int> set;
	EXPECT_FALSE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));

	set.Add(1);

	EXPECT_TRUE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));
}

TEST(MathematicalSet, IncludeSetRemove)
{
	MathematicalSet<int> set(1);
	set.Add(2);

	EXPECT_TRUE(set.Contains(1));
	EXPECT_TRUE(set.Contains(2));

	set.Remove(2);

	EXPECT_TRUE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));
}

TEST(MathematicalSet, Complementaly)
{
	MathematicalSet<int> set;
	set.Add(1);

	EXPECT_TRUE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));

	set = ~ set;
	EXPECT_FALSE(set.Contains(1));
	EXPECT_TRUE(set.Contains(2));
}

TEST(MathematicalSet, ExcludesAdd)
{
	MathematicalSet<int> set(1);
	set.Add(2);

	set = ~ set;

	EXPECT_FALSE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));

	set.Add(1);
	EXPECT_TRUE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));
}

TEST(MathematicalSet, ExcludesRemove)
{
	MathematicalSet<int> set(1);

	set = ~ set;

	EXPECT_FALSE(set.Contains(1));
	EXPECT_TRUE(set.Contains(2));

	set.Remove(2);
	EXPECT_FALSE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));
}


}
