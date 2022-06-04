#include <bits/stdc++.h>
#include "pieces.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

#define F first 
#define S second 

typedef pair<int, int> pii;

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

        void draw_board(ChessBoard* board, pii selected){
            window->clear(sf::Color::Black);

            place_sprite("graphics/board.png", pii(0, 0));
            set<pii> valid_moves;
            if(selected.F != -1){
                pii selected_pos = pii(8 + selected.S * 88, 8 + (7 - selected.F) * 88);
                place_sprite("graphics/select.png", selected_pos);
                valid_moves = board->state[selected.F][selected.S].occupant[0]->get_moves(board);
            }
            for (int i = 7; i >= 0; i--){
                for(int j = 0; j < 8; j++){
                    pii sprite_pos = pii(8 + j * 88, 8 + (7 - i) * 88);
                    if(valid_moves.find(pii(i, j)) != valid_moves.end()){
                        place_sprite("graphics/move.png", sprite_pos);
                    }
                    if(board->state[i][j].occupant.size() != 0){
                        place_sprite("graphics/"+board->state[i][j].occupant[0]->representation+".png", sprite_pos);
                    }
                }
            }

            window->display();
        }
};