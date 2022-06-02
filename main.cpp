#include <bits/stdc++.h>
using namespace std;

#define F first 
#define S second 

typedef pair<int, int> pii;

map<string, string> negative = {{"B", "W"}, {"W", "B"}};
class ChessBoard;
class Piece {
    public:
    pair<int, int> position;
    string color;
    string representation;

    bool has_moved = false;

    Piece(pii init_position, string init_color){
        position = init_position;
        color = init_color;
        representation = ".";
    }

    virtual bool does_threaten(pii pos, ChessBoard* board){
        return false;
    }
    virtual set<pii> get_moves(ChessBoard* board){
        return {};
    }
};

class Square {
    public:
    vector<Piece*> occupant;
    set<string> threats;

    bool is_occupied(){
        return occupant.size() != 0;
    }
};

class ChessBoard {
    public:
    Square state[8][8];
    vector<Piece*> pieces;

    void place_piece(Piece* p){
        state[p->position.F][p->position.S].occupant.push_back(p);
        pieces.push_back(p);
    }

    void print_state(){
        for (int i = 7; i >= 0; i--){
            for(int j = 0; j < 8; j++){
                if(state[i][j].occupant.size() != 0)
                    cout << " " + state[i][j].occupant[0]->representation + " ";
                else{
                    if(true || state[i][j].threats.empty())
                        cout << " -- ";
                    else{
                        cout << " ";
                        for (string l : state[i][j].threats)
                            cout << l;
                        if (state[i][j].threats.size() == 1)
                            cout << "-";
                        cout << " ";
                    }
                }
            }
            cout << endl;
        }
    }

};

class Pawn : public Piece {
    public:
    Pawn(pii init_position, string init_color): Piece(init_position, init_color){
        representation = "P" + init_color;
    }
    
    set<pii> get_moves(ChessBoard* board){
        set<pii> ans = {};
        int dir = 1 + 2 * (-1 * int(color == "B"));

        if(position.F != 3.5 + dir * 3.5){
            if(position.S != 7 && board->state[position.F + dir][position.S + 1].is_occupied())
                if(board->state[position.F + dir][position.S + 1].occupant[0]->color != color)
                    ans.insert(pii(position.F + dir, position.S + 1));

            if(position.S != 0 && board->state[position.F + dir][position.S - 1].is_occupied())
                if(board->state[position.F + dir][position.S - 1].occupant[0]->color != color)
                    ans.insert(pii(position.F + dir, position.S - 1));
        }

        if(position.F == 3.5 + dir * 3.5 || board->state[position.F + dir][position.S].is_occupied())
            return ans;

        ans.insert(pii(position.F + dir, position.S));

        if(position.F == 3.5 - (dir * 2.5)&& !board->state[position.F + dir * 2][position.S].is_occupied())
            ans.insert(pii(position.F + dir * 2, position.S));

        return ans;
    }
    bool does_threaten(pii pos, ChessBoard* board){
        int dir = 1 + 2 * (-1 * int(color == "B"));
        if (pos.F - dir == position.F && abs(pos.S - position.S) == 1)
            return true;
        return false;
    }
};

class Knight : public Piece {
    public:
    Knight(pii init_position, string init_color): Piece(init_position, init_color){
        representation = "N" + init_color;
    }
    
    set<pii> get_moves(ChessBoard* board){
        set<pii> ans = {};
        int ix[8] = {1, 2,  2,  1, -1, -2, -2, -1};
        int iy[8] = {2, 1, -1, -2,  2,  1, -1, -2};

        for (int i = 0; i < 8; i++){
            int new_i = position.F + ix[i];
            int new_j = position.S + iy[i];
            if(0 <= new_i && new_i < 7 && 0 <= new_j && new_j < 7)
                if(!board->state[new_i][new_j].is_occupied() || board->state[new_i][new_j].occupant[0]->color != color)
                    ans.insert(pii(new_i, new_j));

        }
        return ans;
    }
    bool does_threaten(pii pos, ChessBoard* board){
        int ix[8] = {1, 2,  2,  1, -1, -2, -2, -1};
        int iy[8] = {2, 1, -1, -2,  2,  1, -1, -2};
        for (int i = 0; i < 8; i++)
            if (pos.F - ix[i] == position.F && pos.S - iy[i] == position.S)
                return true;
        return false;
    }
};

class King : public Piece {
    public:
    King(pii init_position, string init_color): Piece(init_position, init_color){
        representation = "K" + init_color;
    }
    
    set<pii> get_moves(ChessBoard* board){
        set<pii> ans = {};
        int ix[8] = {1, 1,  1, 0,  0, -1, -1, -1};
        int iy[8] = {1, 0, -1, 1, -1,  1,  0, -1};

        for (int i = 0; i < 8; i++){
            int new_i = position.F + ix[i];
            int new_j = position.S + iy[i];
            if(0 <= new_i && new_i <= 7 && 0 <= new_j && new_j <= 7 && board->state[new_i][new_j].threats.find(negative[color]) == board->state[new_i][new_j].threats.end())
                if(!board->state[new_i][new_j].is_occupied() || board->state[new_i][new_j].occupant[0]->color != color)
                    ans.insert(pii(new_i, new_j));
        }
        return ans;
    }

    bool does_threaten(pii pos, ChessBoard* board){
        int ix[8] = {1, 1,  1, 0,  0, -1, -1, -1};
        int iy[8] = {1, 0, -1, 1, -1,  1,  0, -1};
        for (int i = 0; i < 8; i++)
            if (pos.F - ix[i] == position.F && pos.S - iy[i] == position.S)
                return true;
        return false;
    }

};

class Rook : public Piece {
    public:
    Rook(pii init_position, string init_color): Piece(init_position, init_color){
        representation = "R" + init_color;
    }
    
    set<pii> get_moves(ChessBoard* board){
        set<pii> ans = {};

        for (int i = position.F + 1; i < 8; i++){
            if(board->state[i][position.S].is_occupied()){
                if (board->state[i][position.S].occupant[0]->color != color)
                    ans.insert(pii(i, position.S));
                break;
            }
            ans.insert(pii(i, position.S));
        }
        for (int i = position.S + 1; i < 8; i++){
            if(board->state[position.F][i].is_occupied()){
                if (board->state[position.F][i].occupant[0]->color != color)
                    ans.insert(pii(position.F, i));
                break;
            }
            ans.insert(pii(position.F, i));
        }
        for (int i = position.F - 1; i >= 0; i--){
            if(board->state[i][position.S].is_occupied()){
                if (board->state[i][position.S].occupant[0]->color != color)
                    ans.insert(pii(i, position.S));
                break;
            }
            ans.insert(pii(i, position.S));
        }
        for (int i = position.S - 1; i >= 0; i--){
            if(board->state[position.F][i].is_occupied()){
                if (board->state[position.F][i].occupant[0]->color != color)
                    ans.insert(pii(position.F, i));
                break;
            }
            ans.insert(pii(position.F, i));
        }
        return ans;
    }
    bool does_threaten(pii pos, ChessBoard* board){
        if(pos.F == position.F){
            for (int i = min(pos.S, position.S) + 1; i < max(pos.S, position.S); i++)
                if(board->state[pos.F][i].is_occupied())
                    return false;
            return true;
        }
        if(pos.S == position.S){
            for (int i = min(pos.F, position.F) + 1; i < max(pos.F, position.F); i++)
                if(board->state[i][pos.S].is_occupied())
                    return false;
            return true;
        }
        return false;
    }
};

class Bishop : public Piece {
    public:
    Bishop(pii init_position, string init_color): Piece(init_position, init_color){
        representation = "B" + init_color;
    }
    
    set<pii> get_moves(ChessBoard* board){
        set<pii> ans = {};
        for(int i = 1; i + max(position.F, position.S) < 8; i++){
            if(board->state[position.F + i][position.S + i].is_occupied()){
                if (board->state[position.F + i][position.S + i].occupant[0]->color != color)
                    ans.insert(pii(position.F + i, position.S + i));
                break;
            }
            ans.insert(pii(position.F + i, position.S + i));
        }
        for(int i = 1; i + position.F < 8 && position.S - i >= 0; i++){
            if(board->state[position.F + i][position.S - i].is_occupied()){
                if (board->state[position.F + i][position.S - i].occupant[0]->color != color)
                    ans.insert(pii(position.F + i, position.S - i));
                break;
            }
            ans.insert(pii(position.F + i, position.S - i));
        }
        for(int i = 1; i + position.S < 8 && position.F - i >= 0; i++){
            if(board->state[position.F - i][position.S + i].is_occupied()){
                if (board->state[position.F - i][position.S + i].occupant[0]->color != color)
                    ans.insert(pii(position.F - i, position.S + i));
                break;
            }
            ans.insert(pii(position.F - i, position.S + i));
        }
        for(int i = 1; min(position.F, position.S) - i >= 0; i++){
            if(board->state[position.F - i][position.S - i].is_occupied()){
                if (board->state[position.F - i][position.S - i].occupant[0]->color != color)
                    ans.insert(pii(position.F - i, position.S - i));
                break;
            }
            ans.insert(pii(position.F - i, position.S - i));
        }
        
        return ans;
    }
    bool does_threaten(pii pos, ChessBoard* board){
        if(abs(pos.F - position.F) != abs(pos.S - position.S))
            return false;
        if(pos.F == position.F)
            return false;
        if((pos.F > position.F && pos.S > position.S) || (pos.F < position.F && pos.S < position.S)){
            for(int i = 1; i + min(pos.F, position.F) < max(pos.F, position.F); i++)
                if(board->state[min(pos.F, position.F) + i][min(pos.S, position.S) + i].is_occupied())
                    return false;
            return true;
        }
        else{
            for(int i = 1; i + min(pos.F, position.F) < max(pos.F, position.F); i++)
                if(board->state[min(pos.F, position.F) + i][max(pos.S, position.S) - i].is_occupied())
                    return false;
            return true;
        }
    }
};

class Queen : public Piece {
        public:
    Queen(pii init_position, string init_color): Piece(init_position, init_color){
        representation = "Q" + init_color;
    }
    
    set<pii> get_moves(ChessBoard* board){
        set<pii> ans = {};
        Rook* replace_rook = new Rook(position, color);
        Bishop* replace_bishop = new Bishop(position, color);

        set<pii> rook_moves = replace_rook->get_moves(board);
        set<pii> bishop_moves = replace_bishop->get_moves(board); 
        
        set_union(rook_moves.begin(), rook_moves.end(), bishop_moves.begin(), bishop_moves.end(), inserter(ans, ans.begin()));
        
        return ans;
    }
    bool does_threaten(pii pos, ChessBoard* board){
        Rook* replace_rook = new Rook(position, color);
        Bishop* replace_bishop = new Bishop(position, color);
        
        return replace_rook->does_threaten(pos, board) | replace_bishop->does_threaten(pos, board);
    }
};

class Engine {
    public:
    ChessBoard* calculate_threats(ChessBoard* board){
        for (int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                set<string> threats;
                for (Piece* p : board->pieces){ 
                    if(p->does_threaten(pii(i, j), board)) 
                        threats.insert(p->color);
                }
                board->state[i][j].threats = threats;
            }
        }
        return board;
    }
    ChessBoard* move(pii source, pii destination, ChessBoard* board){
        if(board->state[destination.F][destination.S].is_occupied())
            for(int i = 0; i < board->pieces.size(); i++)
                if(board->pieces[i]->position == destination)
                    board->pieces.erase(board->pieces.begin() + i);
        
        for(int i = 0; i < board->pieces.size(); i++)
            if(board->pieces[i]->position == source)
                board->pieces[i]->position = destination;
        
        board->state[destination.F][destination.S].occupant = board->state[source.F][source.S].occupant;  
        board->state[source.F][source.S].occupant.pop_back();
        return board;
    }

    bool check(ChessBoard* board, char color){
        string col = "";
        col.push_back(color);
        for (int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(board->state[i][j].is_occupied() && board->state[i][j].occupant[0]->representation == "K" + col){
                    if(board->state[i][j].threats.find(negative[col]) != board->state[i][j].threats.end()){
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
};

int main(){

    ChessBoard* board = new ChessBoard();
    Engine e;
    for (int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            string sq = "";
            cin >> sq;
            string col = "";
            col.push_back(sq[1]);
            if(sq[0] == 'P'){
                Pawn* p = new Pawn(pii(i, j), col);
                board->place_piece(p);
            }
            else if(sq[0] == 'N'){
                Knight* p = new Knight(pii(i, j), col);
                board->place_piece(p);
            }
            else if(sq[0] == 'K'){
                King* p = new King(pii(i, j), col);
                board->place_piece(p);
            }
            else if(sq[0] == 'B'){
                Bishop* p = new Bishop(pii(i, j), col);
                board->place_piece(p);
            }
            else if(sq[0] == 'R'){
                Rook* p = new Rook(pii(i, j), col);
                board->place_piece(p);
            }
            else if(sq[0] == 'Q'){
                Queen* p = new Queen(pii(i, j), col);
                board->place_piece(p);
            }            
        }
    }
    board = e.calculate_threats(board);
    
    string cmd = "";
    int turn = 0;
    char turns[2] = {'W', 'B'};
    while (cin >> cmd){
        if(cmd == "fin"){
            if(turn % 2 == 0)
                cout << "black win" << endl;
            else
                cout << "white win" << endl;
            return 0;
        }
        if(cmd[3] != turns[turn % 2]){
            cout << "not your turn" << endl;
            continue;
        }
        pii source = pii(cmd[1] - '1', cmd[0]-'a');
        pii destination = pii(cmd[5] - '1', cmd[4]-'a');
        set<pii> valid_moves = board->state[source.F][source.S].occupant[0]->get_moves(board);

        if(valid_moves.find(destination) != valid_moves.end()){
            ChessBoard* backup = new ChessBoard(*board);
            board = e.move(source, destination, board);
            board = e.calculate_threats(board);
            if(!e.check(board, turns[turn % 2])){
                cout<< "done" << endl;
                turn++;
            }
            else{
                cout<< "try again check" << endl;
                board = backup;
                for(int i = 0; i < board->pieces.size(); i++)
                    if(board->pieces[i]->position == destination)
                        board->pieces[i]->position = source;
                continue;
            }
            
        }

}