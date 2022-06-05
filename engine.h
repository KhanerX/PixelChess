#include <bits/stdc++.h>
#include "pieces.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#define F first 
#define S second 

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
    ChessBoard* copy(ChessBoard* board){
        ChessBoard* ans = new ChessBoard();
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(board->state[i][j].is_occupied()){
                    string sq = board->state[i][j].occupant[0]->representation;
                    string col = "";
                    col.push_back(sq[1]);
                    if(sq[0] == 'P'){
                        Pawn* p = new Pawn(pii(i, j), col);
                        ans->place_piece(p);
                    }
                    else if(sq[0] == 'N'){
                        Knight* p = new Knight(pii(i, j), col);
                        ans->place_piece(p);
                    }
                    else if(sq[0] == 'K'){
                        King* p = new King(pii(i, j), col);
                        ans->place_piece(p);
                    }
                    else if(sq[0] == 'B'){
                        Bishop* p = new Bishop(pii(i, j), col);
                        ans->place_piece(p);
                    }
                    else if(sq[0] == 'R'){
                        Rook* p = new Rook(pii(i, j), col);
                        ans->place_piece(p);
                    }
                    else if(sq[0] == 'Q'){
                        Queen* p = new Queen(pii(i, j), col);
                        ans->place_piece(p);
                    } 
                }
            }
        }
        return this->calculate_threats(ans);
    }

    set<pii> sanitize(pii source, set<pii> moves, ChessBoard* board){
        
        set<pii> ans = {};
        string color = board->state[source.F][source.S].occupant[0]->color;
        for(pii move : moves){
            ChessBoard* test = this->copy(board);
            test = this->move(source, move, test);
            test = this->calculate_threats(test);
            if(!this->check(test, color[0]))
                ans.insert(move);   
        }
                
        return ans;
    }

    bool is_mate(ChessBoard* board, char color){
        if(!this->check(board, color))
            return false;

        for(Piece* p: board->pieces)
            if(p->color != string()+color && !this->sanitize(p->position, p->get_moves(board), board).empty()) 
                return false;

        return true;

    }

    bool is_critical(pii source, pii destination, ChessBoard* board){

        string color = board->state[source.F][source.S].occupant[0]->color;

        ChessBoard* test = this->copy(board);
        test = this->move(source, destination, test); //Do the move
        test = this->calculate_threats(test);
        test->print_state();
        cout << " First Move " << endl;
        
        for(Piece* op_piece : test->pieces){ 
            if(op_piece->color == color)
                continue;
            for(pii op_move : this->sanitize(op_piece->position, op_piece->get_moves(test), test)){ //Foreach opponent move
                bool is_mate = false;
                bool is_oc = false;
                Piece* upiece;
                if(test->state[op_move.F][op_move.S].is_occupied()){
                    is_oc = true;
                    upiece = test->state[op_move.F][op_move.S].occupant[0];
                }
                test = this->move(op_piece->position, op_move, test); //Do the opponent move
                test = this->calculate_threats(test);

                test->print_state();
                cout << " Opponent Move " << endl;
                
                for(Piece* my_piece: test->pieces){                   
                    if(my_piece->color != color)
                        continue;
                    for(pii my_move : this->sanitize(my_piece->position, op_piece->get_moves(test), test)){     //For each ally move
                        
                        
                        bool is_oc2 = false;
                        Piece* upiece2;
                        if(test->state[my_move.F][my_move.S].is_occupied()){
                            is_oc2 = true;
                            upiece2 = test->state[my_move.F][my_move.S].occupant[0];
                        }
                        pii pos = my_piece->position;
                        test = this->move(my_piece->position, my_move, test);
                        test = this->calculate_threats(test);
                        test->print_state();
                        cout << " 1 " << endl;
                        if(this->is_mate(test, negative[color][0])){  //If after the move opponent is checkmate
                            is_mate=true;
                        }
                        test = this->move(my_move, pos, test); //Undo move
                        if(is_oc2)
                            test->place_piece(upiece2);
                        test = this->calculate_threats(test);
                        test->print_state();
                        cout << "2" << endl;
                        if(is_mate)
                            break;
                    }
                    if(is_mate)
                        break;
                } //If loop ended without finding a check mate
                if(!is_mate){

                    return false; //there exists an opponent move that doesnt have immediate check mate;
                }
               
                test = this->move(op_move, op_piece->position, test); //Undo the move
                if(is_oc)
                    test->place_piece(upiece);
                test = this->calculate_threats(test);
            }
        }
        return true;
    }
    
};

      