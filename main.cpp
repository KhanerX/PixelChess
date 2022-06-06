#include <bits/stdc++.h>
#include "pieces.h"
#include "render.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define F first 
#define S second 

int ChessBoard::num = 0;
int Piece::num = 0;

Rook* Queen::replace_rook = new Rook(pii(0, 0), "W");
Bishop* Queen::replace_bishop = new Bishop(pii(0, 0), "W");

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
    e.original_board = e.copy(board);

    sf::RenderWindow window(sf::VideoMode(722+90, 720), "Pixel Chess :3");

    RenderEngine re = RenderEngine{ &window, 4};
    pii selected = pii(-1, -1);
    int turn = 0;
    char turns[2] = {'W', 'B'};
    re.draw_board(board, selected, e); //Draw the current state to window;
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
                    
                    if(selected.F == -1 && board->state[i_click][j_click].is_occupied() && board->state[i_click][j_click].occupant[0]->color == string()+turns[turn%2]){
                        selected = pii(i_click, j_click);
                    }
                    else if(selected.F != -1){
                        pii source = selected;
                        pii destination = pii(i_click, j_click);
                        
                        set<pii> valid_moves = e.sanitize(source, board->state[source.F][source.S].occupant[0]->get_moves(board), board);
                        if(valid_moves.find(destination) != valid_moves.end()){
                            board = e.move(source, destination, board);
                            board = e.calculate_threats(board);
                            turn++;
                            selected = pii (-1, -1);
                            
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
                else if(mouse_pos.x > 720 && mouse_pos.y > 624 && mouse_pos.x < 720+88 && mouse_pos.y < 622+88){
                    board = e.copy(e.original_board);
                    pii selected = pii(-1, -1);
                    turn = 0;
                }
                else if(mouse_pos.x > 720 && mouse_pos.y > 624-88 && mouse_pos.x < 720+88 && mouse_pos.y < 622){
                    e.use_ai = !e.use_ai;
                }
                re.draw_board(board, selected, e); //Draw the current state to window;
            }
            
            
        }
        
    }

}