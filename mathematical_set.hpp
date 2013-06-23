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
	
	MathematicalSet()
		: m_elements( )
		, m_type(Includes)
	{ }

	MathematicalSet(const T& e)
		: m_elements( )
		, m_type(Includes)
	{
		Add(e);
	}

	void Add(const T& t)
	{ 
		if (this->definesWith(Includes)) {
			if (isInElements(t)) { return; }
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
			if (isInElements(t)) { return; }

			m_elements.push_back(t);
		}
	}


	bool Contains(const T& t) const
	{ 
		return (this->definesWith(Includes))
			? (  isInElements(t))
			: (! isInElements(t));
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

	MathematicalSet(Type t)
		: m_elements( )
		, m_type(t)
	{ }

	bool isInElements(const T& t) const
	{
		const typename std::list<T>::const_iterator found =
			std::find(m_elements.begin( ), m_elements.end( ), t);
		return (found != m_elements.end( ));
	}


	template  <class Type> 
	friend MathematicalSet<Type> operator+(const MathematicalSet<Type>& lhs,  const MathematicalSet<Type>& rhs);
	template  <class Type> 
	friend MathematicalSet<Type> operator*(const MathematicalSet<Type>& lhs,  const MathematicalSet<Type>& rhs);
	template  <class Type> 
	friend MathematicalSet<Type> operator~(const MathematicalSet<Type>& set);
};

template <class T>
MathematicalSet<T> operator+(const MathematicalSet<T>& lhs,  const MathematicalSet<T>& rhs)
{
	if (lhs.definesWith(MathematicalSet<T>::Excludes) && rhs.definesWith(MathematicalSet<T>::Includes)) {
		return rhs + lhs;
	}

	if (lhs.definesWith(MathematicalSet<T>::Includes) && rhs.definesWith(MathematicalSet<T>::Excludes)) {
		std::list<T> tmp;
		tmp = rhs.m_elements;

		for (typename std::list<T>::const_iterator itr(lhs.m_elements.begin( )); itr != lhs.m_elements.end( ); ++itr) {
			tmp.remove(*itr);
		}


		MathematicalSet<T> ret;
		ret.m_elements.swap(tmp);
		ret.m_type = MathematicalSet<T>::Excludes;
		return ret;
	}

	if (lhs.definesWith(MathematicalSet<T>::Includes) && rhs.definesWith(MathematicalSet<T>::Includes)) {
		std::list<T> tmp;
		tmp = lhs;
		tmp.merge(rhs.m_elements);
		tmp.unique( );


		MathematicalSet<T> ret;
		ret.m_elements.swap(tmp);
		ret.m_type = MathematicalSet<T>::Includes; 
		return ret;
	}

	if (lhs.definesWith(MathematicalSet<T>::Excludes) && rhs.definesWith(MathematicalSet<T>::Excludes)) {
		std::list<T> tmp;
		tmp = lhs;

		for (typename std::list<T>::const_iterator itr(lhs.m_elements.begin( )); itr != lhs.m_elements.end( );  ++itr) {
			if (! rhs.Contains(*itr)) { continue; }
			tmp.remove(*itr);
		}


		MathematicalSet<T> ret;
		ret.m_elements.swap(tmp);
		ret.m_type = MathematicalSet<T>::Excludes; 
		return ret;
	}
}

template <class T>
MathematicalSet<T> operator*(const MathematicalSet<T>& lhs,  const MathematicalSet<T>& rhs)
{
	if (lhs.definesWith(MathematicalSet<T>::Excludes) && rhs.definesWith(MathematicalSet<T>::Includes)) {
		return rhs + lhs;
	}

	if (lhs.definesWith(MathematicalSet<T>::Includes) && rhs.definesWith(MathematicalSet<T>::Excludes)) {
		std::list<T> tmp;
		tmp = lhs.m_elements;

		for (typename std::list<T>::const_iterator itr(rhs.m_elements.begin( )); itr != rhs.m_elements.end( );  ++itr) {
			tmp.remove(*itr);
		}


		MathematicalSet<T> ret;
		ret.m_elements.swap(tmp);
		ret.m_type = MathematicalSet<T>::Includes;
		return ret;
	}

	if (lhs.definesWith(MathematicalSet<T>::Excludes) && rhs.definesWith(MathematicalSet<T>::Excludes)) {
		std::list<T> tmp;
		tmp.merge(rhs.m_elements);
		tmp.unique( );

		MathematicalSet<T> ret;
		ret.m_elements.swap(tmp);
		ret.m_type = MathematicalSet<T>::Excludes; 
		return ret;
	}

	if (lhs.definesWith(MathematicalSet<T>::Includes) && rhs.definesWith(MathematicalSet<T>::Includes)) {
		std::list<T> tmp;
		tmp = lhs;

		for (typename std::list<T>::const_iterator itr(lhs.m_elements.begin( )); itr != lhs.m_elements.end( );  ++itr) {
			if (! rhs.Contains(*itr)) { continue; }
			tmp.remove(*itr);
		}


		MathematicalSet<T> ret;
		ret.m_elements.swap(tmp);
		ret.m_type = MathematicalSet<T>::Includes; 
		return ret;
	}
}


template <class T>
MathematicalSet<T> operator~(const MathematicalSet<T>& set)
{
	MathematicalSet<T> ret;
	ret.m_elements = set.m_elemets;
	if (set.m_type == MathematicalSet<T>::Includes) {
		set.m_type = MathematicalSet<T>::Excludes;
	}
	if (set.m_type == MathematicalSet<T>::Excludes) {
		set.m_type = MathematicalSet<T>::Includes;
	}

	return ret;
}

		 
}
