#ifndef SHANNON_ENCODER_H
#define SHANNON_ENCODER_H

#include "tree.h"
#include "utils.h"
#include <string>
#include <algorithm>
#include <cmath>

Node* getProbTable(FILE* file);
std::map<char, std::string>* getCodeTable(Node* symbolData);
void bitEncode(FILE *f0, FILE *f1, Node *headTree, std::map<char, std::string> *codeTable);
unsigned long getCompressedDataSize(Node* headTree, std::map<char, std::string>* codeTable);

#endif
