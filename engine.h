#include <bits/stdc++.h>
#include "pieces.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

#define F first 
#define S second 

typedef pair<int, int> pii;

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