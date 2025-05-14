#include<iostream>
using namespace std;
class Arr{
public:
	Arr(){
		arr = nullptr;
	}
	~Arr(){
		if(arr != nullptr)
		delete[] arr;
	}
	
	void setLength(int length){
		if(arr != nullptr){
			delete[] arr;
		}
		Length = length;
		arr = new int [Length];
	}
	friend std::istream& operator>>(std::istream& input,Arr &a){
		for(int i = 0;i < a.Length;i++){
			input >> *(a.arr+i);
		}
		return input;
	}
	void swap(Arr &b){
		int* temparr = arr;
		int tempLength = Length;
		
		arr = b.getArr();
		Length = b.getLength();
		
		b.getArr() = temparr;
		b.getLength() = tempLength;
	}
	int* &getArr(){
		return arr;
	}
	int &getLength(){
		return Length;
	}
	friend std::ostream& operator<<(std::ostream& output,Arr &a){
		for(int i = 0;i < a.Length;i++){
			cout<<*(a.arr+i)<<" ";
		}
		cout<<endl;
		return output;
	}
private:
	int Length;
	int *arr;
};
int main(){
	Arr a;
	Arr b;
	int x,y;
	cin>>x>>y;
	a.setLength(x);
	b.setLength(y);
	cin>>a;
	cin>>b;
	a.swap(b);
	cout<<a;
	cout<<b;
}

