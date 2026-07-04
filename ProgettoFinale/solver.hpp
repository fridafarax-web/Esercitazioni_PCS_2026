#pragma once
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <Eigen/Dense>
#include "component.hpp"
#include "gradiente_coniugato.hpp"

struct RisultatoResistore
{
	std::string nome;
	double tensione;  // Volt
	double corrente;  // Ampere
};

struct RisultatoSoluzione
{
	Eigen::MatrixXd B;
	Eigen::MatrixXd R;
	Eigen::MatrixXd A; // B^T R B
	Eigen::VectorXd v; // termini noti
	Eigen::VectorXd correnti_maglia;
	std::vector<RisultatoResistore> resistori;
	bool convergenza = false;
	int iterazioni  = 0;
};

// Verso di percorrenza dell'arco a->b rispetto all'orientamento di
// riferimento (sempre dal nodo minore al maggiore).
int segno_arco(int a, int b) // importante per i segni delle matrici 
{
	if (a < b)
		return +1;
	else
		return -1;
}

// Risolve il sistema con il gradiente coniugato (es.10).
RisultatoSoluzione risolvi_maglie(const Circuito& circuito, const std::vector<std::vector<int>>& cicli)
{
	RisultatoSoluzione out;
	const int n = cicli.size(); // numero maglie

	// Mappa coppia di nodi (piccolo,grande) -> indice del componente.
	std::map<std::pair<int,int>, int> arco_a_componente;
	std::vector<int> riga_resistore(circuito.n_componenti(), -1);
	int m = 0;                                              // numero resistori
	for (int i = 0; i < circuito.n_componenti(); ++i)
	{
		const Componente& c = circuito.componenti[i];
		arco_a_componente[{c.nodo_piccolo(), c.nodo_grande()}] = i;
		if (c.tipo == 'R')
			riga_resistore[i] = m++;
	}

	Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n);
	Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m);
	Eigen::VectorXd v = Eigen::VectorXd::Zero(n);

	// Matrice diagonale delle resistenze.
	for (int i = 0; i < circuito.n_componenti(); ++i)
	{
		const Componente& c = circuito.componenti[i];
		if (c.tipo == 'R')
			R(riga_resistore[i], riga_resistore[i]) = c.valore;
	}

	// Scansione di ogni maglia.
	for (int j = 0; j < n; ++j)
	{
		const std::vector<int>& ciclo = cicli[j];
		const int L = ciclo.size();
		for (int t = 0; t < L; ++t)
		{
			int a = ciclo[t];
			int b = ciclo[(t + 1) % L];
			int piccolo = std::min(a, b), grande = std::max(a, b);

			auto it = arco_a_componente.find({piccolo, grande});
			if (it == arco_a_componente.end())
			{
				std::cerr << "ERRORE: arco di maglia senza componente associato\n";
				exit(1);
			}
			const Componente& c = circuito.componenti[it->second];
			int s = segno_arco(a, b);

			if (c.tipo == 'R')
			{
				B(riga_resistore[it->second], j) = s;
			}
			else // generatore: contributo al termine noto
			{
				// terminale "+" = primo nodo elencato (c.nodo1).
				// s_gen = +1 se "+" sul nodo maggiore, -1 se sul minore.
				int s_gen;
				if (c.nodo1 == grande)
					s_gen = +1;
				else
					s_gen = -1;

				v(j) += s * s_gen * c.valore;
			}
		}
	}

	Eigen::MatrixXd A = B.transpose() * R * B; // A = B'*R*B

	Eigen::VectorXd i_maglia; // vettore correnti di maglia 
	// gradiente coniugato (es.10)
	ris_gradiente_coniugato sol;
	sol.gc(A, v, 1.0e-12, 10000);
	i_maglia = sol.x;
	out.convergenza = sol.convergenza;
	out.iterazioni = sol.iterazioni;

	// Correnti e tensioni sui resistori.
	Eigen::VectorXd I  = B * i_maglia; // corrente per resistore
	Eigen::VectorXd VR = R * I; // tensione per resistore

	for (int i = 0; i < circuito.n_componenti(); ++i)
	{
		const Componente& c = circuito.componenti[i];
		if (c.tipo != 'R') continue; // salta i non resistori
		int riga = riga_resistore[i];
		out.resistori.push_back({c.nome, VR(riga), I(riga)});
	}

	out.B = B;
	out.R = R;
	out.A = A;
	out.v = v;
	out.correnti_maglia = i_maglia;
	return out;
}
