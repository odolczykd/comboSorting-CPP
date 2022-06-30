// Podstawy programowania - projekt zaliczeniowy, rok 2020/21
// Autor: Dawid Odolczyk, grupa LE*
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>
using namespace std;

//pliki strumienia wejscia/wyjscia danych z/do pliku
ifstream input;
ofstream output;

//struktura pomocnicza do rozszerzania tablicy z danymi do posortowania
struct vctr{	
	int *T;
	int size, cap;
};

//funkcja rozszerzajaca tablice z danymi i dopisujaca wartosci do niej
void addToArray(vctr *pv, int X){	
	if(pv->size == pv->cap){
		pv->cap = pv->cap + 1;
		int *ext = new int[pv->cap];	//ext - nowa, rozszerzona (o nowy element X) tablica 
		for(int i=0; i<(pv->size); i++) *(ext+i)=*((pv->T)+i);	//przepisanie wartosci z poprzedniej tablicy 
		delete [] pv->T;
		pv->T = ext;
	}
	*(pv->T + pv->size) = X;
	(pv->size)++;
}

//funkcja zamiany wartosci zmiennych x i y
void swp(int *x, int *y){	
	int t=*x;
	*x=*y;
	*y=t;
}

//sortowanie przez wybieranie
void selectionSort(int *beg, int *end){		
	int indexMin;
	for(int i=0; i<end-beg; i++){
		indexMin=i;
		for(int j=i+1; j<end-beg; j++){
			if(*(beg+j)<*(beg+indexMin)) indexMin=j;
		}
		swp((beg+i),(beg+indexMin));
	}
}

//funkcja przywracajaca tablicy danych wlasnosc kopca (do sortowania przez kopcowanie)
void heapify(int *beg, int *end, int k, int n){		
	int i=*(beg+k-1), j;
	while(k<=n/2){
		j=2*k;
		if((j<n) && (*(beg+j-1)<*(beg+j))) j++;
		if(i>=*(beg+j-1)) break;
		else{
			*(beg+k-1)=*(beg+j-1);
			k=j;
		}
	}
	*(beg+k-1)=i;
}

//sortowanie przez kopcowanie
void heapSort(int *beg, int *end){		
	int k, t, n=end-beg;
	for(k=n/2; k>0; k--) heapify(beg,end,k,n);
	do{
		swp(beg,(beg+n-1));
		n--;
		heapify(beg,end,1,n);
	} while(n>1);
}

//funkcja obliczajaca mediane 3-elementowego zbioru
int med3(int a, int b, int c){		
	int *T=new int[3], res;
	*T=a;
	*(T+1)=b;
	*(T+2)=c;
	selectionSort(T,T+3);
	res=*(T+1);
	delete [] T;
	return res;
}

//zmienna liczaca poziom zaglebienia rekurencji w sortowaniu szybkim
int depth=1;

//zmienna przechowujaca dlugosc sortowanego ciagu
int N=0;

//sortowanie szybkie (wzbogacone o sort. przez wybor i sort. przez kopcowanie)
void quickSort(int *beg, int *end){		
	//jesli pozostaly nie wiecej niz 4 el. do posortowania, to zastosuj sort. przez wybor
	if(end-beg<=4) selectionSort(beg,end);
	
	//jesli poziom zaglebienia rekurencji przekracza 2*log2(N), to zastosuj sort. przez kopcowanie
	else if(depth>=2*log2(N)) heapSort(beg,end);
	
	//jesli poprzednie warunki nie zostaly spelnione, zastosuj sortowanie szybkie
	else{
		int *d, *i, *c, x;
		if(beg<end){
			x=med3(*beg,*(beg+((end-beg)/2)),*(end-1));
			if(x==(*beg)) d=beg;
				else if(x==(*(beg+((end-beg)/2)))) d=(beg+((end-beg)/2));
					else d=(end-1);
			swp(d,end-1);
			c=beg;
			for(i=beg; i<end-1; i++){
				if(*i<x){
					swp(i,c);
					c++;
				}
			}
			swp(c,end-1);
			depth++;
			quickSort(beg,c);
			quickSort(c+1,end);
		}
	}
}

int main(){
	char srcFilePath[100], outFilePath[100];
	cout<<"PROJEKT ZALICZENIOWY - SORTOWANIE\nDawid Odolczyk, grupa LE*\n";
	cout<<setw(33)<<setfill('=')<<"="<<endl<<endl;
	
	cout<<"Podaj sciezke dostepu do pliku z danymi do posortowania: ";
	cin>>srcFilePath;
	input.open(srcFilePath,ios::in);
	
	if(input.good()){
		int X;
		vctr v;
		v.size=0;
		v.cap=2;
		v.T=new int[2];
		
		while(!input.eof()){
			input>>X;
			N++;
			addToArray(&v,X);
		}
		input.close();

		/*
		cout<<"\nTablica przed posortowaniem:\n";
		for(int i=0; i<v.size; i++) cout<<setw(10)<<setfill(' ')<<*(v.T+i);
		cout<<endl;
		*/

		cout<<"\nTrwa sortowanie...\n";
		quickSort(v.T,(v.T)+(v.size));
		cout<<"\nSortowanie zakonczone.\n";
		
		//tworzenie nazwy pliku wyjsciowego
		strcpy(outFilePath,srcFilePath);
		int len=strlen(outFilePath);
		for(int i=0; i<4; i++) outFilePath[len-4+i]='\0';
		strcat(outFilePath,".out.txt");
		output.open(outFilePath,ios::out);

		//cout<<"\nTablica posortowana niemalejaco: \n";
		for(int i=0; i<v.size; i++){
			//cout<<setw(10)<<*(v.T+i);
			output<<setw(10)<<*(v.T+i);
		}
		//cout<<endl;
		
		cout<<"\nPosortowana tablice zapisano w sciezce "<<outFilePath<<"."<<endl;
		delete [] v.T;
		output.close();
	}
	else cout<<"Wystapil blad przy odczycie danych z pliku!\n";
	
	return 0;
}
