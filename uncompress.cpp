#include <iostream>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;

int main(int argc,char * argv[])
{
    if (argc < 3 || argc > 3)
    {
        cout << "invalid number of arguments" << endl;
        return -1;
    }
    ifstream input(argv[1]);
    if (input.peek() == ifstream::traits_type::eof()) 
    {
        FancyOutputStream outputFile(argv[2]);
        exit(0);
    }
    
    FancyInputStream inputFile(argv[1]);    
    vector<int> freqs(256,0);
    int numCharacters = inputFile.read_int();
    for (int i= 0;i < 256;i++)
    {
        int freq = inputFile.read_int();
        freqs[i] = freq;
      //  cout << "freqs[" << i << "]: " << freq << endl;
    }    

    HCTree huff;
    huff.build(freqs);

    FancyOutputStream outputFile(argv[2]);

    unsigned char c;
    for (int i = 0;i < numCharacters;i++)
    {
        c = huff.decode(inputFile);
        outputFile.write_byte(c);
    }
    
    

    input.close();
    return 0;
}
