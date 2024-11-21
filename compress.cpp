#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Helper.hpp"
#include "HCTree.hpp"
using namespace std;

int main(int argc,char * argv[])
{
    bool flag = false;
    if (argc < 3 || argc > 3)
    {
        cout << "invalid number of arguments" << endl;
    }

    string originalFile = argv[1];
    ifstream input(argv[1]);
    string compressedFile = argv[2];

    FancyInputStream inputFile(argv[1]);

    if (input.peek() == ifstream::traits_type::eof()) 
    {
        FancyOutputStream outputFile(argv[2]);
        exit(0);
    }
    
    int numCharacters = 0;
    vector<int> freqs(256,0);
    int index = inputFile.read_byte();
    while (index != -1)                     // works
    {
        numCharacters++;
        freqs[index] = freqs[index]+1;
        index = inputFile.read_byte();
    }

    // erase all characters that didn't occur
//    myVector.erase(remove_if(myVector.begin(), myVector.end(), [](int x){ return x == 0; }), myVector.end());

    // Create huffman tree
    HCTree huff;
    huff.build(freqs);

    
    
    
    // add header
    
    FancyOutputStream outputFile(argv[2]);
    
    outputFile.write_int(numCharacters);
    for (int i = 0;i < 256;i++)
    {
       // cout << "Here: " << i << endl;
        outputFile.write_int(freqs[i]);
    }
    
    inputFile.reset();
    index = inputFile.read_byte();
    while (index != -1)
    {
        unsigned char symbol = index;
        huff.encode(symbol,outputFile);
        index = inputFile.read_byte();
    }

    input.close();
    
    return 0;
}
