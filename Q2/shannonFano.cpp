#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <iomanip>
using namespace std;

struct Symbol {
    char symbol;
    double probability;
    string code;

    Symbol(char s, double p) : symbol(s), probability(p), code("") {}
};

bool symbolCompare(Symbol s1, Symbol s2) {
    return s1.probability > s2.probability;
}

void shannonFano(vector<Symbol>& symbols, int start, int end) {
    if (end - start <= 1) {
        return;
    }

    int mid = start;
    double totalProb = 0.0;

    for (int i = start; i < end; i++) {
        totalProb += symbols[i].probability;
    }

    totalProb /= 2.0;

    while (abs(int(totalProb - symbols[mid].probability)) > abs(int(totalProb - symbols[mid+1].probability))) {
        mid++;
        totalProb -= symbols[mid].probability;
    }

    for (int i = start; i <= mid; i++) {
        symbols[i].code += "0";
    }

    for (int i = mid+1; i < end; i++) {
        symbols[i].code += "1";
    }
    
    shannonFano(symbols, start, mid+1);
    shannonFano(symbols, mid+1, end);
}

pair<string, string> shannonFanoEncode(string input) {
    unordered_map<char, double> symbolProbs;
    for (char c : input) {
        if (symbolProbs.find(c) == symbolProbs.end()) {
            symbolProbs[c] = 0.0;
        }
        symbolProbs[c] += 1.0;
    }
    vector<Symbol> symbols;
    for (auto it : symbolProbs) {
        double prob = it.second / input.length();
        symbols.push_back(Symbol(it.first, prob));
    }
    sort(symbols.begin(), symbols.end(), symbolCompare);
    shannonFano(symbols, 0, symbols.size());
    unordered_map<char, string> codewords;
    string encodedOutput = "";
    string table = "Symbol\tProbability\tShannon-Fano Code\n";
    for (Symbol s : symbols) {
        codewords[s.symbol] = s.code;
        table += s.symbol;
        table += "\t";
        table += to_string(s.probability);
        table += "\t\t";
        table += s.code;
        table += "\n";
        for (char c : input) {
            if (c == s.symbol) {
                encodedOutput += s.code;
            }
        }
    }
    return make_pair(encodedOutput, table);
}

int main() {
    string input = "abracadabra";
    pair<string, string> result = shannonFanoEncode(input);
    
    cout << "Encoded Output: " << result.first << endl;
    cout << endl;
    cout << "Table:\n" << result.second << endl;
    
    return 0;
}
