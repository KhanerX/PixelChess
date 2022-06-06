#include <bits/stdc++.h>
#include "pieces.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#define F first 
#define S second 

class Engine {
    public:
    ChessBoard* test = new ChessBoard();
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
                if(board->pieces[i]->position == destination){
                    board->pieces.erase(board->pieces.begin() + i);
                }
        
        
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
   

    set<pii> sanitize(pii source, set<pii> moves, ChessBoard* board){
        
        set<pii> ans = {};
        string color = board->state[source.F][source.S].occupant[0]->color;
        cout << "HERE 2" << endl;
        for(pii move : moves){
            
            bool is_oc = false;
            Piece* upiece;
            if(board->state[move.F][move.S].is_occupied()){
                is_oc = true;
                upiece = board->state[move.F][move.S].occupant[0];
            }
            board = this->move(source, move, board);
            board = this->calculate_threats(board);

            if(!this->check(board, color[0]))
                ans.insert(move);

            board = this->move(move, source, board); //Undo the move
            if(is_oc)
                board->place_piece(upiece);
            board = this->calculate_threats(board);
        }
                
        return ans;
    }

    bool is_mate(ChessBoard* board, char color){
        if(!this->check(board, color))
            return false;

        for(Piece* p: board->pieces){
            if(p->color == string()+color && !this->sanitize(p->position, p->get_moves(board), board).empty()){
                return false;
            }
        }

        return true;

    }

    bool is_critical_type2(pii source, pii destination, ChessBoard* test){
        cout << "HERE" << endl;
        string color = test->state[source.F][source.S].occupant[0]->color;
        bool is_oc0 = false;
        Piece* upiece0;
        if(test->state[destination.F][destination.S].is_occupied()){
            is_oc0 = true;
            upiece0 = test->state[destination.F][destination.S].occupant[0];
        }
        
        pii init_pos = destination;
        test = this->move(source, destination, test); //Do the move
        test = this->calculate_threats(test);
        test->print_state();
        cout << " First Move " << endl;
        
        for(Piece* op_piece : test->pieces){ 
            if(op_piece->color == color)
                continue;
            for(pii op_move : this->sanitize(op_piece->position, op_piece->get_moves(test), test)){ //Foreach opponent move
                cout << "NEW MOVE: " << op_piece->representation << " " << op_move.F << " " << op_move.S<< endl;
                bool is_mate = false;
                bool is_oc = false;
                Piece* upiece;
                if(test->state[op_move.F][op_move.S].is_occupied()){
                    is_oc = true;
                    upiece = test->state[op_move.F][op_move.S].occupant[0];
                }
                pii op_pos = op_piece->position;
                test = this->move(op_piece->position, op_move, test); //Do the opponent move
                test = this->calculate_threats(test);

                test->print_state();
                cout << " Opponent Move " << endl;
                
                for(Piece* my_piece: test->pieces){                   
                    if(my_piece->color != color)
                        continue;
                    for(pii my_move : this->sanitize(my_piece->position, my_piece->get_moves(test), test)){     //For each ally move
                        
                        
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
                            cout << "THIS IS MATE" << endl;
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
                
               
                test = this->move(op_move, op_pos, test); //Undo the move
                if(is_oc)
                    test->place_piece(upiece);
                test = this->calculate_threats(test);

                test->print_state();
                cout << "Undo op move" << endl;

                if(!is_mate){
                    test = this->move(init_pos, source, test); //Undo the first move
                    if(is_oc0)
                        test->place_piece(upiece0);
                    test = this->calculate_threats(test);
                    return false; //there exists an opponent move that doesnt have immediate check mate;
                }

            }
        }
        test = this->move(init_pos, source, test); //Undo the first move
        if(is_oc0)
            test->place_piece(upiece0);
        test = this->calculate_threats(test);
        return true;
    }
};

