#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using std::vector, std::pair, std::string, std::string_view, std::unordered_set,
    std::unordered_multimap, std::shared_ptr;

struct Symbol {
public:
  virtual ~Symbol() = default;
  [[nodiscard]] bool IsTerminal() const;

protected:
  bool is_terminal_;
};

struct NonTerminalSymbol : public Symbol {
  NonTerminalSymbol(string str = string());
  string data_;

  bool operator==(const NonTerminalSymbol &another) const;

  class Hash {
  public:
    size_t operator()(const NonTerminalSymbol &symbol) const;
  private:
    std::hash<string> string_hash_;
  };
};

struct TerminalSymbol : public Symbol {
  TerminalSymbol(char data = '\0');
  ;
  char data_;
};

struct SymbolString {
public:
  SymbolString() = default;
  explicit SymbolString(const NonTerminalSymbol &symbol);
  bool operator==(const SymbolString &another) const;

  class Hash {
  public:
    size_t operator()(const SymbolString &symbol_string) const;
  private:
    std::hash<shared_ptr<Symbol>> shared_ptr_hash_;
  };

  vector<shared_ptr<Symbol>> symbols_;
};

struct Grammar {
  void ScanSingleLetterGrammar(char start,
                               const vector<pair<char, string>> &rules);
  NonTerminalSymbol start_;
  unordered_multimap<NonTerminalSymbol, SymbolString, NonTerminalSymbol::Hash>
      rules_;
};

class Algo {
public:
  void SetGrammar(Grammar grammar);
  bool AnalyzeWord(string_view word);

private:
  struct State {
    uint32_t dot_position_ = 0;
    uint32_t state_ref_ = 0;
    pair<NonTerminalSymbol, SymbolString> rule_;

    bool operator==(const State &another) const;

    class Hash {
    public:
      size_t operator()(const State &state) const;
    private:
      NonTerminalSymbol::Hash non_terminal_hash_;
      SymbolString::Hash symbol_string_hash_;
      std::hash<size_t> size_t_hash_;
    };
  };

  Grammar grammar_;
  vector<unordered_set<State, State::Hash>> states_;
  pair<NonTerminalSymbol, SymbolString> begin_rule_;

  void Scan(string_view word, uint32_t current_letter);
  void Predict(string_view word, uint32_t current_letter, bool *was_changed);
  void Complete(string_view word, uint32_t current_letter, bool *was_changed);
};