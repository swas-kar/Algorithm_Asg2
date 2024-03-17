#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;

    Node(char data, int frequency) {
        this->data = data;
        this->frequency = frequency;
        left = right = nullptr;
    }

    Node(int frequency, Node* left, Node* right) {
        data = '\0';
        this->frequency = frequency;
        this->left = left;
        this->right = right;
    }
};

struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->frequency > right->frequency;
    }
};

unordered_map<char, string> codes;

void generateCodes(Node* root, string code) {
    if (root == nullptr) {
        return;
    }
    if (root->data != '\0') {
        codes[root->data] = code;
    }
    generateCodes(root->left, code + "0");
    generateCodes(root->right, code + "1");
}

string encode(string text) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    unordered_map<char, int> freq;

    for (char c : text) {
        freq[c]++;
    }

    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        int sum = left->frequency + right->frequency;
        pq.push(new Node(sum, left, right));
    }

    Node* root = pq.top();
    pq.pop();
    generateCodes(root, "");
    string encoded = "";

    for (char c : text) {
        if(c == ' ')
            encoded += " ";
        encoded += codes[c];
    }
    
    return encoded;
}

string decode(string encoded) {
    cout<<"inside decoded";
    string decoded = "";
    Node* current = new Node('\0', 0);
    cout<<"\nStill inside";
    for (char c : encoded) {
        if (c == '0') {
            current = current->left;
        } 
        else {
            current = current->right;
        }
        if (current->data != '\0') {
            decoded += current->data;
            current = new Node('\0', 0);
        }
    }
    cout<<"outside for loop";
    cout<<decoded;
    return decoded;
}

string makeTable(){
    string table;
    for(auto pair: codes){
        table += (pair.first + " " + pair.second);
        table += "\n";
    }
    return table;
}

int main() {
    FILE* file;
    FILE* file2;
    file = fopen("file2.txt", "r");
    char ch;
    string text;
    while ((ch=getc(file))!=EOF) {
      text=text+ch;
   }
    string encoded = encode(text);
    cout << "Encoded: " << encoded << endl;
    string table = makeTable();
    
    ofstream output("Encoded2.txt");
    output<<encoded;
    output<<"\n";
    output<<table;
    output.close();
    
    return 0;
}
