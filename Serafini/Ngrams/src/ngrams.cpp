// By Jackie Chan with aid from Tessera Chin and David Morales

// In Ngrams/Random Writer, the user inputs a file name, the number of word blocks (called N-grams), and the amount of words they wish
// to generate. The main function sets this up by first displaying the instructions, and then proceeding to read through the file
// while building up a map of N-grams. Once the entire map is populated, it is used to generate random text based on the input file
// which surprisingly sounds the same as the original text!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "strlib.h"
#include "simpio.h"
#include "map.h"
#include "random.h"

using namespace std;
void display_Instructions();
void read_FileAndBuildMap(int &N, string &key, string &value, Map<string, Vector<string>> &n_Grams, Vector<string> &split_key);
void get_N(int &N);
void first_Key(ifstream &stream, int const &N, string &word, string &key);
void update_Map(Map<string, Vector<string>> &n_Grams, string &key, string const &value);
void update_Window(Vector<string> &split_key, string &key);
void move_AndBuildMap(Map<string, Vector<string>> &n_Grams, string &key, string const &value, Vector<string> &split_key);
void initial_Output(Map<string, Vector<string>> const &n_Grams, string &key);
void build_Window(Map<string, Vector<string>> const &n_Grams, string &value, string &key);
void prompt_AmountWords(int &word_Amount, const int &N);
void generate_RandomText(Map<string, Vector<string>> const &n_Grams, string &key, string &output, string &value, Vector<string> &split_key, int const &word_Amount);

int main() {
    int N;
    int word_Amount;
    string key;
    string output;
    string value;
    Vector<string> split_key;
    Map<string, Vector<string>> n_Grams;

    display_Instructions();
    read_FileAndBuildMap(N,key,value,n_Grams,split_key);

    do{
        prompt_AmountWords(word_Amount,N);
        generate_RandomText(n_Grams,key,output,value,split_key, word_Amount);
        if(word_Amount == 0){
            break;
        }

    }while(true);
    cout << "Exiting." << endl;
    return 0;
}

// Helper Functions

// Displays directions for the user.

void display_Instructions(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;
}

// Prompts user for input (file name and N) and builds the map by calling the move_AndBuildMap function. This also takes care of
// wrapping around the text by storing the initial window as a string and then putting it in the map when the window is at the end.

void read_FileAndBuildMap(int &N, string &key, string &value, Map<string, Vector<string>> &n_Grams, Vector<string> &split_key){
    string filename;
    string word;
    string wrapped_Words;
    ifstream stream;

    filename = promptUserForFile(stream, "Input file name? ");
    get_N(N);
    first_Key(stream,N,word,key);
    wrapped_Words = key; //creates string that will help associate the last words to the first ones in the file

    while(stream >> word){
        value = word;
        move_AndBuildMap(n_Grams,key,value,split_key);
    }

    //to handle words that are wrapped around
    for(int i = 0; i < N - 1; i++){
        value = stringSplit(wrapped_Words, " ")[i];
        move_AndBuildMap(n_Grams,key,value,split_key);
    }
    stream.close();
}

// Prompts user for the integer N, which determines how large each N-gram will be.

void get_N(int &N){

    N = getInteger("Value of N? ", "Illegal integer format. Try again.");
    while(N < 2){
        cout << "N must be 2 or greater." << endl;
        N = getInteger("Value of N? ", "Illegal integer format. Try again.");
    }
}

// Basically, a fencepost in which the initial key is created from the window.

void first_Key(ifstream &stream, int const &N, string &word, string &key){
    for(int i = 0; i < N - 1; i++){
        stream >> word;
        key += word + " ";
    }
    key = trimEnd(key);
}

// Checks the map if the current window (key) has been seen before. If this is a new window, a new key-value pair is put into the map. If
// not, then the value (the word following the key) is appended to the end of values vector.

void update_Map(Map<string, Vector<string>> &n_Grams, string &key, string const &value){
    if(n_Grams.containsKey(key)){
        Vector<string> values = n_Grams.get(key);
        values.add(value);
        n_Grams.put(key,values);
    }else{
        Vector<string> values;
        values.add(value);
        n_Grams.put(key,values);
    }
}

// This ensures that the window is always N-1 words long by removing the first word after appending the value. This is done by converting
// the string to a vector with split_key, and then stitching it back together into a string.

void update_Window(Vector<string> &split_key, string &key){
    split_key = stringSplit(key, " ");
    split_key.remove(0);
    key = "";

    for(int i = 0; i < split_key.size(); i++){
        key += split_key[i] + " ";
    }
    key = trimEnd(key);
}

// Updates the map and window and appends the suffix (value) to the prefix (key).

void move_AndBuildMap(Map<string, Vector<string>> &n_Grams, string &key, string const &value, Vector<string> &split_key){
    update_Map(n_Grams,key,value);
    update_Window(split_key,key);
    key += " " + value;
}

// The first key is randomly selected from the vector of keys stored in map_Keys.

void initial_Output(Map<string, Vector<string>> const &n_Grams, string &key){
    int get_RandomKey;
    Vector<string> map_Keys;

    map_Keys = n_Grams.keys();
    get_RandomKey = randomInteger(0, map_Keys.size() - 1);
    key = map_Keys[get_RandomKey];
}

// A random value associated to the key is selected and appended at the end of the key.

void build_Window(Map<string, Vector<string>> const &n_Grams, string &value, string &key){
    int random_Value;
    Vector<string> random_ValueVector;

    random_ValueVector = n_Grams.get(key);
    random_Value = randomInteger(0, random_ValueVector.size() - 1);
    value = random_ValueVector[random_Value];
    key += " " + value;
}

// Prompts the user for the desired amount of words outputted.

void prompt_AmountWords(int &word_Amount, int const &N){
    cout << "" << endl;
    word_Amount = getInteger("# of random words to generate (0 to quit)? ", "Illegal integer format. Try again.");
    while(word_Amount < N && word_Amount != 0){
        cout << "Must be at least " << N << " words." << endl;
        cout << "" << endl;
        word_Amount = getInteger("# of random words to generate (0 to quit)? ", "Illegal integer format. Try again.");
    }
}

// The primary function responsible for generating the random text, which calls the helper functions described above.

void generate_RandomText(Map<string, Vector<string>> const &n_Grams, string &key, string &output, string &value, Vector<string> &split_key, int const &word_Amount){
    initial_Output(n_Grams,key);
    output = key;

    for(int i = stringSplit(key, " ").size(); i < word_Amount; i++){
        build_Window(n_Grams,value,key);
        output += " " + value;
        update_Window(split_key,key);
    }
    if(word_Amount != 0){
        cout << "... " << output << " ..." << endl;
    }
}
