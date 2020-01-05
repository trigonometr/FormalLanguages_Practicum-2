#include "../sources/Algo.h"
#include "tools.h"
#include "gtest/gtest.h"

class Tests : public ::testing::Test {
protected:
  Grammar grammar;
  Algo algo;
  vector<pair<char, string>> rules;
};

TEST_F(Tests, Test_1) {
  rules = {{'S', "SOS"}, {'S', "(S)"}, {'S', "0"},  {'S', "DN"}, {'O', "+"},
           {'O', "-"},   {'O', "*"},   {'O', "/"},  {'D', "1"},  {'D', "2"},
           {'D', "3"},   {'D', "4"},   {'D', "5"},  {'D', "6"},  {'D', "7"},
           {'D', "8"},   {'D', "9"},   {'N', "NN"}, {'N', ""},   {'N', "0"},
           {'N', "1"},   {'N', "2"},   {'N', "3"},  {'N', "4"},  {'N', "5"},
           {'N', "6"},   {'N', "7"},   {'N', "8"},  {'N', "9"}};
  grammar.ScanSingleLetterGrammar('S', rules);
  algo.SetGrammar(grammar);

  vector<pair<string, bool>> args = {
      {"0", true},
      {"+", false},
      {"0+", false},
      {"+0+", false},
      {"1+1", true},
      {"123456789987654321000000000001234567890000000", true},
      {"(0)", true},
      {"()", false},
      {")0(", false},
      {"(1+1*((2+2)*(321-333)/100/0/0/"
       "(34*(0+0+0+0+0+0+125689)-1234567890)+946132758*(((10))))*1110)",
       true},
      {"1+00", false},
      {"-1", false},
      {"(((1234+45*6520+(123)+((348*(3746)))))", false},
      {"(45++87)", false}};
  for (const auto &[word, correct_answer] : args) {
    EXPECT_EQ(correct_answer, algo.AnalyzeWord(word));
  }
}

TEST_F(Tests, Test_2) {
  rules = {{'S', "aSa"}, {'S', "bSb"}, {'S', "cSc"}, {'S', "a"},
           {'S', "b"},   {'S', "c"},   {'S', ""}};
  grammar.ScanSingleLetterGrammar('S', rules);
  algo.SetGrammar(grammar);

  vector<pair<string, bool>> args = {
      {"a", true},
      {"", true},
      {"aaaaaa", true},
      {"aaaaaaa", true},
      {"abcabccbacba", true},
      {"abc", false},
      {"acbabcbbccabbaccabcabcabcabacbacabcabacbacabcabacbacabacbacbacabcabacab"
       "cabcabacbaccabcabacbacbacabacbacabcabcabacabcabacbacabcabacbacabcabacba"
       "cbacbaccabbaccbbcbabca",
       true},
      {"acbabcbbccabbaccabcabcabcabacbacabcabacbacabcabacbacabacbacbacabcabacab"
       "cabcabacbaccabcabacbacbacabacbacabcabcabacabcabacbacabcabacbacabcсabacb"
       "acbacbaccabbaccbbcbabca",
       false},
      {"d", false},
      {"abcabacabacbacacacacabacabcabacabcabacabacbacabacbacacabcacabacababacac"
       "abacbacabacabacacabacabacabaccaccacaccabacabbbacacacaaaacabacbacabacabc"
       "abacababccbabacabacbacabacabcabacaaaacacacabbbacabaccacaccaccabacabacab"
       "acacabacabacabcabacacababacabacacbacacabcabacabcabacabacbacabacbacabaca"
       "cacacabcabacabacba",
       true},
      {"abcabacabacbacacacacabacabcabacabcabacabacbacabacbacacabcacabacababacac"
       "abacbacabacabacacabacabacabaccaccacaccabacabbbacacacaaaacabacbacabacabc"
       "abacababcacbabacabacbacabacabcabacaaaacacacabbbacabaccacaccaccabacabaca"
       "bacacabacabacabcabacacababacabacacbacacabcabacabcabacabacbacabacbacabac"
       "acacacabcabacabacba",
       true}};
  for (const auto &[word, correct_answer] : args) {
    EXPECT_EQ(correct_answer, algo.AnalyzeWord(word));
  }
}

TEST_F(Tests, Test_3) {
  rules = {{'S', "SSS"}, {'S', "TTT"}, {'S', "DDD"}, {'S', "AAA"}, {'S', "s"},
           {'T', "t"},   {'A', "a"},   {'D', "d"}};
  grammar.ScanSingleLetterGrammar('S', rules);
  algo.SetGrammar(grammar);

  vector<pair<string, bool>> args = {
      {"", false},
      {"s", true},
      {"ss", false},
      {"sss", true},
      {"aaaaaaaaa", true},
      {"aaaaaaaa", false},
      {"tttdddtttaaaaaatttdddtttddddddaaa", true},
      {"tttdddtttaaaaaatttdddtttdddddd", false},
      {"ssssssssssssssssssss", false},
      {"sssssssssssssssssss", true},
      {"ssssssssssssssssss", false},
      {"sssssssssssssssss", true},
      {"aaadddtta", false}
      };
  for (const auto &[word, correct_answer] : args) {
    EXPECT_EQ(correct_answer, algo.AnalyzeWord(word)) << Toolkit::GenerateTestInfo(word);
  }
}