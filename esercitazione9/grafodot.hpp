#pragma once
#include <fstream>
#include <string>
#include "grafi.hpp"

// Aggiunto il secondo parametro template P per gestire il tipo del peso
template <typename T, typename P>
void salva_grafo_dot(const unidirected_graph<T, P>& G, const std::string& nome_file) {
    std::ofstream out(nome_file); // Apro il file in scrittura
    
    if (!out) {
        std::cerr << "Errore nella creazione del file: " << nome_file << std::endl;
        return;
    }

    out << "graph G {\n"; // Inizio del file DOT
    out << "    node [shape=circle];\n"; // Rende i nodi tondi e graficamente puliti
    
    // Scorro tutti gli archi e stampo anche il peso associato usando get_peso()
    for (const auto& arco : G.all_edges()) {
        out << "    \"" << arco.from() << "\" -- \"" << arco.to() << "\" [label=\"" << G.get_peso(arco) << "\"];\n";
    }
    
    out << "}\n"; // Fine del file
    out.close();
    std::cout << "File " << nome_file << " salvato correttamente." << std::endl;
}
