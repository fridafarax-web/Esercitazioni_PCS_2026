#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include "netlist.hpp"
#include "component.hpp"
#include "cycles.hpp"

int rango_gf2(const Circuito& circuito, const std::vector<std::vector<int>>& cicli)
{
	std::map<std::pair<int,int>, int> indice_arco;
	int m = 0;
	for (const auto& c : circuito.componenti)
		indice_arco[{c.nodo_piccolo(), c.nodo_grande()}] = m++;

	// righe = vettori d'incidenza delle maglie
	std::vector<std::vector<int>> righe;
	for (const auto& ciclo : cicli)
	{
		std::vector<int> r(m, 0);
		int L = ciclo.size();
		for (int t = 0; t < L; ++t)
		{
			int a = ciclo[t], b = ciclo[(t + 1) % L];
			r[indice_arco[{std::min(a,b), std::max(a,b)}]] ^= 1;
		}
		righe.push_back(r);
	}

	// eliminazione di Gauss mod 2
	int rango = 0;
	for (int col = 0; col < m && rango < (int)righe.size(); ++col)
	{
		int piv = -1;
		for (int i = rango; i < (int)righe.size(); ++i)
			if (righe[i][col]) { piv = i; break; }
		if (piv < 0)
			continue;
		std::swap(righe[rango], righe[piv]);
		for (int i = 0; i < (int)righe.size(); ++i)
			if (i != rango && righe[i][col])
				for (int j = 0; j < m; ++j)
					righe[i][j] ^= righe[rango][j];
		++rango;
	}
	return rango;
}
bool cicli_validi(const Circuito& circuito, const std::vector<std::vector<int>>& cicli)
{
	// insieme delle coppie (piccolo,grande) presenti nel circuito
	std::set<std::pair<int,int>> archi;
	for (const auto& c : circuito.componenti)
		archi.insert({c.nodo_piccolo(), c.nodo_grande()});

	if (cicli.size() != circuito.cicli_attesi())
	{
		std::cout << "  numero cicli errato: " << cicli.size() << " (atteso " << circuito.cicli_attesi() << ")\n";
		return false;
	}

	int rango = rango_gf2(circuito, cicli);
	if (rango != circuito.cicli_attesi())
	{
		std::cout << "  maglie dipendenti: rango " << rango << " (atteso " << circuito.cicli_attesi() << ")\n";
		return false;
	}

	for (const auto& ciclo : cicli)
	{
		if (ciclo.size() < 3)
		{
			std::cout << "  ciclo troppo corto\n";
			return false; 
		}
		
		int L = ciclo.size(); // num nodi maglia 
		
		for (int t = 0; t < L; ++t)
		{
			int a = ciclo[t], b = ciclo[(t + 1) % L];
			
			if (archi.find({std::min(a,b), std::max(a,b)}) == archi.end())
			{
				std::cout << "  arco inesistente (" << a << "," << b << ")\n";
				return false;
			}
		}
	}
	return true;
}

int main()
{
	std::string testo =
		"V1 30.0 1 4\nV2 40.0 3 5\nR1 4.0 4 2\nR2 10.0 1 2\n"
		"R3 30.0 1 3\nR4 10.0 3 2\nR5 4.0 2 5\n";
	std::istringstream in(testo);
	Circuito circuito(leggi_netlist(in));

	bool ok = true;

	std::cout << "Metodo DFS + coalbero:\n";
	auto cdfs = cicli_fondamentali_dfs(circuito);
	ok = cicli_validi(circuito, cdfs) && ok;

	std::cout << "Metodo De Pina:\n";
	auto cdp = cicli_minimi_depina(circuito);
	ok = cicli_validi(circuito, cdp) && ok;

	if (ok)
	{
		std::cout << "test_cycles: OK\n";
		return EXIT_SUCCESS;
	}
	std::cout << "test_cycles: FALLITO\n";
	return EXIT_FAILURE;
}
