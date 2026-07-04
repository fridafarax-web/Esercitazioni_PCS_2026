#include <iostream>
#include <string>
#include <fstream>
#include <Eigen/Dense>
#include "netlist.hpp"
#include "component.hpp"
#include "cycles.hpp"
#include "solver.hpp"
#include "timecounter.h"

void stampa_cicli(const std::vector<std::vector<int>>& cicli)
{
	std::cout << "Maglie individuate: " << cicli.size() << "\n";
	for (std::size_t j = 0; j < cicli.size(); ++j)
	{
		std::cout << "  C" << (j + 1) << " = ( ";
		for (std::size_t t = 0; t < cicli[j].size(); ++t)
			std::cout << cicli[j][t] << " ";
		std::cout << ")\n";
	}
}

// Stampa la soluzione completa (maglie, matrici B/R/v, condizionamento,
// tensioni e correnti) per un dato insieme di maglie.
void stampa_soluzione(const Circuito& circuito, const std::vector<std::vector<int>>& cicli,
                      const std::string& nome_metodo)
{
	std::cout << "------------------------------------------------------------\n";
	std::cout << " SOLUZIONE - Metodo maglie: " << nome_metodo << "\n";
	std::cout << "------------------------------------------------------------\n";

	stampa_cicli(cicli);

	RisultatoSoluzione ris = risolvi_maglie(circuito, cicli); // risolve B'*R*B*i=v col gradiente coniugato

	// Matrici del sistema: B (incidenza resistori x maglie), R (resistenze,
	// diagonale) e v (termine noto, contributi dei generatori).
	std::cout << "\nMatrice di incidenza B (resistori x maglie):\n" << ris.B << "\n";
	std::cout << "\nMatrice delle resistenze R:\n" << ris.R << "\n";
	std::cout << "\nVettore dei generatori v (termine noto):\n" << ris.v << "\n";

	// Numero di condizionamento del sistema A = B^T R B. A e' simmetrica e
	// definita positiva, quindi cond_2(A) = lambda_max / lambda_min.
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> autovalori(ris.A);
	double lambda_min = autovalori.eigenvalues().minCoeff();
	double lambda_max = autovalori.eigenvalues().maxCoeff();
	std::cout << "\nNumero di condizionamento del sistema (A = B^T R B): "
	          << lambda_max / lambda_min << "\n";

	std::cout << "\nTensioni e correnti sui resistori:\n";
	for (const auto& r : ris.resistori)
		std::cout << "  " << r.nome << ": V = " << r.tensione << " volt, I = " << r.corrente << " amp\n";

	std::cout << "\n";
}

int main(int argc, char** argv)
{
	std::string percorso;

	for (int k = 1; k < argc; ++k)
	{
		std::string arg = argv[k];
		if (!arg.empty() && arg[0] != '-') // se non inizia con '-' => fa parte del percorso
			percorso = arg;
	}

	std::vector<Componente> componenti;
	if (!percorso.empty()) // se c'è un percorso. => c'è un file
		componenti = leggi_netlist_da_file(percorso);
	else
	{
		std::cerr << "Lettura della netlist da standard input, per completare input premi Ctrl+D\n";
		componenti = leggi_netlist(std::cin);
	}

	if (componenti.empty())
	{
		std::cerr << "Errore: nessun componente valido nella netlist.\n";
		return EXIT_FAILURE;
	}

	Circuito circuito(componenti);

	// Cronometro entrambi i metodi di ricerca delle maglie (timecounter, es.4/5).
	timecounter cron;

	cron.tic();
	std::vector<std::vector<int>> cicli_dfs = cicli_fondamentali_dfs(circuito);
	double tempo_dfs = cron.toc();

	cron.tic();
	std::vector<std::vector<int>> cicli_dp = cicli_minimi_depina(circuito);
	double tempo_dp = cron.toc();

	std::cout << "Tempi di esecuzione ricerca maglie:\n";
	std::cout << "  DFS + coalbero       : " << tempo_dfs << " s\n";
	std::cout << "  De Pina (cicli min.) : " << tempo_dp  << " s\n\n";

	// Si stampano sempre entrambe le soluzioni: la fisica (tensioni e correnti
	// sui resistori) deve coincidere, mentre maglie, B, v e condizionamento
	// possono differire a seconda della base di maglie scelta.
	stampa_soluzione(circuito, cicli_dfs, "DFS + coalbero");
	stampa_soluzione(circuito, cicli_dp,  "De Pina (cicli minimi)");

	return EXIT_SUCCESS;
}
