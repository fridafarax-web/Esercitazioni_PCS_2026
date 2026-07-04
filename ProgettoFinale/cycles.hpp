#pragma once
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <Eigen/Dense>
#include "component.hpp"
#include "undirected_graph.hpp"
#include "lifofifo.hpp"

// Il circuito e' modellato come grafo non orientato di interi.
using Grafo = undirected_graph<int>;

// Costruisce il grafo del circuito: un arco per ogni componente, memorizzato
// (per convenzione) dal nodo di indice minore a quello maggiore.
Grafo costruisci_grafo(const Circuito& circuito)
{
	Grafo G;
	for (const auto& c : circuito.componenti)
		G.add_edge(c.nodo_piccolo(), c.nodo_grande()); 
	return G;
}

// Visita iterativa del grafo usando un contenitore generico (lifo o fifo, es.9):
// estrae un nodo dalla frontiera C e ne accoda i vicini non ancora visitati,
// aggiungendo a T l'arco di scoperta. Con un lifo si ottiene un albero DFS,
// con un fifo un albero BFS; in entrambi i casi T e' un albero di supporto.
template <typename Contenitore>
void visita_albero(const Grafo& G, int sorgente, std::set<int>& visitati, Grafo& T)
{
	Contenitore C;
	C.put(sorgente);
	visitati.insert(sorgente);

	while (!C.empty())
	{
		int u = C.get();
		for (int w : G.neighbours(u))
		{
			if (visitati.find(w) == visitati.end()) // vicino non ancora visitato
			{
				visitati.insert(w);
				T.add_edge(u, w); // arco di scoperta -> albero
				C.put(w);
			}
		}
	}
}

// Albero di supporto T (via visita con lifo = DFS) e coalbero C = G \ T
// (archi non appartenenti all'albero).
void albero_e_coalbero(const Grafo& G, Grafo& T, Grafo& coalbero)
{
	std::set<int> visitati;
	for (int s : G.all_nodes()) // ciclo: copre anche grafi non connessi
		if (visitati.find(s) == visitati.end())
			visita_albero<lifo<int>>(G, s, visitati, T);

	for (const auto& e : G.all_edges()) // scorre tutti gli archi del grafo
		if (!T.has_edge(e.from(), e.to())) // se arco non nell'albero => nel coalbero
			coalbero.add_edge(e.from(), e.to());
}

// findpath: cerca ricorsivamente il percorso u -> v dentro
// l'albero T. Se lo trova, 'percorso' contiene i nodi da u a v e ritorna true.
bool findpath(const Grafo& T, int u, int v, std::set<int>& visitati, std::vector<int>& percorso)
{
	visitati.insert(u);
	percorso.push_back(u);

	if (u == v)
		return true;

	for (int n : T.neighbours(u))
	{
		if (visitati.find(n) == visitati.end())
		{
			if (findpath(T, n, v, visitati, percorso)) // se da n si arriva a v
				return true;
		}
	}

	percorso.pop_back(); // nessun tragitto da u porta a v > toglo u dal percorso 
	return false;
}

// BFS su grafo non orientato: cammino minimo (in numero di archi) da sorg a
// dest. Ritorna la sequenza di nodi (vuota se dest non e' raggiungibile).
std::vector<int> bfs_cammino(const Grafo& G, int sorg, int dest)
{
	fifo<int> coda; // contenitore FIFO (es.9): visita in ampiezza
	std::set<int> raggiunti;
	std::map<int, int> padri; // nodo -> nodo da cui si arriva

	raggiunti.insert(sorg);
	coda.put(sorg);
	padri[sorg] = -1; // la sorgente non ha padre

	bool trovato = false;
	while (!coda.empty())
	{
		int nodo = coda.get();
		if (nodo == dest) // appena raggiungo dest mi fermo
		{
			trovato = true;
			break;
		}
		for (int vicino : G.neighbours(nodo))
		{
			if (raggiunti.find(vicino) == raggiunti.end())
			{
				raggiunti.insert(vicino);
				padri[vicino] = nodo;
				coda.put(vicino);
			}
		}
	}

	std::vector<int> cammino;
	if (!trovato)
		return cammino; // nessun percorso

	for (int cur = dest; cur != -1; cur = padri[cur])
		cammino.push_back(cur); // ricostruzione a ritroso
	return cammino;
}

// ------------------------------------------------------
//  Metodo 1: albero DFS + coalbero (con findpath)
// -----------------------------------------------------

std::vector<std::vector<int>> cicli_fondamentali_dfs(const Circuito& circuito)
{
	Grafo G = costruisci_grafo(circuito);
	Grafo T, coalbero;
	albero_e_coalbero(G, T, coalbero);

	// ogni arco del coalbero chiude un ciclo: il percorso in T tra i suoi
	// estremi, piu' l'arco stesso (implicito), forma la maglia.
	std::vector<std::vector<int>> cicli;
	
	for (const auto& arco : coalbero.all_edges())
	{
		std::set<int> visitati;
		std::vector<int> percorso;
		
		if (findpath(T, arco.from(), arco.to(), visitati, percorso))
		{
			cicli.push_back(percorso);
			}
	}
	return cicli;
}

// ---------------------------------------------------------------
//  Metodo 2: base di cicli minimi (De Pina, grafo a due strati)
// ----------------------------------------------------------------
std::vector<std::vector<int>> cicli_minimi_depina(const Circuito& circuito)
{
	Grafo G = costruisci_grafo(circuito);
	Grafo T, coalbero;
	albero_e_coalbero(G, T, coalbero);

	int m = G.n_edges(); // numero archi
	int k = coalbero.n_edges(); // numero maglie = |E| - |V| + 1
	
	int n = *G.all_nodes().rbegin() + 1; // prende il valorre dell'ultimo nodo +1,
	// se i nodi sono {1, 2, 3, 5}, allora *...rbegin() è 5 e n diventa 6.
	
	// grafo "a due strati": copia di G piu' lo strato duplicato (u+n, v+n).
	Grafo Gsupporto(G);
	
	for (const auto& e : G.all_edges())
		Gsupporto.add_edge(e.from() + n, e.to() + n); // aggiungo il secondo strato

	// testimoni S: m righe (archi di G), k colonne (maglie). La colonna j parte
	// con un 1 sulla posizione dell'arco j-esimo del coalbero.
	Eigen::MatrixXi S = Eigen::MatrixXi::Zero(m, k);
	int col = 0;
	for (const auto& e : coalbero.all_edges())
	{
		S(G.edge_number(e), col) = 1;
		++col;
	}

	std::vector<std::vector<int>> cicli;
	for (int j = 0; j < k; ++j)
	{
		// Gprimo: per ogni arco "attivo" nella colonna S(:,j) si scambiano gli
		// archi diretti con quelli incrociati, cosi' attraversarlo cambia strato.
		Grafo Gprimo(Gsupporto);
		for (int i = 0; i < m; ++i)
		{
			if (S(i, j) == 1)
			{
				undirected_edge<int> e = G.edge_at(i);
				int u = e.from(), v = e.to();
				Gprimo.rm_edge(u, v);
				Gprimo.rm_edge(u + n, v + n);
				Gprimo.add_edge(u, v + n);
				Gprimo.add_edge(u + n, v);
			}
		}

		// ciclo minimo dispari: BFS da a (strato +) ad a+n (strato -), per ogni
		// nodo a del grafo originale; si tiene il cammino piu' corto trovato.
		std::vector<int> migliore;
		for (int a : G.all_nodes())
		{
			std::vector<int> cammino = bfs_cammino(Gprimo, a, a + n);
			if (!cammino.empty() && (migliore.empty() || cammino.size() < migliore.size()))
				migliore = cammino;
		}
		if (migliore.empty())
			continue;                         // grafo disconnesso: nessun ciclo

		// dal cammino (nodi sollevati) ricavo la maglia come sequenza di nodi
		// reali e il vettore d'incidenza Cj sugli archi di G.
		Eigen::VectorXi Cj = Eigen::VectorXi::Zero(m);
		std::vector<int> maglia;
		int L = migliore.size();
		for (int i = 0; i < L - 1; ++i)
		{
			int u = migliore[i];     if (u >= n) u -= n;   // riporto al nodo reale
			int v = migliore[i + 1]; if (v >= n) v -= n;
			Cj(G.edge_number(undirected_edge<int>(u, v))) = 1;
			maglia.push_back(u);     // l'ultimo nodo (duplicato del primo) si omette
		}
		cicli.push_back(maglia);

		// mantengo i testimoni indipendenti: se <Cj, S(:,h)> e' dispari (prodotto
		// scalare mod 2), correggo con lo XOR S(:,h) ^= S(:,j).
		for (int h = j + 1; h < k; ++h)
		{
			if (Cj.dot(S.col(h)) % 2 == 1)
				for (int i = 0; i < m; ++i)
					S(i, h) ^= S(i, j);
		}
	}
	return cicli;
}
