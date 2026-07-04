#include <iostream>
#include <string>
#include <cstdlib>
#include <Eigen/Dense>
#include "gradiente_coniugato.hpp"
//esercitazione 10

bool risolve_bene(const Eigen::MatrixXd& A, const Eigen::VectorXd& x_vero, const std::string& nome)
{
	Eigen::VectorXd b = A * x_vero;

	ris_gradiente_coniugato sol;
	sol.gc(A, b, 1e-12, 1000);

	double errore = (sol.x - x_vero).norm();
	bool ok = sol.convergenza && errore < 1e-6;

	std::cout << "  [" << nome << "] iterazioni = " << sol.iterazioni
	          << ", residuo = " << sol.residuo
	          << ", errore = " << errore
	          << (ok ? "  OK" : "  ERRORE") << "\n";
	return ok;
}

int main()
{
	bool ok = true;

	// Caso 1: 2x2 simmetrica definita positiva.
	{
		Eigen::MatrixXd A(2, 2);
		A(0,0) = 4; A(0,1) = 1;
		A(1,0) = 1; A(1,1) = 3;
		Eigen::VectorXd x(2);
		x(0) = 1.0; x(1) = 2.0;
		ok = risolve_bene(A, x, "2x2") && ok;
	}

	// Caso 2: 3x3 costruita come A = M^T M + I (sicuramente SPD).
	{
		Eigen::MatrixXd M(3, 3);
		M(0,0) = 2; M(0,1) = 0; M(0,2) = 1;
		M(1,0) = 1; M(1,1) = 3; M(1,2) = 0;
		M(2,0) = 0; M(2,1) = 1; M(2,2) = 2;
		Eigen::MatrixXd A = M.transpose() * M + Eigen::MatrixXd::Identity(3, 3);
		Eigen::VectorXd x(3);
		x(0) = -1.0; x(1) = 2.0; x(2) = 0.5;
		ok = risolve_bene(A, x, "3x3") && ok;
	}

	// Caso 3: matrice diagonale (caso limite, convergenza rapida).
	{
		Eigen::MatrixXd A = Eigen::MatrixXd::Zero(4, 4);
		A(0,0) = 10; A(1,1) = 5; A(2,2) = 2; A(3,3) = 1;
		Eigen::VectorXd x(4);
		x(0) = 1; x(1) = -2; x(2) = 3; x(3) = -4;
		ok = risolve_bene(A, x, "diagonale") && ok;
	}

	if (ok)
	{
		std::cout << "test_gradiente: OK\n";
		return EXIT_SUCCESS;
	}
	std::cout << "test_gradiente: FALLITO\n";
	return EXIT_FAILURE;
}
