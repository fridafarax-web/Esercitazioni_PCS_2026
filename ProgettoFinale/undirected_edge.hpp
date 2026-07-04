#include <iostream>
#include <numeric>
#include <concepts>
#pragma once
//Es 8/9/10

template <typename T>
class undirected_edge
{
	private:
	T from_;
	T to_;
	double peso_;
	
	public:
	undirected_edge() : from_(T{}), to_(T{}), peso_(0.0) {}

	undirected_edge(const T& nodoA, const T& nodoB, double w = 1.0)
		: from_(std::min(nodoA, nodoB)), to_(std::max(nodoA, nodoB)), peso_(w)
	{}

	T from() const { return from_; }
	T to() const { return to_; }
	T get_from() const { return from_; }
	T get_to() const { return to_; }
	double peso() const { return peso_; }
	double get_peso() const { return peso_; }

	// Operatore<
	bool operator<(const undirected_edge<T>& other) const
	{
		if (from_ != other.from_)
		{
			return from_ < other.from_;
		}
		else
		{
			return to_ < other.to_;
		}
	}
	
	// Operatore==
	bool operator==(const undirected_edge<T>& other) const
	{
		if (from_ == other.from_ && to_ == other.to_)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	bool operator!=(const undirected_edge<T>& other) const
	{
		return !(*this == other);
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const undirected_edge<T>& a)
{
	os << "(" << a.from() << " - " << a.to() << ")";
	return os;
}

