#include "graphs.hpp"
#include <iostream>
#include <string>

using namespace std;
int main()
{
    cout << "1 Test Inserimento e Duplicati" << endl;
    unidirected_graph<int> g1;
    g1.add_edge({1, 3});
    g1.add_edge({3, 1}); 
    g1.add_edge({2, 60});
    g1.add_edge({3, 2});

    cout << "Grafo G1 (dovrebbe avere 3 archi): " << g1 << endl; 

    cout << "2 Verifica Nodi e Vicini" << endl;
    cout << "Nodi presenti nel grafo: ";

    for(auto n : g1.all_nodes()) {cout << n << " ";}
    cout << endl;

    int nodo_test = 3;
    cout << "Vicini del nodo " << nodo_test << ": ";
    auto vicini = g1.neighours(nodo_test);
    for(auto v : vicini) cout << v << " ";
    cout << "\n" << endl;


    cout << "3 Verifica Numerazione Arco Invertito" << endl;
    unidirected_edge<int> e_cerca(60, 2); 
    int id = g1.edge_number(e_cerca);
    
    cout << "Cerco la numerazione corrispondente all'arco (60,2): " << endl;
    if(id != -1) {
        cout << "Numero trovato: " << id << endl;
        cout << "Verifica inversa: l'arco al numero:  " << id << " e' " << g1.edge_at(id) << endl;
    } else {
        cout << "Errore: arco non trovato!" << endl;
    }
    cout << endl;

    cout << "4 Test Operatore Sottrazione (G1 - G2)" << endl;
    unidirected_graph<int> g2;
    g2.add_edge({1, 3}); // Arco comune a G1
    
    cout << "G1: " << g1;
    cout << "G2: " << g2;
    
    unidirected_graph<int> g_risultato = g1 - g2;
    cout << "Risultato G1 - G2 (dovrebbe mancare l'arco (1,3)): " << g_risultato << endl;

    cout << "5 Test Genericità (Stringhe)" << endl;
    unidirected_graph<string> g_str;
    g_str.add_edge({"Torino", "Milano"});
    g_str.add_edge({"Milano", "Roma"});
    cout << "Grafo di città: " << g_str << endl;

    return 0;
}