#include "../sources/Algo.h"
#include <iostream>
#include <string>

int main() {
  Algo algo;
  Grammar grammar;
  vector<pair<char, string>> rules;
  std::string right;
  char left;
  while (true) {
    std::cin >> left >> right;
    if (left == '0' && right == "0") {
      break;
    }
    rules.emplace_back(left, right);
  }
  grammar.ScanSingleLetterGrammar('S', rules);
  algo.SetGrammar(grammar);

  std::string word;
  while (true) {
    std::cin >> word;
    if (word == "0") {
      break;
    }
    std::cout << algo.AnalyzeWord(word) << std::endl;
  }

  return 0;
}