#pragma once
#include <string>
#include <vector>
#include <set>
#include <algorithm>

struct Componente // descrivo i dati come la netlist
{
	std::string nome; // es. "R1", "V2"
	char tipo; // 'R' = resistore, 'V' = generatore
	double valore; // Ohm (resistore) oppure Volt (generatore)
	int nodo1; // primo nodo elencato  (per V: terminale +)
	int nodo2; // secondo nodo elencato (per V: terminale -)

	int nodo_piccolo() const { return std::min(nodo1, nodo2); }
	int nodo_grande()  const { return std::max(nodo1, nodo2); }
};

class Circuito
{
public:
	std::vector<Componente> componenti; // vettore di tutti i componenti del circuito

	Circuito(std::vector<Componente> comp)
	{
		componenti = comp;          // copia normale

		std::set<int> ns; // insieme di nodi
		for (const auto& c : componenti) // scorre i componenti e inserisce i nodi, no duplicati
		{
			ns.insert(c.nodo1);
			ns.insert(c.nodo2);
		}
		nodi.assign(ns.begin(), ns.end()); // ordinati e univoci
	}

	const std::vector<int>& lista_nodi() const { return nodi; }
	int n_componenti() const { return componenti.size(); }

	// Numero di cicli fondamentali atteso: |E| - |V| + 1 (grafo connesso).
	int cicli_attesi() const
	{
		return n_componenti() - nodi.size() + 1;
	}

private:
	std::vector<int> nodi;
};
