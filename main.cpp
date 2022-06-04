#include <bits/stdc++.h>
#include "pieces.h"
#include "render.h"
#include "engine.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

#define F first 
#define S second 

typedef pair<int, int> pii;


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

    sf::RenderWindow window(sf::VideoMode(720, 720), "Pixel Chess :3");
    RenderEngine re = RenderEngine{ &window, 4};
    pii selected = pii(-1, -1);
    int turn = 0;
    char turns[2] = {'W', 'B'};
    while (window.isOpen()) // Game Loop
    {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)//Check for window exit
                window.close();
            
        

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                if (mouse_pos.x > 8 && mouse_pos.y > 8 && mouse_pos.x < 712 && mouse_pos.y < 712){
                    int i_click = 7 - ((mouse_pos.y - 8) / 88);
                    int j_click = (mouse_pos.x - 8) / 88;

                    if(selected.F == -1 && board->state[i_click][j_click].is_occupied() && board->state[i_click][j_click].occupant[0]->color == string()+turns[turn%2])
                        selected = pii(i_click, j_click);

                    else if(selected.F != -1){
                        pii source = selected;
                        pii destination = pii(i_click, j_click);

                        set<pii> valid_moves = board->state[source.F][source.S].occupant[0]->get_moves(board);
                        if(valid_moves.find(destination) != valid_moves.end()){
                            ChessBoard* backup = new ChessBoard(*board);
                            board = e.move(source, destination, board);
                            board = e.calculate_threats(board);
                            if(!e.check(board, turns[turn % 2])){
                                cout<< "done" << endl;
                                turn++;
                                selected = pii (-1, -1);
                            }
                            else{
                                cout<< "try again check" << endl;
                                board = backup;
                                for(int i = 0; i < board->pieces.size(); i++)
                                    if(board->pieces[i]->position == destination)
                                        board->pieces[i]->position = source;

                                selected = pii (-1, -1);
                                continue;
                            }
                            
                        }
                        else{
                            if(board->state[i_click][j_click].is_occupied() && board->state[i_click][j_click].occupant[0]->color == string()+turns[turn%2])
                                selected = pii(i_click, j_click);
                            else
                                selected = pii (-1, -1);
                            cout<< "try again" << endl;
                        }
                        
                    }
                }
            }
            //cout << selected.F << " " << selected.S << endl;
        }
        re.draw_board(board, selected); //Draw the current state to window;
        
        /*string cmd = "";
        int turn = 0;
        char turns[2] = {'W', 'B'};
        cin >> cmd;
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
        else{
            cout<< "try again" << endl;
        }*/
        
    }

}