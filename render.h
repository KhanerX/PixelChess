#include <bits/stdc++.h>
#include "pieces.h"
#include "engine.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#define F first 
#define S second 

class RenderEngine{
    public:

        sf::RenderWindow* window;
        int scale = 4; 

        void place_sprite(string address, pii position){
            sf::Texture texture;
            if (!texture.loadFromFile(address))
            {
                cout << "help";
            }
            sf::Sprite sprite;
            sprite.setScale(scale, scale);
            sprite.setTexture(texture);
            sprite.setPosition(position.F, position.S);

            window->draw(sprite);
        }

        void draw_board(ChessBoard* board, pii selected, Engine e){
            window->clear(sf::Color::Black);

            place_sprite("graphics/board.png", pii(0, 0));
            set<pii> valid_moves;
            set<pii> type2_critical_moves;
            if(selected.F != -1){
                pii selected_pos = pii(8 + selected.S * 88, 8 + (7 - selected.F) * 88);
                place_sprite("graphics/select.png", selected_pos);
                valid_moves = e.sanitize(selected, board->state[selected.F][selected.S].occupant[0]->get_moves(board), board);
                
            }
            for (int i = 7; i >= 0; i--){
                for(int j = 0; j < 8; j++){
                    pii sprite_pos = pii(8 + j * 88, 8 + (7 - i) * 88);
                    if(board->state[i][j].is_occupied() && board->state[i][j].occupant[0]->representation[0] == 'K')
                        if(e.check(board, board->state[i][j].occupant[0]->color[0]))
                            place_sprite("graphics/check.png", sprite_pos);

                    if(valid_moves.find(pii(i, j)) != valid_moves.end()){
                        place_sprite("graphics/move.png", sprite_pos);
                        if(e.is_critical_type2(pii(selected.F, selected.S), pii(i, j), board))
                            place_sprite("graphics/critical2.png", sprite_pos);
                        //else if(e.is_critical_type1(pii(selected.F, selected.S), pii(i, j), board))
                        //    place_sprite("graphics/critical1.png", sprite_pos);
                    }

                    if(board->state[i][j].occupant.size() != 0){
                        place_sprite("graphics/"+board->state[i][j].occupant[0]->representation+".png", sprite_pos);
                    }
                }
            }
            window->display();
        }
};