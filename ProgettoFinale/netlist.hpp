#pragma once
#include <string>
#include <vector>
#include <istream>
#include <fstream>
#include <iostream>
#include <cstdlib>   
#include "component.hpp"

std::vector<Componente> leggi_netlist(std::istream& in) 
{
	std::vector<Componente> componenti; // vettore delle componenti 
	std::string nome; 
	double valore = 0.0;
	int nodo1 = 0;
	int nodo2 = 0;

	// in >> nome legge un componente alla volta e si ferma a fine input;
	// gli spazi bianchi (anche i ritorni a capo) vengono saltati da soli.
	while (in >> nome)
	{
		if (!(in >> valore >> nodo1 >> nodo2)) // se non legge in quessto modo => errore
		{
			std::cerr << "ERRORE: la riga è malformata per il " << "componente " << nome << ".\n";
			exit(1);
		}

		char tipo = nome[0]; // tipo del componente: prima lettera del nome ('R' oppure 'V').
		
		if (tipo != 'R' && tipo != 'V') // se il tipo non è nessuno dei due => errore
		{
			std::cerr << "ERRORE: componente sconosciuto '" << nome << "'. Devi inserire R o V.\n";
			exit(1);
		}

		Componente c; // creo una variabile c di tipo Componente che avevo creato in component.hpp
		c.nome   = nome;
		c.tipo   = tipo;
		c.valore = valore;
		c.nodo1  = nodo1;
		c.nodo2  = nodo2;
		componenti.push_back(c); // aggiungo c al fondo del vettore componenti
	}

	return componenti;
}

// Variante di comodo: legge da file.
std::vector<Componente> leggi_netlist_da_file(const std::string& percorso)
{
	std::ifstream file(percorso);
	if (!file.is_open()) // se il file non si apre esco 
	{
		std::cerr << "ERRORE: impossibile aprire il file " << percorso << "\n";
		exit(1);
	}
	return leggi_netlist(file);
}
