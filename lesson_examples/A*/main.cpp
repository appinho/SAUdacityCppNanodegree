#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

bool Compare(const vector<int> & node1, const vector<int> & node2){
    return (node1[2] + node1[3]) < (node2[2] + node2[3]);
}

void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

bool CheckValidCell(int x, int y,
    const vector<vector<State>> & board){
    if(x >= 0 && x < board.size() && y >= 0 && y < board[0].size()){
        return board[x][y] == State::kEmpty || board[x][y] == State::kFinish;
    }
    else{
        return false;
    }
}

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

int Heuristic(int x1, int y1, int x2, int y2){
    return abs(x2-x1) + abs(y2-y1);
}

void AddToOpen(int x, int y, int g, int h,
    vector<vector<int>> & open,
    vector<vector<State>> & board){

    vector<int> new_node{x,y,g,h};
    open.push_back(new_node);
    board[x][y] = State::kClosed;
}

void ExpandNeighbors(const vector<int> & current_node,
    vector<vector<int>> & open,
    vector<vector<State>> & board,
    const vector<int> & goal){

    int c_x = current_node[0];
    int c_y = current_node[1];
    for(int i = 0; i < 4; i++){
        int n_x = delta[i][0] + c_x;
        int n_y = delta[i][1] + c_y;
        if(CheckValidCell(n_x, n_y, board)){
            int n_g = current_node[2] + 1;
            int n_h = Heuristic(n_x, n_y, goal[0], goal[1]);
            std::cout << n_x << " " << n_y << " " << n_g << " " << n_h << std::endl;
            AddToOpen(n_x, n_y, n_g, n_y, open, board);
        }
    }
}

vector<vector<State>> Search(std::vector<std::vector<State>> & board,
  const vector<int> & init, const vector<int> & goal){
    
    vector<vector<int>> open {};
    int h_start = Heuristic(init[0], init[1], goal[0], goal[1]);
    AddToOpen(init[0], init[1], 0, h_start, open, board);

    while(! open.empty()){
        CellSort(&open);
        vector<int> current_node = open.back();
        open.pop_back();
        int c_x = current_node[0];
        int c_y = current_node[1];
        board[c_x][c_y] = State::kPath;
        if(goal[0] == c_x && goal[1] == c_y){
            board[init[0]][init[1]] = State::kStart;
            board[goal[0]][goal[1]] = State::kFinish;
            return board;
        }
        ExpandNeighbors(current_node, open, board, goal);
    }
    std::cout << "No path found!" << std::endl;
    return vector<vector<State>>{};
}

string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   "; 
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}


int main() {
  
  // TODO: Declare "init" and "goal" arrays with values {0, 0} and {4, 5} respectively.
  vector<int> init{0,0};
  vector<int> goal{4,5};
  auto board = ReadBoardFile("1.board");
  // TODO: Call Search with "board", "init", and "goal". Store the results in the variable "solution".
  vector<vector<State>> solution = Search(board, init, goal);
  // TODO: Change the following line to pass "solution" to PrintBoard.
  PrintBoard(solution);
}
