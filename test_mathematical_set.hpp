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

	set = ~set;
	EXPECT_FALSE(set.Contains(1));
	EXPECT_TRUE(set.Contains(2));

	EXPECT_TRUE(set == ~~set);
}

TEST(MathematicalSet, ExcludesAdd)
{
	MathematicalSet<int> set(1);
	set.Add(2);

	set = ~set;

	EXPECT_FALSE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));

	set.Add(1);
	EXPECT_TRUE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));
}

TEST(MathematicalSet, ExcludesRemove)
{
	MathematicalSet<int> set(1);

	set = ~set;

	EXPECT_FALSE(set.Contains(1));
	EXPECT_TRUE(set.Contains(2));

	set.Remove(2);
	EXPECT_FALSE(set.Contains(1));
	EXPECT_FALSE(set.Contains(2));
}

TEST(MathematicalSet, IncludesPlusIncludes)
{
	MathematicalSet<int> left(1);
	left.Add(3);

	MathematicalSet<int> right(2);
	right.Add(3);

	MathematicalSet<int> result(left + right);

	EXPECT_TRUE(result.Contains(1));
	EXPECT_TRUE(result.Contains(2));
	EXPECT_TRUE(result.Contains(3));
	EXPECT_FALSE(result.Contains(5));
}


TEST(MathematicalSet, ExcludesPlusExcludes)
{
	MathematicalSet<int> left(1);
	left.Add(3);
	left = ~left;

	MathematicalSet<int> right(2);
	right.Add(3);
	right = ~right;

	MathematicalSet<int> result(left + right);

	EXPECT_TRUE(result.Contains(1));
	EXPECT_TRUE(result.Contains(2));
	EXPECT_FALSE(result.Contains(3));
	EXPECT_TRUE(result.Contains(5));
}

TEST(MathematicalSet, IncludesPlusExcludes)
{
	MathematicalSet<int> left(1);
	left.Add(3);

	MathematicalSet<int> right(2);
	right.Add(3);
	right = ~right;

	MathematicalSet<int> result(left + right);

	EXPECT_TRUE(result.Contains(1));
	EXPECT_FALSE(result.Contains(2));
	EXPECT_TRUE(result.Contains(3));
	EXPECT_TRUE(result.Contains(5));
}

TEST(MathematicalSet, Equal)
{
	MathematicalSet<int> base(2);
	base.Add(3);

	MathematicalSet<int> same(3);
	same.Add(2);
	EXPECT_TRUE(base == same);


	MathematicalSet<int> over(same);
	over.Add(4);
	EXPECT_FALSE(base == over);



	MathematicalSet<int> differ(1);
	differ.Add(3);
	EXPECT_FALSE(base == differ);


	MathematicalSet<int> complementary(base);
	complementary = ~complementary; 
	EXPECT_FALSE(base == complementary);
}

TEST(MathematicalSet, PlusCommunitativeLow)
{
	MathematicalSet<int> left(2);
	left.Add(3);
	left = ~left;

	MathematicalSet<int> right(1);
	right.Add(3);


	MathematicalSet<int> result(left + right);

	EXPECT_TRUE((left + right) == (right + left));
}

TEST(MathematicalSet, IncludesMultIncludes)
{
	MathematicalSet<int> left(1);
	left.Add(3);

	MathematicalSet<int> right(2);
	right.Add(3);

	MathematicalSet<int> result(left * right);

	EXPECT_FALSE(result.Contains(1));
	EXPECT_FALSE(result.Contains(2));
	EXPECT_TRUE(result.Contains(3));
	EXPECT_FALSE(result.Contains(5));
}


TEST(MathematicalSet, ExcludesMultExcludes)
{
	MathematicalSet<int> left(1);
	left.Add(3);
	left = ~left;

	MathematicalSet<int> right(2);
	right.Add(3);
	right = ~right;

	MathematicalSet<int> result(left * right);

	EXPECT_FALSE(result.Contains(1));
	EXPECT_FALSE(result.Contains(2));
	EXPECT_FALSE(result.Contains(3));
	EXPECT_TRUE(result.Contains(5));
}

TEST(MathematicalSet, IncludesMultExcludes)
{
	MathematicalSet<int> left(1);
	left.Add(3);

	MathematicalSet<int> right(2);
	right.Add(3);
	right = ~right;

	MathematicalSet<int> result(left * right);

	EXPECT_TRUE(result.Contains(1));
	EXPECT_FALSE(result.Contains(2));
	EXPECT_FALSE(result.Contains(3));
	EXPECT_FALSE(result.Contains(5));
}


TEST(MathematicalSet, MultCommunitativeLow)
{
	MathematicalSet<int> left(2);
	left.Add(3);
	left = ~left;

	MathematicalSet<int> right(1);
	right.Add(3);


	MathematicalSet<int> result(left * right);

	EXPECT_TRUE((left * right) == (right * left));
}

TEST(MathmaticalSet,  Copy)
{
	MathematicalSet<int> src(2);
	MathematicalSet<int> dst(src);

	EXPECT_TRUE(src == dst);
}

TEST(MathematicalSet,  Swap)
{
	MathematicalSet<int> a(2);
	MathematicalSet<int> b(3);


	EXPECT_TRUE(a.Contains(2));
	EXPECT_FALSE(a.Contains(3));

	EXPECT_FALSE(b.Contains(2));
	EXPECT_TRUE(b.Contains(3));

	a.Swap(b);

	EXPECT_FALSE(a.Contains(2));
	EXPECT_TRUE(a.Contains(3));

	EXPECT_TRUE(b.Contains(2));
	EXPECT_FALSE(b.Contains(3));
}

TEST(MathmaticalSet,  Assign)
{
	MathematicalSet<int> src(2);
	MathematicalSet<int> dst(3);

	EXPECT_FALSE(src == dst);

	dst = src;

	EXPECT_TRUE(src == dst);
}


}
