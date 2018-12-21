/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Jackie Chan and Ike Osafo
 * Section leader: Jordan Rosen-Kaplan
 * This file contains grammar generating code for CS106B. It takes in user input regarding the text file, symbol, and number of times
 * to generate it.
 */

#include "grammarsolver.h"
#include "vector.h"
#include "map.h"
#include "filelib.h"
#include <iostream>
#include <fstream>
#include "strlib.h"
#include "random.h"
#include <string>

using namespace std;

Vector<string> grammarGenerate(istream& input, string symbol, int times);
void create_Map(istream &input, Map<string, Vector<string>> &my_map);
Vector<string> grammarGenerate(const Map<string, Vector<string>> &my_map, string symbol, int times);
string grammarGenerate(const Map<string, Vector<string>> &my_map, string symbol);

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */

// This function creates the map, calls grammarGenerate, and returns its Vector.

Vector<string> grammarGenerate(istream& input, string symbol, int times){
    Map<string, Vector<string>> my_map;
    if(trim(symbol) == ""){
        throw("Please enter a valid symbol.");
    }
    create_Map(input,my_map);
    Vector<string> to_return = grammarGenerate(my_map,symbol,times);
    return to_return;
}

// Responsible for reading in the map. Keys are the symbols to the left of the delimiter "::=" while the values are those to the
// right of it. The values are further broken up by the delimiter "|" such that only one string in the vector is chosen.

void create_Map(istream &input, Map<string, Vector<string>> &my_map){
    string line;
    Vector<string> split;

    while(getline(input,line)){
        split = stringSplit(line,"::=");
        string key = split[0]; // extracts key
        if(my_map.containsKey((key))){
           string duplicate;
           duplicate = "The file contains a duplicate of " + key;
           throw(duplicate);
        }
        split.remove(0); // removes key so value remains
        string rest = split[0]; // rest is now the string of the value vector
        split = stringSplit(rest,"|"); // splits such that the vector now contains multiple words connected by '|' previously
        my_map.put(key,split);
    }
}


//1. uses overloaded functions
//2. first overload type calls the second which is recursive
//3. The recursive calls the checker to see if a delimiter still exists in a key-value pair

Vector<string> grammarGenerate(const Map<string, Vector<string>> &my_map, string symbol, int times){
    Vector<string> to_print;
    string to_store;
    while(times > 0){
        to_store = grammarGenerate(my_map, symbol);//get the full sentence from the recursive func
        to_store = trim(to_store); //trims the last space around the string concatenation
        to_print.add(to_store); //add it to the vector to be printed
        times --; // decreases times (the counter) by one
    }
    return to_print;
}

// Recursive function takes in symbol as key and randomly chooses a string from the vector mapped to it. The base case is where the
// symbol is not one of the keys, meaning it is terminal. The recursion generates sentences for two cases - for non-terminal symbols
// separated by " " and for those initially separated by "|" (i.e. <n><pn>).

string grammarGenerate(const Map<string, Vector<string>> &my_map, string symbol){
    if(!my_map.containsKey(symbol)){ //checks the false base case when the symbol is not in the map
        return symbol; //base case, just returns the symbol
    }else{
        string sentence = ""; // this string will be concatenated to construct the sentence
        Vector<string> selects = my_map.get(symbol); //get the values corresponding to that non-terminal value
        int rnd_num = randomInteger(0, selects.size() - 1);
        string search_str = selects[rnd_num];

        if(stringContains(search_str, " ")){ // if there is a space, perform grammarGenerate on each non-terminal and combine their outputs
            Vector<string> an_split = stringSplit(search_str, " ");
            for(string symb: an_split){
                sentence += trim(grammarGenerate(my_map, symb)) + " ";
            }
            return sentence;
        }else{
            sentence += trim(grammarGenerate(my_map, search_str)) + " "; // handles the symbols/text initially separated by "|"
            return sentence;
        }
    }
}

