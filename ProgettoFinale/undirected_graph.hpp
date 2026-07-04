#include <iostream>
#include <fstream>
#include <numeric>
#include <limits>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <list>
#include <concepts>
#include "undirected_edge.hpp"
#pragma once
//Es 8/9/10
template <typename T>
class undirected_graph
{
	private:
	std::map<int, undirected_edge<T>> i_arco;          // indice -> arco
	std::map<undirected_edge<T>, int> arco_i;          // arco   -> indice
	std::map<T, std::set<undirected_edge<T>>> lista_adiacenza; // ad ogni nodo associati un set di archi adiacenti
	int id = 0; // contatore indici

	public:
	undirected_graph() = default;
	undirected_graph(const undirected_graph& other) = default;

	void add_edge(const T& u, const T& v, double w = 1.0)
	{
		undirected_edge<T> a(u, v, w);

		// Evita di inserire due volte lo stesso arco.
		if (lista_adiacenza.count(u) > 0 && lista_adiacenza[u].find(a) != lista_adiacenza[u].end())
		{
			return;
			}
		else
		{
			lista_adiacenza[u].insert(a);
			lista_adiacenza[v].insert(a);

			i_arco[id] = a;
			arco_i[a] = id;
			id++;
		}
	}

	// rm_edge -> rimuove un arco (speculare ad add_edge): lo toglie dalla lista
	// di adiacenza dei due nodi e dalle mappe indice<->arco.
	void rm_edge(const undirected_edge<T>& a)
	{
		const T& u = a.get_from();
		const T& v = a.get_to();

		if (lista_adiacenza.count(u) > 0) lista_adiacenza[u].erase(a);
		if (lista_adiacenza.count(v) > 0) lista_adiacenza[v].erase(a);

		auto it = arco_i.find(a);
		if (it != arco_i.end())
		{
			i_arco.erase(it->second);
			arco_i.erase(it);
		}
	}

	// Comodita': rimuove l'arco fra i due nodi u e v.
	void rm_edge(const T& u, const T& v) { rm_edge(undirected_edge<T>(u, v)); }

	// neighbours -> trova nodi vicini
	std::set<T> neighbours(const T& nodo) const
	{
		std::set<T> vicini;

		if (lista_adiacenza.count(nodo) > 0)
		{
			for (const auto& arco : lista_adiacenza.at(nodo))
			{
				if (arco.get_from() == nodo)
				{
					vicini.insert(arco.get_to()); // per capire come prendere solo il nodo presente nell'arco che non fosse il nodo scelto ho chiesto a Gemini che mi ha suggerito di implementare i metodi get_from e get_to nel undirected_edge
				}
				else
				{
					vicini.insert(arco.get_from());
				}
			}
		}
		return vicini;
	}

	// all_edges -> tutti gli archi
	std::set<undirected_edge<T>> all_edges() const
	{
		std::set<undirected_edge<T>> ris;

		for (const auto& pair : i_arco)
		{
			ris.insert(pair.second); // funzionamento scorrimento mappa, pair e (first/second) chiesto a Gemini
			}
		return ris;
	}

	// all_nodes -> tutti i nodi
	std::set<T> all_nodes() const
	{
		std::set<T> ris;
		for (const auto& pair : lista_adiacenza)
		{
			ris.insert(pair.first);
			}
		return ris;
	}

	// numero nodi e numero archi
	int n_nodes() const { return static_cast<int>(lista_adiacenza.size()); } // static_cast convert l valore in tipo int
	int n_edges() const { return static_cast<int>(i_arco.size()); }

	// edge_number -> numero dell'arco
	int edge_number(const undirected_edge<T>& a) const
	{
		if (arco_i.count(a)>0)
		{
			return arco_i.at(a);
		}
		else
		{
			return -1;
		}
	}

	// edge_at -> dato il numero dell'arco restituisce l'arco
	undirected_edge<T> edge_at(int n) const
	{
		if (i_arco.count(n)>0)
		{
			return i_arco.at(n);
		}
		else
		{
			std::cerr << "Errore: indice non trovato" << "\n";
			return undirected_edge<T>();
		}
	}

	// get_peso -> utilizzato Gemini, utilizzato per l'algoritmo dijkstra
	// devo chiedere al grafo: "Esiste un collegamento tra U e V? E se sì, quanto pesa?"

	double get_peso(const T& u, const T& v) const
	{
		undirected_edge<T> a(u,v);

		if (lista_adiacenza.count(u)>0)
		{
			auto i_a = lista_adiacenza.at(u).find(a);

			if (i_a != lista_adiacenza.at(u).end())
			{
				return i_a -> get_peso(); // i_a iteratore
			}
		}
		return std::numeric_limits<double>::infinity();
	}

	// esiste un arco tra u e v
	bool has_edge(const T& u, const T& v) const
	{
		undirected_edge<T> a(u, v);
		if (lista_adiacenza.count(u) > 0 && lista_adiacenza.at(u).find(a) != lista_adiacenza.at(u).end())
		{
			return true;
		}
		return false;
	}

	void to_dot(std::ostream& os) const
	{
		os << "graph G {\n";
		for (const auto& arco : all_edges())
			os << "  \"" << arco.get_from() << "\" -- \"" << arco.get_to() << "\";\n";
		os << "}\n";
	}
};
