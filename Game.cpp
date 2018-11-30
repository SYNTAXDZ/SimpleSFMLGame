#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "GUI.h"
// normalize:
/*

U = V / |V| => ( Vx, Vy ) / sqrt( pow( 2, Vx ) + pow( 2, Vy ) );

*/


// we have created a bullet class
class Bullet {

    private:
        // bullet's Sprite and texture
        sf::Sprite BSprite;
        sf::Texture *BTexture;

    public:
        // the bullet constructor has one param who is the texture
        Bullet( sf::Texture *texture ) {
            // init the texture
            this->BTexture = texture;

        }
        // a setter method to set the sprite and do stuff related to it
        void setSprite( sf::Sprite _sprite, sf::Vector2f position ) {
            // setting up the sprite and it's texture
            this->BSprite = _sprite;
            this->BSprite.setTexture( *this->BTexture );
            this->BSprite.setPosition( position.x + 32, position.y - 40 );

        }

        // getting the sprite
        sf::Sprite getSprite() {

            return this->BSprite;

        }

        void Shoot() {

            this->BSprite.move( .0f, -8.0f );

        }

};

// player class
class Player {

    private:
        // player's parameters like health, sprite...
        unsigned int health;
        unsigned int maxHealth;

        sf::Sprite PSprite;
        sf::Texture *PTexture;

    public:

        // player's constructor is like the bullet constructor
        Player( sf::Texture *_PTexture ) {

            // initialina maxHealth
            this->maxHealth = 20;
            // w fel badya tkoun hiya l health
            this->health = this->maxHealth;

            // player 3tinah texture ta3ah
            this->PTexture = _PTexture;

        }

        // setting the player sprite
        void setSprite( sf::Sprite sprite ) {

            // setting up the sprite
            this->PSprite = sprite;
            // hada sprite 3tinah texture
            this->PSprite.setTexture( *this->PTexture );

            this->PSprite.setScale( .8, .8 );
            this->PSprite.setPosition( sf::Vector2f( 350.0f, 520.0f ) );

        }

        // getting the sprite because she's a private member
        sf::Sprite getSprite() {

            return this->PSprite;

        }

        // method for movement and collision
        void Move( sf::RenderWindow &_window, float _movement ) {

            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Left ) ) {

                this->PSprite.move( -1*_movement, .0f );

            }

            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Right ) ) {

                this->PSprite.move( _movement, .0f );

            }
            // here is a simple collision algorithm
            if( this->PSprite.getPosition().x <= 0 ) {

                this->PSprite.setPosition( 0, this->PSprite.getPosition().y );

            } else if( this->PSprite.getPosition().x >= _window.getSize().x - this->PSprite.getGlobalBounds().width ) {

                this->PSprite.setPosition( _window.getSize().x - this->PSprite.getGlobalBounds().width, this->PSprite.getPosition().y );

            }

        }

        ~Player() {}

};

// Enemies class
class Enemy {

    private:

        int health;
        int maxHealth;
        sf::Sprite ESprite;
        sf::Texture *ETexture;

    public:

        // Enemy constructor
        Enemy( sf::Texture *texture ) {

            this->ETexture = texture;

            this->maxHealth = rand()%5 + 1; // maxHealth between 5 and 1
            this->health = this->maxHealth;

        }

        // get the sprite
        sf::Sprite getSprite() {

            return this->ESprite;

        }

        void setSprite( sf::Sprite sprite, sf::Vector2u windowSize ) {

            this->ESprite = sprite;
            this->ESprite.setTexture( *this->ETexture );
            this->ESprite.setScale( 0.5f, 0.5f );
            this->ESprite.setPosition( rand()% (int)( windowSize.x - this->ESprite.getGlobalBounds().width ), -30 );

        }

        // method to move the enemy
        void Move() {

            this->ESprite.move( 0.0f, 5.0f );

        }

        ~Enemy() {}

};


int main() {

    // to randomize stuffs
    srand( time( NULL ) );

    // creating the window
    sf::RenderWindow MyWindow( sf::VideoMode( 800, 600 ), "SimpleGame" );
    MyWindow.setFramerateLimit( 60 );

    // timers
    int shootTimer       = 10;
    int enemySpawnTimer  = 20;
    int score            =  0;


    // hna criyna font ta3na w drnalah loading
    sf::Font myFont;
    if( !myFont.loadFromFile( "source/fonts/arcadeclassic/ARCADECLASSIC.TTF" ) )
        printf( "Could Not Open Font \n" );

    sf::Font myFont2;
    if( !myFont2.loadFromFile( "source/fonts/from-cartoon-blocks/From Cartoon Blocks.ttf" ) )
        printf( "Could Not Open Font \n" );

    // han criyina text object
    sf::Text myText;
    // 3tinah font
    myText.setFont( myFont );
    // w size
    myText.setCharacterSize( 24 );
    //myText.setColor( sf::Color( 250, 90, 150 ) );
    // w style
    myText.setStyle( sf::Text::Bold );
    myText.setString( "u are Playing !" );
    myText.setPosition( sf::Vector2f( 310.0f, 10 ) );

    // init textures
    sf::Texture playerTexture;
    playerTexture.loadFromFile( "source/textures/spaceShip.png" );
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile( "source/textures/enemy.png" );
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile( "source/textures/Bullet.png" );

    // init the player
    Player MyPlayer( &playerTexture );
    sf::Sprite playerSprite;
    MyPlayer.setSprite( playerSprite );


    // init the bullet
    std::vector<Bullet> MyBullets;
    Bullet MyBullet( &bulletTexture );
    sf::Sprite bulletSprite;

    // init the enemy
    std::vector<Enemy> Enemies;
    sf::Sprite enemySprite;

    std::string score_str, score_text;
    sf::Text Score;

    // waiting when the window is opened
    while( MyWindow.isOpen() ) {

        sf::Event _event;

        // handling all events
        while( MyWindow.pollEvent( _event ) ) {

            // switching in  any event
            switch( _event.type ) {

                case sf::Event::EventType::Closed:

                    MyWindow.close();
                    break;
                // if the event is a key press
                case sf::Event::EventType::KeyPressed:

                    if( sf::Keyboard::Key::Escape == _event.key.code ) {

                        MyWindow.close();

                    }
                    break;

            }

        }

        if( shootTimer < 10 )
            shootTimer++;


        // setting bullets in this condition
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Space ) && shootTimer >= 10 ) {

            MyBullet.setSprite( bulletSprite, MyPlayer.getSprite().getPosition() );
            MyBullets.push_back( MyBullet );

            shootTimer = 0;

        }

        if( enemySpawnTimer < 20 ) {

            enemySpawnTimer++;

        }
        // adding the Enemies
        if( enemySpawnTimer >= 20 ) {

            Enemy NewEnemy( &enemyTexture );
            NewEnemy.setSprite( enemySprite, MyWindow.getSize() );

            Enemies.push_back( NewEnemy );

            enemySpawnTimer = 0;

        }

        MyPlayer.Move( MyWindow, 10.0f );

        // clearing the window
        MyWindow.clear( sf::Color( 110, 100, 190, 240 ) );

        // draw bullets
        for( size_t i = 0; i < MyBullets.size(); i++ ) {

            MyBullets[i].Shoot();

            if( MyBullets[i].getSprite().getPosition().y <= -2 ) {

                MyBullets.erase( MyBullets.begin() + i );

            }

            MyWindow.draw( MyBullets[i].getSprite() );

        }

        // draw Enemies
        for( size_t i = 0; i < Enemies.size(); i++ ) {

            Enemies[i].Move();

            if( Enemies[i].getSprite().getPosition().y >= MyWindow.getSize().y ) {

                Enemies.erase( Enemies.begin() + i );

            }
            if( Enemies[i].getSprite().getGlobalBounds().intersects( MyPlayer.getSprite().getGlobalBounds() ) ) {

                Enemies.erase( Enemies.begin() + i );

            }

            for( size_t j = 0; j < MyBullets.size(); j++ ) {

                if( Enemies[i].getSprite().getGlobalBounds().intersects( MyBullets[j].getSprite().getGlobalBounds() ) ) {

                    Enemies.erase( Enemies.begin() + i );

                    score++;

                }

            }

            MyWindow.draw( Enemies[i].getSprite() );

        }

        score_str = std::to_string( score );
        score_text = "score: " + score_str;

        Score.setCharacterSize( 42 );
        Score.setFont( myFont2 );
        Score.setStyle( sf::Text::Italic );
        Score.setString( score_text );
        Score.setPosition( sf::Vector2f( 620.0f, 530.0f ) );
        if( score_str.length() >= 3 ) { Score.setPosition( sf::Vector2f( 592.0f, 530.0f ) ); }

        MyWindow.draw( Score );
        MyWindow.draw( myText );
        MyWindow.draw( MyPlayer.getSprite() );
        // display everything
        MyWindow.display();

    }

    return EXIT_SUCCESS;

}
