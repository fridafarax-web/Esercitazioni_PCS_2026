#pragma once

#include <optional>
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

template<typename T>
void bubblesort(vector<T>& vec)
{
    int N = vec.size();
    for (int i = 0; i < N - 1; i++) {
        for (int j = N - 1; j > i; j--) { 
            if (vec[j] < vec[j - 1]) {
                T key = vec[j];
                vec[j] = vec[j - 1];
                vec[j - 1] = key;
            }
        }
    }
}

template<typename T>
void insertionsort(vector<T>& vec)
{
	int N=vec.size();
	for (int j=1;j<N;j++){
		T key = vec[j];
		int i = j-1;
		while(i>=0 && vec[i]>key){
			vec[i+1] = vec[i];
			i=i-1;
		}
		vec[i+1]=key;
	}
}

template<typename T>
void selectionsort(vector<T>& vec)
{
	int N=vec.size();
	for (int i=0;i<N;i++){
		int min = i;
		for(int j = i+1; j<N; j++){
			if(vec[j]<vec[min]){
				min = j;
			}
		}
		
		T key = vec[i];
		vec[i] = vec[min];
		vec[min] = key;
	}
	return;
}

template<typename T>
void merge(vector<T>& A, size_t p, size_t q, size_t r)
{
	size_t n1=q-p+1;
	size_t n2=r-q;
	vector<T> L(n1+1);
	vector<T> R(n2+1);
	
	for(size_t i=0;i<n1;i++)
	{
		L[i]=A[p+i];
	}

	for(size_t j=0;j<n2;j++)
	{
		R[j]=A[q+j+1];
	}
	if constexpr(is_arithmetic_v<T>)
    {    
        L[n1] = numeric_limits<T>::max();
        R[n2] = numeric_limits<T>::max();
    }
    else 
    {
        L[n1] = string(1000,'\xff');
        R[n2] = string(1000,'\xff');
    }
	size_t i=0;
	size_t j=0;

	for(size_t k=p;k<=r;k++)
	{
		if(L[i]<=R[j]){
			A[k]=L[i];
			i=i+1;
		}
		else{
			A[k]=R[j];
			j=j+1;
		}
	}
}

template<typename T>
void mergesort_fake(vector<T>& A, size_t p, size_t r)
{
	if(p<r){
		size_t q=(p+r)/2;
		mergesort_fake(A,p,q);
		mergesort_fake(A,q+1,r);
		merge(A,p,q,r);
	}
}

template<typename T>
void mergesort(vector<T>& vec)
{
	if(vec.size()>1){
		mergesort_fake(vec,0,vec.size()-1);
	}
}

template<typename T>
int partition(vector<T>& A, int p, int r)
{
	T x=A[r];
	int i=p-1;

	for (int j=p; j<=r-1;j++)
	{
		if (A[j]<=x){
			i=i+1;
			T  key = A[i];
            A[i] = A[j];
            A[j] = key;
		}
	}
	T key = A[i+1];
    A[i+1] = A[r];
    A[r] = key;
    return i+1;
}

template<typename T>
void quicksort_fake(vector<T>& A, int p, int r)
{
	if(p<r)
	{
		int q=partition(A,p,r);
		quicksort_fake(A,p,q-1);
		quicksort_fake(A,q+1,r);
	}
}

template<typename T>
void quicksort(vector<T>& vec)
{
	if(vec.size()>1){
		quicksort_fake(vec,0,vec.size()-1);
	}
}

template<typename T>
int issorted(const vector<T>& vec)
{
	int N=vec.size();
	for(int i=0;i<N-1;i++){
		if(vec[i]>vec[i+1]){
			return 1;
		}
	}
	return 0;
}

template<typename T>
void hybridsort_fake(vector<T> vec, int p, int r)
{
	for (int j=p+1; j<=r;j++){
		T key=vec[j];
		int i=j-1;
		while (i>=0 && vec[i]>key){
			vec[i+1] =vec[i];
			i=i-1;
		}
		vec[i+1]=key;
	}
}

template<typename T>
void hybrid_sort(vector<T>& vec, int p, int r)
{
	// se la dimensione è minore di 100 passa a insertionsort
	if((r-p)<100){
		hybridsort_fake(vec, p, r);
	}else{
		if(p<r){
			int q = partition(vec,p,r);
			hybrid_sort(vec,p,q-1);
			hybrid_sort(vec,q+1,r);
		}
	}
	return;
}