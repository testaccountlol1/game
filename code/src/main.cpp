#include <SFML/Graphics.hpp>
#include <iostream>

void flipSprite(sf::Sprite& sprite, bool horizontal, bool vertical) {
    sf::Vector2f currentScale = sprite.getScale();
    
    float scaleX = horizontal ? -currentScale.x : currentScale.x;
    float scaleY = vertical   ? -currentScale.y : currentScale.y;
    
    sprite.setScale(scaleX, scaleY);
}

int main() {
    // SFML 3: VideoMode takes a sf::Vector2u instead of two numbers
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "SFML Character Controller");
    window.setFramerateLimit(60); 

    // 1. LOAD ALL TEXTURES
    sf::Texture texBase, texWalk1, texWalk2, texSwing;
    
    // (Keep these commented out or active based on your placeholder strategy)
    if (!texBase.loadFromFile("../../pngs/base.png")  || 
        !texWalk1.loadFromFile("../../pngs/walk1.png") || 
        !texWalk2.loadFromFile("../../pngs/walk2.png") || 
        !texSwing.loadFromFile("../../pngs/swing.png")) {
        std::cout << "Error: Could not find image files relative to gm/code/build/!" << std::endl;
        return -1;
    }

    texBase.setSmooth(false);
    texWalk1.setSmooth(false);
    texWalk2.setSmooth(false);
    texSwing.setSmooth(false);

    // 2. CREATE THE PLAYER SPRITE
    sf::Sprite player(texBase);
    
    // SFML 3: Spatial setters take structured sf::Vector2f brackets {}
    player.setPosition({0.f, 0.f});
    player.setScale({3.f, 3.f}); 

    // 3. ANIMATION TIMERS
    sf::Clock animationClock; 
    float speed = 4.f;        
    int walkFrame = 1;        

    // MAIN RENDERING & LOGIC LOOP
    while (window.isOpen()) {
        
        // SFML 3: pollEvent() returns a std::optional. We check if it holds a value.
        while (const std::optional event = window.pollEvent()) {
            // SFML 3 event matching uses standard variant checking or method checks
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 4. MULTI-DIRECTIONAL MOVEMENT INPUT HANDLING
        sf::Vector2f movement(0.f, 0.f);
        bool isMoving = false;

        // SFML 3: Keys are strictly nested under sf::Keyboard::Key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= speed;
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += speed;
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            movement.y -= speed;
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            movement.y += speed;
            isMoving = true;
        }

        // Apply spatial shift to player coordinates
        player.move(movement);

        // 5. ANIMATION STATE SELECTION
        
        // Priority 1: Swing Attack
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.setTexture(texSwing);
        }
        // Priority 2: Walk Animation Cycle
        else if (isMoving) {
            if (animationClock.getElapsedTime().asSeconds() > 0.15f) {
                if (walkFrame == 1) {
                    player.setTexture(texWalk1);
                    walkFrame = 2;
                } else {
                    player.setTexture(texWalk2);
                    walkFrame = 1;
                }
                animationClock.restart(); 
            }
        }
        // Priority 3: Default Standing Idle
        else {
            player.setTexture(texBase);
        }

        // 6. RENDER QUEUE
        window.clear(sf::Color(50, 50, 50)); 
        window.draw(player);
        window.display();
    }

    return 0;
}
