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
};