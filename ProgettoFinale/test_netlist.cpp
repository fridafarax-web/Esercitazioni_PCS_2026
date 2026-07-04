#include <iostream>
#include <sstream>
#include <cmath>
#include "netlist.hpp"


int main()
// provo con una netlist 
{ 
	std::string testo =
		"\n"
		"R1   10    1   2\n"
		"\t R2\t20\t2\t3 \n"
		"   \n"                      
		"R3 30 2 4\n"
		"V1     10   1 3\n"
		"V2 20 4 3\n";

	std::istringstream in(testo); // crea un flusso in leggibile 
	std::vector<Componente> componenti = leggi_netlist(in);

	bool ok = true;

	if (componenti.size() != 5)
	{
		std::cout << "ERRORE: attesi 5 componenti, trovati " << componenti.size() << "\n";
		ok = false;
	}

	if (ok)
	{
		// R1 10 1 2
		ok = ok && componenti[0].nome == "R1"
		        && componenti[0].tipo == 'R'
		        && std::abs(componenti[0].valore - 10.0) < 1e-9
				//&& componenti[0].valore == 10.0
		        && componenti[0].nodo1 == 1 && componenti[0].nodo2 == 2;
		// R2 con tab
		ok = ok && componenti[1].nome == "R2"
		        && componenti[1].nodo1 == 2 && componenti[1].nodo2 == 3;
		// V1 generatore
		ok = ok && componenti[3].nome == "V1"
		        && componenti[3].tipo == 'V'
		        && componenti[3].valore == 10.0;
	}

	if (ok)
	{
		std::cout << "test_netlist: OK\n";
		return EXIT_SUCCESS;
	}
	std::cout << "test_netlist: FALLITO\n";
	return EXIT_FAILURE;
}
