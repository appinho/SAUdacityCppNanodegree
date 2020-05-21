#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State { kEmpty, kObstacle };

// TODO: Change the return type to use auto or
// explicitly indicate vector<State>
vector<State> ParseLine(string line) {
  istringstream sline(line);
  int n;
  char c;
  // TODO: Change the variable type for row
  // to be a vector<State>
  vector<State> row;
  while (sline >> n >> c && c == ',') {
    // TODO: Modify the line below to push_back
    // a State::kEmpty if n is 0, and push_back
    // a State::kObstacle otherwise.
    if (n == 0) {
      row.push_back(State::kEmpty);
    } else {
      row.push_back(State::kObstacle);
    }
  }
  return row;
}

// TODO: Modify the return type here as well. Just
// as above, the board will contain State objects
// instead of ints.
vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile(path);
  // TODO: Modify the board declaration to store
  // State objects instead of ints.
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      // TODO: Modify the row type to use State
      // objects instead of ints.
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

// TODO: Modify the function signature below to accept
// cells with State type instead of int type.
string CellString(State cell) {
  switch (cell) {
  case State::kObstacle:
    return "⛰️   ";
  default:
    return "0   ";
  }
}

void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      // TODO: Modify the line below to call CellString
      // on each element of the board before streaming to cout.
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

int main() {
  auto board = ReadBoardFile("1.board");
  PrintBoard(board);
}