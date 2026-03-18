#include <iostream>
using namespace std;

int main(){
static const int N = 5;
double arr[N]={2.2,1.1,4.4,3.3,0.0};
double arr_ordinato[N];

for(int i=0; i<N;i++){
    if (i==0){
        arr_ordinato[i]=arr[i];
    }else{
        double attuale=arr[i];
        int j=i-1;
        while(j>=0 && arr_ordinato[j]>attuale){
            arr_ordinato[j+1]=arr_ordinato[j];
            j--;
        }
        arr_ordinato[j+1]=attuale;
        }
    }
cout << "Array ordinato: ";
for (int i = 0; i < N; i++) {
    cout << arr_ordinato[i] << " ";
}
cout << "\n";
return 0;
} 
