#include <iostream>
#include <vector>
#include "lifofifo.hpp"
//es.9

bool test_lifo()
{
	lifo<int> s;
	bool ok = true;

	if (!s.empty()) { std::cout << "  [lifo] dovrebbe essere vuoto all'inizio\n"; ok = false; }

	for (int v : {1, 2, 3, 4})
		s.put(v);

	if (s.empty()) { std::cout << "  [lifo] non dovrebbe essere vuoto dopo i put\n"; ok = false; }

	// estrazione attesa: 4, 3, 2, 1 (Last In First Out)
	std::vector<int> atteso = {4, 3, 2, 1};
	std::vector<int> ottenuto;
	while (!s.empty())
		ottenuto.push_back(s.get());

	if (ottenuto != atteso)
	{
		std::cout << "  [lifo] ordine errato:";
		for (int v : ottenuto) std::cout << " " << v;
		std::cout << " (atteso 4 3 2 1)\n";
		ok = false;
	}

	if (!s.empty()) { std::cout << "  [lifo] dovrebbe essere vuoto alla fine\n"; ok = false; }
	return ok;
}

bool test_fifo()
{
	fifo<int> q;
	bool ok = true;

	if (!q.empty()) { std::cout << "  [fifo] dovrebbe essere vuoto all'inizio\n"; ok = false; }

	for (int v : {1, 2, 3, 4})
		q.put(v);

	if (q.empty()) { std::cout << "  [fifo] non dovrebbe essere vuoto dopo i put\n"; ok = false; }

	// estrazione attesa: 1, 2, 3, 4 (First In First Out)
	std::vector<int> atteso = {1, 2, 3, 4};
	std::vector<int> ottenuto;
	while (!q.empty())
		ottenuto.push_back(q.get());

	if (ottenuto != atteso)
	{
		std::cout << "  [fifo] ordine errato:";
		for (int v : ottenuto) std::cout << " " << v;
		std::cout << " (atteso 1 2 3 4)\n";
		ok = false;
	}

	if (!q.empty()) { std::cout << "  [fifo] dovrebbe essere vuoto alla fine\n"; ok = false; }
	return ok;
}

int main()
{
	bool ok = true;

	std::cout << "Test lifo (LIFO):\n";
	ok = test_lifo() && ok;

	std::cout << "Test fifo (FIFO):\n";
	ok = test_fifo() && ok;

	if (ok)
	{
		std::cout << "test_lifofifo: OK\n";
		return EXIT_SUCCESS;
	}
	std::cout << "test_lifofifo: FALLITO\n";
	return EXIT_FAILURE;
}
