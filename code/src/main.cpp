#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

// Clean state tracker for look direction
enum class LookDirection { Left, Right, Up, Down};

int main() {
    // SFML 3: VideoMode takes a sf::Vector2u instead of two individual numbers
    sf::RenderWindow window(sf::VideoMode({1920u, 1080u}), "SFML Character Controller");
    window.setFramerateLimit(60); 

    // 1. LOAD ALL TEXTURES
    sf::Texture texBase, texWalk1, texWalk2, texSwing, texSideBase;
    
    // Fixed: Uses logical || so loading safely checks all mandatory assets
    if (!texBase.loadFromFile("../../pngs/base.png")   ||
        !texWalk1.loadFromFile("../../pngs/walk1.png") || 
        !texWalk2.loadFromFile("../../pngs/walk2.png") ||
        !texSwing.loadFromFile("../../pngs/swing.png") ||
        !texSideBase.loadFromFile("../../pngs/RightBase.png")) {
        std::cout << "Error: Could not find one or more image files!" << std::endl;
        return -1;
    }

    texBase.setSmooth(false);
    texWalk1.setSmooth(false);
    texWalk2.setSmooth(false);
    texSwing.setSmooth(false);
    texSideBase.setSmooth(false);
    // 2. CREATE THE PLAYER SPRITE
    sf::Sprite player(texBase);
    
    // SFML 3 FIX: Rectangles use .getCenter() instead of old .width/.height
    player.setOrigin(player.getLocalBounds().getCenter());
    
    // SFML 3: Spatial setters take structured sf::Vector2f brackets {}
    player.setPosition({400.f, 400.f});
    
    const float baseScale = 3.f;
    

    // 3. ANIMATION STATE & TIMERS (Kept persistent outside the loop)
    sf::Clock animationClock; 
    float speed = 4.f;        
    int walkFrame = 1;        
    bool isMoving = false;
    LookDirection facing = LookDirection::Right; // Track orientation persistently

    // MAIN RENDERING & LOGIC LOOP
    while (window.isOpen()) {
        player.setScale({baseScale, baseScale}); //to stop the anim thing
        // SFML 3: pollEvent() returns a std::optional.
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 4. MULTI-DIRECTIONAL MOVEMENT INPUT HANDLING
        sf::Vector2f movement(0.f, 0.f);
        isMoving = false; // Reset movement status flag each frame
        bool isMovingHorizontally = false; // Track horizontal movement for animation

        // SFML 3: Keys are strictly nested under sf::Keyboard::Key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= speed;
            facing = LookDirection::Left;
            isMoving = true;
            isMovingHorizontally = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += speed;
            facing = LookDirection::Right;
            isMoving = true;
            isMovingHorizontally = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            movement.y -= speed;
            facing = LookDirection::Up;
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            movement.y += speed;
            facing = LookDirection::Down;
            isMoving = true;
        }

        // Apply spatial shift to player coordinates
        player.move(movement);

        // 5. DIRECTIONAL RENDER FLIPPING (Absolute Scale Mode - Only for horizontal movement)
        if (isMovingHorizontally) {
            if (facing == LookDirection::Left) {
                player.setTexture(texSideBase);
                player.setScale({-baseScale, baseScale});  // Flip horizontally for left orientation
            } else if (facing == LookDirection::Right) {
                player.setTexture(texSideBase); 
                player.setScale({baseScale, baseScale});  // Normal right orientation
            }
        }

        // 6. ANIMATION STATE SELECTION
        // Priority 1: Swing Attack
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.setScale({baseScale, baseScale});
            player.setTexture(texSwing);
        }
        // Priority 2: Walk Animation Cycle (Only triggers on vertical movement, not horizontal)
        else if (isMoving && !isMovingHorizontally) {
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
        // Priority 3: Default Standing Idle or Moving Horizontally
        else {
            if (!isMovingHorizontally) {
                player.setTexture(texBase);
            }
        }

        // 7. RENDER QUEUE
        window.clear(sf::Color(50, 50, 50)); 
        window.draw(player);
        window.display();
    }
    return 0;
}