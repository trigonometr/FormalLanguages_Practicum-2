#include "Algo.h"


bool Symbol::IsTerminal() const { return is_terminal_; }




NonTerminalSymbol::NonTerminalSymbol(string str) : data_(std::move(str)) {
  is_terminal_ = false;
}




bool NonTerminalSymbol::operator==(const NonTerminalSymbol &another) const {
  return this->data_ == another.data_;
}

size_t
NonTerminalSymbol::Hash::operator()(const NonTerminalSymbol &symbol) const {
  return string_hash_(symbol.data_);
}




TerminalSymbol::TerminalSymbol(char data) : data_(data) { is_terminal_ = true; }




SymbolString::SymbolString(const NonTerminalSymbol &symbol) {
  symbols_.push_back(std::make_shared<NonTerminalSymbol>(symbol));
}
bool SymbolString::operator==(const SymbolString &another) const {
  if (this->symbols_.size() != another.symbols_.size()) {
    return false;
  }

  for (uint32_t i = 0; i < this->symbols_.size(); ++i) {
    if (this->symbols_[i] != another.symbols_[i])
      return false;
  }

  return true;
}

size_t SymbolString::Hash::operator()(const SymbolString &symbol_string) const {
  size_t result = 0;
  for (const auto &symbol : symbol_string.symbols_) {
    result += shared_ptr_hash_(symbol);
  }
  return result;
}




void Grammar::ScanSingleLetterGrammar(char start,
                                      const vector<pair<char, string>> &rules) {
  rules_.clear();
  start_ = string(1, start);
  for (const auto &rule : rules) {
    SymbolString symbol_string;
    for (char ch : rule.second) {
      if (ch >= 'A' && ch <= 'Z') {
        symbol_string.symbols_.push_back(
            std::make_shared<NonTerminalSymbol>(string(1, ch)));
      }
      if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') ||
          (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' ||
           ch == ')')) {
        symbol_string.symbols_.push_back(std::make_shared<TerminalSymbol>(ch));
      }
    }

    rules_.insert({string(1, rule.first), symbol_string});
  }
}




void Algo::SetGrammar(Grammar grammar) {
  grammar_ = std::move(grammar);
  NonTerminalSymbol prev_start = grammar_.start_;
  begin_rule_ = {NonTerminalSymbol("START"), SymbolString(prev_start)};
  grammar_.start_ = begin_rule_.first;
  grammar_.rules_.insert(begin_rule_);
}
bool Algo::AnalyzeWord(string_view word) {
  states_.clear();
  states_.resize(word.size() + 1);
  states_[0].insert({0, 0, begin_rule_});

  for (uint32_t current_word_letter = 0; current_word_letter <= word.size();
       ++current_word_letter) {
    Scan(word, current_word_letter);
    bool was_changed;
    do {
      was_changed = false;
      Complete(word, current_word_letter, &was_changed);
      Predict(word, current_word_letter, &was_changed);
    } while (was_changed);
  }

  return !(states_[word.size()].find({1, 0, begin_rule_}) ==
           states_[word.size()].end());
}
void Algo::Scan(string_view word, uint32_t current_letter) {
  if (current_letter == 0) {
    return;
  }

  for (const auto &state : states_[current_letter - 1]) {
    if (state.rule_.second.symbols_.size() > state.dot_position_ &&
        state.rule_.second.symbols_[state.dot_position_]->IsTerminal() &&
        std::dynamic_pointer_cast<TerminalSymbol>(
            state.rule_.second.symbols_[state.dot_position_])
                ->data_ == word[current_letter - 1]) {
      states_[current_letter].insert(
          {state.dot_position_ + 1, state.state_ref_, state.rule_});
    }
  }
}
void Algo::Predict(string_view word, uint32_t current_letter,
                   bool *was_changed) {
  for (const auto &state : states_[current_letter]) {
    if (state.rule_.second.symbols_.size() > state.dot_position_ &&
        !state.rule_.second.symbols_[state.dot_position_]->IsTerminal()) {
      const auto &[begin_rule, end_rule] = grammar_.rules_.equal_range(
          *std::dynamic_pointer_cast<NonTerminalSymbol>(
              state.rule_.second.symbols_[state.dot_position_]));
      for (auto rule = begin_rule; rule != end_rule; ++rule) {
        State new_state = {0, current_letter, {rule->first, rule->second}};
        if (states_[current_letter].find(new_state) ==
            states_[current_letter].end()) {
          states_[current_letter].insert(new_state);
          *was_changed = true;
        }
      }
    }
  }
}
void Algo::Complete(string_view word, uint32_t current_letter,
                    bool *was_changed) {
  for (const auto &completed_state : states_[current_letter]) {
    if (completed_state.rule_.second.symbols_.size() ==
        completed_state.dot_position_) {
      for (const auto &state : states_[completed_state.state_ref_]) {
        if (state.rule_.second.symbols_.size() > state.dot_position_ &&
            !state.rule_.second.symbols_[state.dot_position_]->IsTerminal() &&
            *std::dynamic_pointer_cast<NonTerminalSymbol>(
                state.rule_.second.symbols_[state.dot_position_]) ==
                completed_state.rule_.first) {
          State new_state = {state.dot_position_ + 1, state.state_ref_,
                             state.rule_};
          if (states_[current_letter].find(new_state) ==
              states_[current_letter].end()) {
            states_[current_letter].insert(new_state);
            *was_changed = true;
          }
        }
      }
    }
  }
}
bool Algo::State::operator==(const Algo::State &another) const {
  return this->dot_position_ == another.dot_position_ &&
         this->state_ref_ == another.state_ref_ && this->rule_ == another.rule_;
}
size_t Algo::State::Hash::operator()(const Algo::State &state) const {
  return size_t_hash_(state.dot_position_) + size_t_hash_(state.state_ref_) +
         non_terminal_hash_(state.rule_.first) +
         symbol_string_hash_(state.rule_.second);
}
