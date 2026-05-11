#include "graphs.hpp"
#include <iostream>
#include <string>

using namespace std;


bool test_inserimento() {
    unidirected_graph<int> g;
    unidirected_edge<int> e(1, 2);
    g.add_edge(e);

    if (g.all_edges().size() == 1 && 
        g.all_nodes().size() == 2 && 
        g.all_nodes().contains(1) && 
        g.all_nodes().contains(2)) 
    {
        cout << "Test inserimento: OK" << endl;
        return true;
    } else {
        cout << "Test inserimento: FALLITO!" << endl;
        return false;
    }
}


bool test_mapping_id() {
    unidirected_graph<string> g;
    unidirected_edge<string> e1("Roma", "Milano");
    g.add_edge(e1);

    int id = g.edge_number(e1);
    unidirected_edge<string> e2 = g.edge_at(id);


    if (id == 0 && (e2.from() == "Milano" || e2.from() == "Roma")) {
        cout << "Test Mapping ID: OK" << endl;
        return true;
    } else {
        cout << "Test Mapping ID: FALLITO!" << endl;
        return false;
    }
}


bool test_vicini() {
    unidirected_graph<int> g;
    g.add_edge({1, 2});
    g.add_edge({1, 3});

    auto v = g.neighours(1);

    
    if (v.size() == 2 && v.contains(2) && v.contains(3)) {
        cout << "Test Vicini: OK" << endl;
        return true;
    } else {
        cout << "Test Vicini: FALLITO!" << endl;
        return false;
    }
}


bool test_sottrazione() {
    unidirected_graph<int> g1, g2;
    g1.add_edge({1, 2});
    g1.add_edge({2, 3});
    
    g2.add_edge({2, 3}); 

    unidirected_graph<int> diff = g1 - g2;

    
    if (diff.all_edges().size() == 1 && 
        diff.edge_number({1, 2}) != -1 && 
        diff.edge_number({2, 3}) == -1) 
    {
        cout << "Test Sottrazione (-): OK" << endl;
        return true;
    } else {
        cout << "Test Sottrazione (-): FALLITO!" << endl;
        return false;
    }
}

int main() {
    
    bool successo = true;
    if(!test_inserimento()||!test_mapping_id()||!test_vicini()||!test_sottrazione()) successo = false;
    
    if(successo){
        cout << "Tutti i test PASSATI" <<endl;
        return EXIT_SUCCESS;
    }
    else
    {
        cout<< "Almeno un test è FALLITO"<<endl;
        return EXIT_FAILURE;
    }
}