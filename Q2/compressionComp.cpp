#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // Read the contents of the original documents
    ifstream file1("file1.txt");
    ifstream file2("file2.txt");
    string text1((istreambuf_iterator<char>(file1)), istreambuf_iterator<char>());
    string text2((istreambuf_iterator<char>(file2)), istreambuf_iterator<char>());

    // Read the contents of the encoded documents
    ifstream encoded1("encoded1.txt");
    ifstream encoded2("encoded2.txt");
    string encodedText1((istreambuf_iterator<char>(encoded1)), istreambuf_iterator<char>());
    string encodedText2((istreambuf_iterator<char>(encoded2)), istreambuf_iterator<char>());

    // Calculate the sizes of the original and encoded documents in bytes
    int originalSize1 = text1.length();
    int originalSize2 = text2.length();
    int encodedSize1 = encodedText1.length();
    int encodedSize2 = encodedText2.length();

    // Calculate the compression ratios
    float compressionRatio1 = (float)encodedSize1 / originalSize1;
    float compressionRatio2 = (float)encodedSize2 / originalSize2;

    // Compare the compression ratios
    cout << "Compression ratio for document 1: " << compressionRatio1 << endl;
    cout << "Compression ratio for document 2: " << compressionRatio2 << endl;

    return 0;
}
