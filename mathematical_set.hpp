#pragma once

#include <list>
#include <algorithm>
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


	bool operator==(const MathematicalSet<T>& target) const
	{
		if (this->m_type != target.m_type) { return false; }
		if (this->m_elements.size( ) != target.m_elements.size( )) { return false; }

		const typename std::list<T>::const_iterator end(this->m_elements.end( ));
		for(
				typename std::list<T>::const_iterator itr(this->m_elements.begin( ));
				itr != end;
				++itr
			)
		{
			if (! target.isInElements(*itr)) { return false; }
		}


		return true;
	}




	MathematicalSet<T> operator+(const MathematicalSet<T>& adder) const
	{
		if (this->definesWith(MathematicalSet<T>::Excludes) && adder.definesWith(MathematicalSet<T>::Includes)) {
			return adder + (*this);
		}

		if (this->definesWith(MathematicalSet<T>::Includes) && adder.definesWith(MathematicalSet<T>::Excludes)) {
			std::list<T> tmp;
			tmp = adder.m_elements;

			for (typename std::list<T>::const_iterator itr(this->m_elements.begin( )); itr != this->m_elements.end( ); ++itr) {
				tmp.remove(*itr);
			}


			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Excludes;
			return ret;
		}

		if (this->definesWith(MathematicalSet<T>::Includes) && adder.definesWith(MathematicalSet<T>::Includes)) {
			std::list<T> new_elements(this->m_elements);
			std::list<T> tail(adder.m_elements);
			
			new_elements.splice(new_elements.end( ), tail);
			new_elements.unique( );


			MathematicalSet<T> ret;
			ret.m_elements.swap(new_elements);
			ret.m_type = MathematicalSet<T>::Includes; 
			return ret;
		}

		if (this->definesWith(MathematicalSet<T>::Excludes) && adder.definesWith(MathematicalSet<T>::Excludes)) {
			std::list<T> tmp(this->m_elements);

			for (
					typename std::list<T>::const_iterator itr(this->m_elements.begin( ));
					itr != this->m_elements.end( );
					++itr
				) {
				if (! adder.Contains(*itr)) { continue; }
				tmp.remove(*itr);
			}


			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Excludes; 
			return ret;
		}
	}






	MathematicalSet<T> operator*(const MathematicalSet<T>& mult) const
	{
		if (this->definesWith(MathematicalSet<T>::Excludes) && mult.definesWith(MathematicalSet<T>::Includes)) {
			return mult * (*this);
		}

		if (this->definesWith(MathematicalSet<T>::Includes) && mult.definesWith(MathematicalSet<T>::Excludes)) {
			std::list<T> tmp;
			tmp = this->m_elements;

			for (typename std::list<T>::const_iterator itr(mult.m_elements.begin( )); itr != mult.m_elements.end( );  ++itr) {
				tmp.remove(*itr);
			}


			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Includes;
			return ret;
		}

		if (this->definesWith(MathematicalSet<T>::Excludes) && mult.definesWith(MathematicalSet<T>::Excludes)) {
			std::list<T> tmp(this->m_elements);
			std::list<T> tail(mult.m_elements);
			tmp.splice(tmp.end( ),  tail);
			tmp.unique( );

			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Excludes; 
			return ret;
		}

		if (this->definesWith(MathematicalSet<T>::Includes) && mult.definesWith(MathematicalSet<T>::Includes)) {
			std::list<T> tmp(this->m_elements);

			for (typename std::list<T>::const_iterator itr(this->m_elements.begin( )); itr != this->m_elements.end( );  ++itr) {
				if (mult.Contains(*itr)) { continue; }
				tmp.remove(*itr);
			}


			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Includes; 
			return ret;
		}
	}


	MathematicalSet<T> operator~() const
	{
		std::list<T> new_list(this->m_elements);

		MathematicalSet<T> ret;
		ret.m_elements.swap(new_list);
		if (this->m_type == MathematicalSet<T>::Includes) {
			ret.m_type = MathematicalSet<T>::Excludes;
		}
		if (this->m_type == MathematicalSet<T>::Excludes) {
			ret.m_type = MathematicalSet<T>::Includes;
		}

		return ret;
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




};




		 
}
