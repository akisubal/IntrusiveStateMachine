#pragma once

#include <list>
#include <cassert>
namespace ism
{


template <class T>
class MathematicalSet
{
public:
	static MathematicalSet<T> EmptySet( )
	{
		return MathematicalSet(Includes);
	}
	static MathematicalSet<T> UniversalSet( )
	{
		return MathematicalSet(Excludes);
	}
	

	MathematicalSet(const T& t)
		: m_elements( )
	{
		Add(t);
	}

	void Add(const T& t)
	{ 
		if (this->definesWith(Includes)) {
			m_elements.push_back(t);
		}
		else if (this->definesWith(Excludes)) {
			m_elements.remove(t);
		}
	}

	void Remove(const T& t)
	{ 
		if (this->definesWith(Includes)) {
			m_elements.remove(t);
		}
		else if (this->definesWith(Excludes)) {
			m_elements.push_back(t);
		}
	}


	bool Contains(const T& t)
	{ 
		const typename std::list<T>::iterator found =
			std::find(m_elements.begin( ),  m_elements.end( ), t);

		return 
			(this->definesWith(Includes))
				? (found != m_elements.end( ))
				: (found == m_elements.end( ));
	}

private:
	enum Type
	{
		Includes, 
		Excludes, 
	};

	std::list<T>	m_elements;
	Type			m_type;

	bool definesWith(Type t) const 
	{
		return t == m_type;
	}

	friend MathematicalSet<T> operator+(const MathematicalSet<T>& lhs,  const MathematicalSet<T>& rhs);
	friend MathematicalSet<T> operator*(const MathematicalSet<T>& lhs,  const MathematicalSet<T>& rhs);
	friend MathematicalSet<T> operator~(const MathematicalSet<T>& set);
};

template <class T>
MathematicalSet<T> operator+(const MathematicalSet<T>& lhs,  const MathematicalSet<T>& rhs)
{
	if (lhs.definesWith(MathematicalSet<T>::Excludes) && rhs.definesWith(MathematicalSet<T>::Includes)) {
		return rhs + lhs;
	}
}

template <class T>
MathematicalSet<T> operator*(const MathematicalSet<T>& lhs,  const MathematicalSet<T>& rhs)
{
	if (lhs.definesWith(MathematicalSet<T>::Excludes) && rhs.definesWith(MathematicalSet<T>::Includes)) {
		return rhs + lhs;
	}
	/// Swap
}

template <class T>
MathematicalSet<T> operator~(const MathematicalSet<T>& set)
{
}

		 
}
