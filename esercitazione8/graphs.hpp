#pragma once
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <iostream>

template<typename T>
class unidirected_edge
{
    private:
        T first;
        T second;
    public:
        unidirected_edge() = default;
        unidirected_edge(const T& a, const T& b)
        {
            if(a>b)
            {
                first = b; second = a;
            }
            else
            {
                first = a; second = b;
            }
        }

        T from() const {return first;}
        T to() const {return second;}

        bool operator<(const unidirected_edge<T>& other) const
        {
            if(first != other.first)
            {
                return first<other.first;
            }
            return second<other.second;
        }   
        bool operator==(const unidirected_edge<T>& other) const
        {
            if(first == other.first)
            {
                if(second == other.second) return true;
                else return false;
                
            }
            else return false;
        }   


};

template<typename T>
std::ostream&
operator<<(std::ostream& os, const unidirected_edge<T>& a)
{
    os << "(" << a.from() << "," << a.to() << ")";
    return os;
}


using namespace std;
template<typename T>
class unidirected_graph
{
    private:
        set<unidirected_edge<T>> edges; 
        set<T> nodes;    
        map<T,set<T>> nnear; 
        public:
        unidirected_graph()=default;
        unidirected_graph(const unidirected_graph& other)
        {
            edges = other.edges;
            nodes = other.nodes;
            nnear = other.nnear;
        }

        set<T> neighours(const T& node) const
        {
            auto it = nnear.find(node);
            
            if(it != nnear.end())
                return it->second; 
            else return set<T>();
        }

        void add_edge(const unidirected_edge<T>& newarc)
        {
            auto prova = edges.insert(newarc);
            if(prova.second)
            {
                nodes.insert(newarc.from());
                nodes.insert(newarc.to());
                nnear[newarc.from()].insert(newarc.to());
                nnear[newarc.to()].insert(newarc.from());
            }
        }

        set<unidirected_edge<T>> all_edges() const {return edges;}
        set<T> all_nodes() const {return nodes;}

        int edge_number(const unidirected_edge<T>& edge) const
        {
            auto target = edges.find(edge);
        
            if (target == edges.end()) {
                return -1;
            }

            int index = 0;
            for (auto it = edges.begin(); it != target; ++it) {
                index++;
            }
            
            return index;
        }

        unidirected_edge<T> edge_at(size_t num) const
        {
            if (num >= edges.size()) 
            {
                std::cerr << "Errore: Indice dell'arco non valido" << std::endl;
                return unidirected_edge<T>(); 
            }

            auto it = edges.begin();
            
            for (size_t i = 0; i < num; i++) 
            {
                ++it; 
            }
            
            return *it; 
        }

        unidirected_graph<T> operator-(const unidirected_graph<T>& other) const
        {
            unidirected_graph<T> result;
            for(const auto& corrente : edges)
            {
                if(other.edges.find(corrente) == other.edges.end())
                {
                   result.add_edge(corrente);
                }
            }
            return result;
        }

};


template<typename T>
std::ostream&
operator<<(std::ostream& os, const unidirected_graph<T>& g)
{
    auto all_the_edges = g.all_edges();
    size_t n = all_the_edges.size();
    size_t i = 0;
    
    os << "{";
    for(const auto& edge : all_the_edges)
    {
        os << edge;
        if(i != n - 1) os << ", ";
        i++;
    }
    os << "}\n";
    return os;
}

