#include<iostream>
using namespace std; 
int main() 
{ 
    char arr[]="The elements are: ";   //array initialization
    int count = 0;
    cout << "The number of 'e' in \"The elements are: \" is ";
    for(char i : arr)
    {
    	if(i == 'e')
    		count++;
    }
    cout << count << endl;
    return 0;
}
