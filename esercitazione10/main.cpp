#include "GC.hpp" 
#include <iostream>

using namespace std;

//Test matrice 3x3 simmetrica definita positiva
bool test_spd_3x3() {
    Eigen::MatrixXd A(3, 3);
    A << 4, 1, 0,
         1, 3, 1,
         0, 1, 2;
    
    Eigen::VectorXd b(3);
    b << 1, 2, 3;
    
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(3);
    Eigen::VectorXd x = gcd(A, b, x0, 1e-7);
    
    if ((b - A * x).norm() < 1e-5) {
        cout << "Test 3x3 SPD: OK\n" << endl;
        return true;
    } else {
        cout << "Test 3x3 SPD: FALLITO!\n" << endl;
        return false;
    }
}

//Test matrice 10x10 tridiagonale (SVD, Sparsa, ben condizionata)
bool test_tridiagonale_10x10() {
    int n = 10;
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);
    
    // Popolo la diagonale principale e le sottodiagonali
    for (int i = 0; i < n; ++i) {
        A(i, i) = 2.0;
        if (i > 0)     A(i, i - 1) = -1.0;
        if (i < n - 1) A(i, i + 1) = -1.0;
    }
    
    Eigen::VectorXd b = Eigen::VectorXd::Ones(n);
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n);
    Eigen::VectorXd x = gcd(A, b, x0, 1e-6);
    
    if ((b - A * x).norm() < 1e-5) {
        cout << "Test 10x10 Tridiagonale: OK\n" << endl;
        return true;
    } else {
        cout << "Test 10x10 Tridiagonale: FALLITO!\n" << endl;
        return false;
    }
}

//Test stress con matrice non-SPD (matrice altamente asimetrica, dovrebbe eccedere il n iterazioni)
bool test_non_spd() {
    Eigen::MatrixXd A(3, 3);
    A << 1, 10,  0,
         0,  1, 10,
         0,  0,  1;
          
    Eigen::VectorXd b(3);
    b << 1, 1, 1;
    
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(3);
    
    // Qui dovrebbe stampare l'errore di itermax in automatico da file GC.hpp
    Eigen::VectorXd x = gcd(A, b, x0, 1e-8);
    
    if ((b - A * x).norm() >= 1e-5) {
        cout << "Test matrice non-SPD: OK\n" << endl;
        return true;
    } else {
        cout << "Test matrice non-SPD: FALLITO!\n" << endl;
        return false;
    }
}

//Test con soluzione nota di tutti 1 (Stile MATLAB)
bool test_soluzione_nota_ones() {
    int n = 5;
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);
    for (int i = 0; i < n; ++i) {
        A(i, i) = 4.0;
        if (i > 0)     A(i, i - 1) = -1.0;
        if (i < n - 1) A(i, i + 1) = -1.0;
    }

    // Costruisco la soluzione esatta (tutti 1)
    Eigen::VectorXd x_esatta = Eigen::VectorXd::Ones(n);
    Eigen::VectorXd b = A * x_esatta;
    
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n);
    Eigen::VectorXd x = gcd(A, b, x0, 1e-7);
    
    // Calcoliamo sia il residuo che l'errore vero sulla soluzione
    double errore_vero = (x - x_esatta).norm();
    double residuo = (b - A * x).norm();
    
    if (residuo < 1e-5 && errore_vero < 1e-5) {
        cout << "Test Soluzione Nota (Ones): OK\n" << endl;
        return true;
    } else {
        cout << "Test Soluzione Nota (Ones): FALLITO!\n" << endl;
        return false;
    }
}

int main() {
    bool successo = true;
    if(!test_spd_3x3() || !test_tridiagonale_10x10() || !test_non_spd() || !test_soluzione_nota_ones()) successo = false;
    
    if(successo){
        cout << "Tutti i test PASSATI" << endl;
        return EXIT_SUCCESS;
    }
    else
    {
        cout << "Almeno un test è FALLITO" << endl;
        return EXIT_FAILURE;
    }
}