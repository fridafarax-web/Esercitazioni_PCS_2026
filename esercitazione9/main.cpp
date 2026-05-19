#include <iostream>
#include "lifofifo.hpp"
#include "grafi.hpp"
#include "visite.hpp"
#include "grafodot.hpp"

using namespace std;

int main() {
    unidirected_graph<int> G;

    //test proposto in classe
    G.add_edge({1,2});G.add_edge({1,3});G.add_edge({1,4});G.add_edge({1,6});G.add_edge({2,4});
    G.add_edge({2,5});G.add_edge({2,7});G.add_edge({4,6});G.add_edge({4,7});G.add_edge({3,6});
    G.add_edge({5,7});G.add_edge({6,7});G.add_edge({6,8});G.add_edge({8,9});G.add_edge({7,9});
    
    // // Creo un grafo di test (un po' intrecciato per far vedere la differenza tra BFS e DFS)
    // G.add_edge({1, 2});
    // G.add_edge({1, 3});
    // G.add_edge({2, 4});
    // G.add_edge({2, 5});
    // G.add_edge({3, 6});
    // G.add_edge({5, 6}); // Questo arco crea un ciclo
    cout << "1.Implemento BFS, DFS, DFS Ricorsiva per il seguente grafo:\n" <<G <<endl;
    // Visita in ampiezza (BFS)
    fifo<int> coda;
    unidirected_graph<int> albero_bfs = graph_visit(G, 1, coda);
    salva_grafo_dot(albero_bfs, "visita_bfs.dot");

    // Visita in profondita (DFS)
    lifo<int> pila;
    unidirected_graph<int> albero_dfs = graph_visit(G, 1, pila);
    salva_grafo_dot(albero_dfs, "visita_dfs.dot");
    
    // Salvo anche il grafo originale per confronto
    salva_grafo_dot(G, "grafo_originale.dot");
    // Visita in profondità (DFS RICORSIVA)
    unidirected_graph<int> albero_dfs_ricorsiva = recursive_dfs(G, 1);
    salva_grafo_dot(albero_dfs_ricorsiva, "visita_dfs_ricorsiva.dot");


    // Creiamo un grafo con nodi interi e pesi double (sfrutta il default = double)
    unidirected_graph<int> G1;

    // Inserisco archi
    G1.add_edge({1, 2});
    G1.add_edge({1, 3});
    G1.add_edge({2, 3});
    G1.add_edge({2, 4});
    G1.add_edge({3, 4});

    // Assegno pesi
    G1.add_peso({1, 2}, 5.0);
    G1.add_peso({1, 3}, 1.0);
    G1.add_peso({2, 3}, 2.0); // Il cammino 1->3->2 (costo 3) batte l'arco diretto 1->2 (costo 5)
    G1.add_peso({2, 4}, 1.0);
    G1.add_peso({3, 4}, 7.0);

    // Stampa del grafo di partenza usando operator<< 
    cout << "\n2.Implemento Dijkstra sul grafo:" << endl;
    cout << G1<<endl;

    // eseguo dijkstra
    int sorgente = 1;
    cout << "Eseguo Dijkstra partendo dal nodo sorgente: " << sorgente << endl;
    
    unidirected_graph<int, double> albero_minimi = dijkstra(G1, sorgente);

    // 4. STAMPA DEL RISULTATO
    cout << "\nAlbero dei cammini minimi (Grafo Risultato):" << endl;
    cout << albero_minimi<<endl;

    // Ispeziono i pesi degli archi scelti per essere sicuro
    cout << "Dettagli dei pesi degli archi scelti dall'algoritmo:" << endl;
    for (const auto& arco : albero_minimi.all_edges()) {
        cout << "Arco dell'albero " << arco << " -> Peso minimo: " << albero_minimi.get_peso(arco) << endl;
    }
    cout << "\n";

    salva_grafo_dot(G1,"originale.dot");
    salva_grafo_dot(albero_minimi,"minimi.dot");

    //provo anche dijkstra con le stringhe sui nodi
    unidirected_graph<string> G2;

    G2.add_edge({"Torino", "Milano"});
    G2.add_edge({"Torino", "Genova"});
    G2.add_edge({"Milano", "Genova"});
    G2.add_edge({"Milano", "Bologna"});
    G2.add_edge({"Genova", "Bologna"});
    G2.add_edge({"Milano", "Venezia"});
    G2.add_edge({"Bologna", "Venezia"});
    G2.add_edge({"Venezia", "Roma"});
    G2.add_edge({"Bologna", "Roma"});

    // Assegnazione dei pesi
    G2.add_peso({"Torino", "Milano"}, 2.0);
    G2.add_peso({"Torino", "Genova"}, 7.0);
    G2.add_peso({"Milano", "Genova"}, 3.0);  // Scorciatoia per Genova: TO->MI->GE costa 5 (batte 7)
    
    G2.add_peso({"Milano", "Bologna"}, 6.0); // TO->MI->BO costerebbe 8
    G2.add_peso({"Genova", "Bologna"}, 1.0); // Ma TO->MI->GE->BO costa 6 (Nuova stradad per Bologna)

    G2.add_peso({"Milano", "Venezia"}, 11.0);
    G2.add_peso({"Bologna", "Venezia"}, 2.0); // TO->MI->GE->BO->VE costa 8 (Batte quella da Milano)

    G2.add_peso({"Bologna", "Roma"}, 10.0);
    G2.add_peso({"Venezia", "Roma"}, 4.0);   // TO->...->VE->RO costa 12. Batte il percorso diretto da Bologna (16)

    //aggiungo componente isolata
    G2.add_edge({"Cagliari", "Sassari"});
    G2.add_peso({"Cagliari", "Sassari"}, 5.0);

    
    cout << "\n3.Implemento Dijkstra su un grafo formato da stringhe:\n"<<G2<< endl; 
    // Eseguiamo Dijkstra partendo da Torino
    string partenza = "Torino";
    cout << "Esecuzione di Dijkstra da: " << partenza <<"\n"<< endl;
    
    unidirected_graph<string, double> albero_minimi_string = dijkstra(G2, partenza);

    // Salva il grafo di partenza completo
    salva_grafo_dot(G2, "mappa_italia_completa.dot");

    // Salva l'albero finale ottenuto
    salva_grafo_dot(albero_minimi_string, "albero_cammini_minimi.dot");

    // Stampiamo a schermo il dettaglio dei percorsi ottimali scelti
    cout << "\nVerifica archi minime con stringhe" << endl;
    for (const auto& arco : albero_minimi_string.all_edges()) {
        cout << "Cammino Minimo include: " << arco << " con peso: " << albero_minimi_string.get_peso(arco) << endl;
    }
    cout << "Per maggiori info printare su graphviz"<<endl;

    return 0;

}

