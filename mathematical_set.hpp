#pragma once

#include <list>
#include <algorithm>
#include <cassert>
namespace ism
{


template <class T>
std::list<T> getMergeUnorderList(const std::list<T>& h_, const std::list<T>& t_) 
{
	std::list<T> h(h_);
	std::list<T> t(t_);
	
	h.splice(h.end( ), t);
	h.unique( );

	return h;
}



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
		if (this->definesWith(Excludes) && adder.definesWith(Includes)) 
		{
			return adder + (*this);
		}

		if (this->definesWith(Includes) && adder.definesWith(Excludes)) {
			std::list<T> tmp;
			tmp = adder.m_elements;

			for (
					typename std::list<T>::const_iterator itr(this->m_elements.begin( ));
					itr != this->m_elements.end( );
					++itr)
			{
				tmp.remove(*itr);
			}

			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Excludes;
			return ret;
		}

		if (this->definesWith(Includes) && adder.definesWith(Includes)) {
			std::list<T> tmp(getMergeUnorderList(this->m_elements, adder.m_elements));
			
			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Includes; 
			return ret;
		}

		if (this->definesWith(Excludes) && adder.definesWith(Excludes)) {
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
		if (this->definesWith(Excludes) && mult.definesWith(Includes)) {
			return mult * (*this);
		}

		if (this->definesWith(Includes) && mult.definesWith(Excludes)) {
			std::list<T> tmp;
			tmp = this->m_elements;

			for (
					typename std::list<T>::const_iterator itr(mult.m_elements.begin( ));
					itr != mult.m_elements.end( ); 
					++itr
				)
			{
				tmp.remove(*itr);
			}


			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Includes;
			return ret;
		}

		if (this->definesWith(Excludes) && mult.definesWith(Excludes)) {
			std::list<T> tmp(getMergeUnorderList(this->m_elements, mult.m_elements));

			MathematicalSet<T> ret;
			ret.m_elements.swap(tmp);
			ret.m_type = MathematicalSet<T>::Excludes; 
			return ret;
		}

		if (this->definesWith(Includes) && mult.definesWith(Includes)) {
			std::list<T> tmp(this->m_elements);

			for (
				typename std::list<T>::const_iterator itr(this->m_elements.begin( ));
				itr != this->m_elements.end( );
				++itr
				)
			{
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
		std::list<T> tmp(this->m_elements);

		MathematicalSet<T> ret;
		ret.m_elements.swap(tmp);
		ret.m_type = flipType(this->m_type);

		return ret;
	}

private:
	enum Type
	{
		Includes, 
		Excludes, 
	};

	static Type flipType(Type t)
	{
		if (t == Includes) { return Excludes; }
		if (t == Excludes) { return Includes; }

		assert(! "unreachable sentence");
		return Includes;
	}

	std::list<T>	m_elements;
	Type			m_type;





private:
	explicit MathematicalSet(Type t)
		: m_elements( )
		, m_type(t)
	{ }


	bool definesWith(Type t) const { return t == m_type; }

	bool isInElements(const T& t) const
	{
		const typename std::list<T>::const_iterator found =
			std::find(m_elements.begin( ), m_elements.end( ), t);
		return (found != m_elements.end( ));
	}
};




		 
}
