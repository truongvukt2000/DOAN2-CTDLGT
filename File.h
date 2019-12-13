#pragma once
#include "HuffmanTree.h"
#include <bitset>

vector<int> findchar(int nRoot);
string fix_bit_end(string compressed, int& count);
void Compress(string inFile, string outFile);
string convert8bit(char s);
void extract(string inFile, string outFile);
