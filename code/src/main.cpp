#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

// Enum for clean direction tracking
enum class LookDirection { Left, Right };

int main() {
    // SFML 3 Vector2u configuration
    sf::RenderWindow window(sf::VideoMode({1920u, 860u}), "SFML Character Controller");
    window.setFramerateLimit(60); 

    // 1. LOAD ALL TEXTURES (Fixed logic operator to ||)
    sf::Texture texBase, texWalk1, texWalk2, texSwing;
    
    if (!texBase.loadFromFile("../../pngs/base.png")  ||
        !texWalk1.loadFromFile("../../pngs/walk1.png") || 
        !texWalk2.loadFromFile("../../pngs/walk2.png") ||
        !texSwing.loadFromFile("../../pngs/swing.png")) {
        std::cout << "Error: Could not find one or more image files!" << std::endl;
        return -1;
    }

    texBase.setSmooth(false);
    texWalk1.setSmooth(false);
    texWalk2.setSmooth(false);
    texSwing.setSmooth(false);

    // 2. CREATE THE PLAYER SPRITE
    sf::Sprite player(texBase);
    
    // Set origin to the center so flipping doesn't teleport the sprite
    sf::FloatRect bounds = player.getLocalBounds();
    player.setOrigin({bounds.width / 2.0f, bounds.height / 2.0f});
    
    player.setPosition({400.f, 400.f});
    
    // Base scale vector constants
    const float baseScale = 3.f;
    player.setScale({baseScale, baseScale}); 

    // 3. STATE AND TIMERS (Persistent outside loop)
    sf::Clock animationClock; 
    float speed = 4.f;        
    int walkFrame = 1;        
    bool isMoving = false;
    LookDirection facing = LookDirection::Right; // Track where player looks globally

    // MAIN RENDERING & LOGIC LOOP
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 4. INPUT HANDLING
        sf::Vector2f movement(0.f, 0.f);
        isMoving = false; // Reset movement check per frame

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= speed;
            facing = LookDirection::Left; // Persistent update
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += speed;
            facing = LookDirection::Right; // Persistent update
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

        // Apply movement
        player.move(movement);

        // 5. DIRECTIONAL RENDER FLIPPING (Absolute Scale Control)
        if (facing == LookDirection::Left) {
            player.setScale({-baseScale, baseScale}); // Flipped Horizontally
        } else {
            player.setScale({baseScale, baseScale});  // Normal Facing Right
        }

        // 6. ANIMATION TEXTURE SELECTION
        // Priority 1: Attack Frame
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.setTexture(texSwing);
        }
        // Priority 2: Walk Animation Cycle (Now triggers cleanly during X or Y movement)
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

        // 7. RENDER QUEUE
        window.clear(sf::Color(50, 50, 50)); 
        window.draw(player);
        window.display();
    }

    return 0;
}
