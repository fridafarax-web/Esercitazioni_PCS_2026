#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <Eigen/Dense>
#include "netlist.hpp"
#include "component.hpp"
#include "cycles.hpp"
#include "solver.hpp"

bool quasi_uguale(double a, double b, double tol = 1e-9)
{
	return std::abs(a - b) < tol;
}

// Verifica le proprieta' di B e R per un dato insieme di maglie.
bool verifica_BR(const Circuito& circuito, const std::vector<std::vector<int>>& cicli,
                 const std::string& metodo, const std::vector<double>& resistenze_attese)
{
	RisultatoSoluzione ris = risolvi_maglie(circuito, cicli);
	const Eigen::MatrixXd& B = ris.B;
	const Eigen::MatrixXd& R = ris.R;

	const int m = static_cast<int>(resistenze_attese.size()); // numero resistori
	const int n = static_cast<int>(cicli.size()); // numero maglie

	bool ok = true;
	std::cout << "[" << metodo << "]\n";

	// --- dimensioni ---
	if (B.rows() != m || B.cols() != n)
	{
		std::cout << "  B ha dimensioni " << B.rows() << "x" << B.cols()
		          << " (attese " << m << "x" << n << ")\n";
		ok = false;
	}
	if (R.rows() != m || R.cols() != m)
	{
		std::cout << "  R ha dimensioni " << R.rows() << "x" << R.cols()
		          << " (attese " << m << "x" << m << ")\n";
		ok = false;
	}

	for (int i = 0; i < R.rows(); ++i)
		for (int j = 0; j < R.cols(); ++j)
		{
			if (i == j)
			{
				if (!quasi_uguale(R(i, j), resistenze_attese[i]))
				{
					std::cout << "  R(" << i << "," << i << ") = " << R(i, j)
					          << " (atteso " << resistenze_attese[i] << ")\n";
					ok = false;
				}
			}
			else if (!quasi_uguale(R(i, j), 0.0))
			{
				std::cout << "  R fuori diagonale non nullo in (" << i << "," << j << ")\n";
				ok = false;
			}
		}


	for (int i = 0; i < B.rows(); ++i)
	{
		int non_nulli = 0;
		for (int j = 0; j < B.cols(); ++j)
		{
			double v = B(i, j);
			if (!quasi_uguale(v, -1.0) && !quasi_uguale(v, 0.0) && !quasi_uguale(v, 1.0))
			{
				std::cout << "  B(" << i << "," << j << ") = " << v << " non in {-1,0,1}\n";
				ok = false;
			}
			if (!quasi_uguale(v, 0.0))
				++non_nulli;
		}
		if (non_nulli == 0)
		{
			std::cout << "  resistore " << i << " non appartiene ad alcuna maglia (riga B nulla)\n";
			ok = false;
		}
	}


	Eigen::FullPivLU<Eigen::MatrixXd> lu(B);
	if (lu.rank() != n)
	{
		std::cout << "  rango di B = " << lu.rank() << " (atteso " << n << ", maglie dipendenti)\n";
		ok = false;
	}


	Eigen::MatrixXd A = ris.A;
	if (!A.isApprox(A.transpose(), 1e-9))
	{
		std::cout << "  A = B^T R B non simmetrica\n";
		ok = false;
	}
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(A);
	if (es.eigenvalues().minCoeff() <= 1e-9)
	{
		std::cout << "  A non definita positiva (autovalore minimo "
		          << es.eigenvalues().minCoeff() << ")\n";
		ok = false;
	}

	if (ok)
		std::cout << "  OK (B " << B.rows() << "x" << B.cols() << ", R " << R.rows() << "x" << R.cols() << ")\n";
	return ok;
}

// Esegue le verifiche con entrambi i metodi di ricerca delle maglie.
bool prova_circuito(const std::string& netlist, const std::string& nome,
                    const std::vector<double>& resistenze_attese)
{
	std::cout << "=== " << nome << " ===\n";
	std::istringstream in(netlist);
	Circuito circuito(leggi_netlist(in));

	bool ok = true;
	ok = verifica_BR(circuito, cicli_fondamentali_dfs(circuito), "DFS",    resistenze_attese) && ok;
	ok = verifica_BR(circuito, cicli_minimi_depina(circuito),    "DePina", resistenze_attese) && ok;
	return ok;
}

int main()
{
	bool ok = true;

	std::string A =
		"V1 30.0 1 4\nV2 40.0 3 5\nR1 4.0 4 2\nR2 10.0 1 2\n"
		"R3 30.0 1 3\nR4 10.0 3 2\nR5 4.0 2 5\n";
	ok = prova_circuito(A, "A", {4.0, 10.0, 30.0, 10.0, 4.0}) && ok;


	std::string B =
		"R1 10 1 2\nR2 20 2 3\nR3 30 2 4\nV1 10 1 3\nV2 20 4 3\n";
	ok = prova_circuito(B, "B", {10.0, 20.0, 30.0}) && ok;

	if (ok)
	{
		std::cout << "test_matrici: OK\n";
		return EXIT_SUCCESS;
	}
	std::cout << "test_matrici: FALLITO\n";
	return EXIT_FAILURE;
}
