#include<iostream>
using namespace std;
#include <string>
#include<fstream>
#include <vector>

void openFile(ifstream &inFile, string file_name);
void processFile(ifstream &inFile_grid, ifstream &inFile_words, vector<string> &grid,
         vector<string> &words, string line_grid, string line_words);
void check(vector<string> grid, Trie trie, int start_i, int start_j, int i, int j, 
            vector<pair<pair<int, int>, pair<int, int>>> &moves);

class Trie{
    class Node{
        public:
        Node* children[26];
        bool isEndOfWord;
        Node(){
            isEndOfWord = false;
            for(int i=0; i<26; i++)
                children[i] = NULL;
        }
        void setEndFalse(){
            isEndOfWord = false;
        }
    };
    public: Node* root;
    public:
    Trie(){
        root = new Node();
    }
    void insert(string word){
        Node* curr = root;
        for(int i=0; i<word.length(); i++){
            int index = word[i] - 'a';
            if(!curr->children[index])
                curr->children[index] = new Node();
            curr = curr->children[index];
        }
        curr->isEndOfWord = true;
        //cout<<word<<" is inserted"<<endl;
    }
    bool search (string word){
        Node* curr = root;
        for(int i=0; i<word.length(); i++){
            int index = word[i] - 'a';
            if(!curr->children[index])
                return false;
            curr = curr->children[index];
        }
        return curr->isEndOfWord;
    }
    bool startsWith(string word){
        Node* curr = root;
        for(int i=0; i<word.length(); i++){
            int index = word[i] - 'a';
            if(!curr->children[index])
                return false;
            curr = curr->children[index];
        }
        return true;
    }
    void deleteWord(string word){
        Node* curr = root;
        for(int i=0; i<word.length(); i++){
            int index = word[i] - 'a';
            if(!curr->children[index])
                return;
            curr = curr->children[index];
        }
        curr->isEndOfWord = false;
    }
    bool isChild(char ch){
        int index = ch - 'a';
        if(root->children[index]) return true;
        else return false;
    }

};

int main(){
    vector<string> grid;  
    vector<string> words;
    string line_grid =""; 
    string line_words="";
    
    // Creating file streams to read grid.txt and words.txt files
    ifstream inFile_grid;
    ifstream inFile_words;

    //opening files
    openFile(inFile_grid, "grid.txt");
    openFile(inFile_words, "words.txt");

    // Reading files
    processFile(inFile_grid, inFile_words, words, grid, line_grid, line_words);

    // creating object of class Trie
    Trie trie;
        
    // Inserting all words in the trie
    for(auto it: words){
        trie.insert(it);
    }

    // Vector to store the starting and ending coordinates of each word
    vector<pair<pair<int, int>, pair<int, int>>>moves;

    // For traversing all 8 directions
    int arrx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int arry[8] = {0, 1, 1, 1, 0, -1, -1, -1}; 

    // Checking each cell in the grid
    for(int i=0;i<grid.size();i++){
        for(int j=0;j<grid[0].size();j++){
            char ch = grid[i][j];
            // If it is present in the child of trie's root then continue checking
            if(trie.isChild(ch)){
                for(int k=0;k<8;k++){
                    int new_x = i + arrx[k];
                    int new_y = j + arry[k];
                    check(grid, trie, i, j, new_x, new_y, moves);
                }
            }
        }
    } 
}

// All the functions used are below: 
void openFile(ifstream &inFile, string file_name){
    inFile.open(file_name);
    if(!inFile.is_open()){
        cout<<"Error! Failed to open file" <<endl;
        exit(-1);
    }
    //else cout<<"File opened";
}
void processFile(ifstream &inFile_grid, ifstream &inFile_words, vector<string> &grid, 
        vector<string> &words, string line_grid, string line_words){
    while(getline(inFile_grid, line_grid) && getline(inFile_grid, line_words)){
        grid.push_back(line_grid);
        words.push_back(line_words);
    }
}

void check(vector<string> grid, Trie trie, int start_i, int start_j, int i, int j, 
            vector<pair<pair<int, int>, pair<int, int>>> &moves){
    int n = grid.size();
    int m = grid[0].size();
    string substring = "";
    start_i = i; start_j = j;
    auto node = trie.root;
    while(i>=0 && j>=0 && i<n && j<m && node->children[grid[i][j]]!=NULL){
        substring += grid[i][j];
        node = node->children[grid[i][j]];
        if(node->isEndOfWord){
            moves.push_back({{start_i, start_j}, {i, j}});
            node->setEndFalse();
        }
    }
}