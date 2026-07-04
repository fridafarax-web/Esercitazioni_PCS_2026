#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <cmath>
#include "netlist.hpp"
#include "component.hpp"
#include "cycles.hpp"
#include "solver.hpp"

// Mappa: nome resistore -> (tensione attesa, corrente attesa).
using ValoriAttesi = std::map<std::string, std::pair<double,double>>;

bool verifica(const Circuito& circuito, const std::vector<std::vector<int>>& cicli,
              const std::string& nome_metodo, const ValoriAttesi& atteso)
{
	RisultatoSoluzione ris = risolvi_maglie(circuito, cicli);

	bool ok = true;
	const double tol = 1e-6;
	std::cout << "[" << nome_metodo << "]\n";

	for (const auto& r : ris.resistori)
	{
		auto it = atteso.find(r.nome);

		if (it == atteso.end()) continue;

		double dv = std::abs(r.tensione - it->second.first); // scarto in tensione
		double di = std::abs(r.corrente - it->second.second); // scarto in corrente

		bool rok = (dv < tol && di < tol);  // resistore va bene se scarto < tol

		std::string esito;

		if (rok)
			esito = "  OK";
		else
			esito = "  ERRORE";

		std::cout << "  " << r.nome << ": V = " << r.tensione << " (atteso " << it->second.first << ")" << ", I = " << r.corrente << " (atteso " << it->second.second << ")" << esito << "\n";

		ok = ok && rok;
	}
	return ok;
}

// Esegue entrambi i metodi delle maglie su un circuito e confronta coi valori attesi.
bool prova_circuito(const std::string& netlist, const std::string& nome, const ValoriAttesi& atteso)
{
	std::cout << "=== " << nome << " ===\n";
	std::istringstream in(netlist);
	Circuito circuito(leggi_netlist(in));

	auto cdfs = cicli_fondamentali_dfs(circuito);
	auto cdp  = cicli_minimi_depina(circuito);

	bool ok = true;
	ok = verifica(circuito, cdfs, "DFS", atteso)    && ok;
	ok = verifica(circuito, cdp,  "DePina", atteso) && ok;
	return ok;
}

int main()
{
	bool ok = true;

	
	std::string A=
		"V1 30.0 1 4\nV2 40.0 3 5\nR1 4.0 4 2\nR2 10.0 1 2\n"
		"R3 30.0 1 3\nR4 10.0 3 2\nR5 4.0 2 5\n";
	ValoriAttesi atteso = {
		{"R1", {  8.0,  2.0}},
		{"R2", { 22.0,  2.2}},
		{"R3", { -6.0, -0.2}},
		{"R4", {-28.0, -2.8}},
		{"R5", { 12.0,  3.0}},
	};
	ok = prova_circuito(A, "A", atteso) && ok;


	std::string B =
		"R1 10 1 2\nR2 20 2 3\nR3 30 2 4\nV1 10 1 3\nV2 20 4 3\n";
	ValoriAttesi atteso2 = {
		{"R1", {  10.0/11.0,   1.0/11.0}},   // ~0.909 V, ~0.0909 A
		{"R2", { 100.0/11.0,   5.0/11.0}},   // ~9.091 V, ~0.4545 A
		{"R3", {-120.0/11.0,  -4.0/11.0}},   // ~-10.909 V, ~-0.3636 A
	};
	ok = prova_circuito(B, "B", atteso2) && ok;

	if (ok)
	{
		std::cout << "test_solver: OK\n";
		return EXIT_SUCCESS;
	}
	else
	{
		std::cout << "test_solver: FALLITO\n";
		return EXIT_FAILURE;
	}
}
