#pragma once
#include <queue>
#include <stack>
//Es9
template <typename T>
class lifo
{
private:
	std::stack<T> s;

public:
	lifo() = default;

	// inserisce un elemento
	void put(const T& val) { s.push(val); }

	// rimuove e restituisce l'ultimo elemento inserito
	T get()
	{
		T val = s.top();
		s.pop();
		return val;
	}

	// true se il contenitore e' vuoto
	bool empty() const { return s.empty(); }

	// numero di elementi presenti
	std::size_t size() const { return s.size(); }
};

// Coda: First In, First Out.
template <typename T>
class fifo
{
private:
	std::queue<T> q;

public:
	fifo() = default;

	// inserisce un elemento
	void put(const T& val) { q.push(val); }

	// rimuove e restituisce il primo elemento inserito
	T get()
	{
		T val = q.front();
		q.pop();
		return val;
	}

	// true se il contenitore e' vuoto
	bool empty() const { return q.empty(); }

	// numero di elementi presenti
	std::size_t size() const { return q.size(); }
};
