// By Jackie Chan with help from Tessera Chin and David Morales.

// Word Ladder takes in two words and transforms one into the other by changing only one letter at a time. Furthermore, it does this in
// the most efficient/shortest path possible.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "strlib.h"
#include "lexicon.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h"

using namespace std;
void display();
void word_Ladder(const Lexicon &file, const string &word1, const string &word2);
void display_Ladder(string const &word1, string const &word2, Stack<string> &words, string &top_Word, string &ladder);
void assess_Input(string const &word1, string const &word2, Lexicon const &file);
void initialize_Queue(string const &word1, Stack<string> &words, Queue<Stack<string>> &queue, Set<string> &used_Words);
void copy_Stack(Stack<string> const &words, string const &top_Word, string const &temp, Stack<string> &copy, Queue<Stack<string>> &queue);

int main() {
    display();
    cout << "Have a nice day." << endl;
    return 0;
}

// This function first displays the instructions then subsequently reads in a dictionary and stores it in a Lexicon. It then takes in
// the two inputted words from the user and assesses them.

void display(){
    ifstream stream;
    string s;
    string word1;
    string word2;
    Lexicon file;

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "" << endl;

    s = promptUserForFile(stream, "Dictionary file name? ");
    stream.close();
    cout << "" << endl;

    file = Lexicon(s);
    do{
        word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if(word1 == ""){
            break;
        }
        word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if(word2 == ""){
            break;
        }
        assess_Input(word1, word2, file);
    }while(true);
}

// This function performs most of the work by first constructing the queue. It then takes in the current word, modifies each letter
// by replacing it with each letter in the alphabet. If the substitution results in a new and valid word, it is added to the set.
// If the modified word is not the second word, it is added to the top of copy (a stack). If the modification produces the second word,
// then display_Ladder is called.

void word_Ladder(Lexicon const &file, string const &word1, string const &word2){
    Stack<string> words;
    Queue<Stack<string>> queue;
    string top_Word;
    Set<string> used_Words;
    string ladder;
    Stack<string> copy;
    string temp;

    initialize_Queue(word1, words, queue, used_Words);

    while(!queue.isEmpty()){
        words = queue.dequeue();
        top_Word = words.pop();

        for(size_t i = 0; i < top_Word.size(); i++){
            for(char j = 'a'; j <= 'z'; j++){
                temp = top_Word;
                temp[i] = j;

                if(file.contains(temp) && !used_Words.contains(temp)){
                    used_Words.add(temp);

                    if(temp != word2){
                        copy_Stack(words, top_Word, temp, copy, queue);
                    }else{
                        words.push(top_Word);
                        words.push(temp);
                        display_Ladder(word1, word2, words, top_Word, ladder);
                    }
                }
            }
        }
    }if(ladder == ""){
        cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
    }
}

// This function is called when the second word is finally achieved. It prints out the word ladder connecting the two words for the user.

void display_Ladder(string const &word1, string const &word2, Stack<string> &words, string &top_Word, string &ladder){
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    while(!words.isEmpty()){
        top_Word = words.pop();
        ladder += top_Word + " ";
    }
    cout << ladder << endl;
}

// This function creates a copy of the words Stack, appends the top word and its modification, and enqueues it.

void copy_Stack(Stack<string> const &words, string const &top_Word, string const &temp, Stack<string> &copy, Queue<Stack<string>> &queue){
    copy = words;
    copy.push(top_Word);
    copy.push(temp);
    queue.enqueue(copy);
}

// Takes in user input and if valid, calls the word_Ladder function.

void assess_Input(string const &word1, string const &word2, Lexicon const &file){
    if(word1.length() != word2.length()){
        cout << "The two words must be the same length." << endl;
    }else if(!file.contains(word1) || !file.contains(word2)){
        cout << "The two words must be in the dictionary." << endl;
    }else if(word1 == word2){
        cout << "The two words must be different." << endl;
    }else{
        word_Ladder(file, word1, word2);
    }
    cout << "" << endl;
}

// Fencepost which initializes the queue using the first word.

void initialize_Queue(string const &word1, Stack<string> &words, Queue<Stack<string>> &queue, Set<string> &used_Words){
    words.push(word1);
    queue.enqueue(words);
    used_Words.add(word1);
}
