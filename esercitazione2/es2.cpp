#include <iostream>
#include <cmath>
using namespace std;

int main()
{
static const int N = 10;
double arr[N];
double minimo;
double massimo;
double media=0;
double varianza=0;
double dev_std;

for (int i = 0; i < N; i++) {
    arr[i] = i * 1.1;
    media=media+arr[i];
    if (i==0){
        minimo=arr[i];
        massimo=arr[i];
    }else{
        minimo=min(minimo,arr[i]);
        massimo=max(massimo,arr[i]);
    }
}
media=media/N;

for(int i=0;i<N;i++){
    varianza=varianza +pow(media-arr[i], 2); //pow elevamento a potenza
}
varianza=varianza/N;
dev_std=sqrt(varianza);

cout << "Il minimo del vettore arr è: "<< minimo <<"\n";
cout << "Il massimo del vettore arr è: "<< massimo << "\n";
cout << "La deviazione standard del vettore arr è: "<< dev_std<< "\n";
}