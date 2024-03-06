#include "Player.hpp"
#include "raylib-cpp.hpp"

Player::Player() {
    position = raylib::Rectangle{{
        .x = static_cast<float>(::GetScreenWidth()) / 2.0F - 50.0F,
        .y = static_cast<float>(::GetScreenHeight()) / 2.0F - 50.0F,
        .width = 100,
        .height = 100
    }};
}

void Player::Draw() const {
    position.Draw(RED);
}

void Player::Update() {
    if (IsKeyDown(KEY_UP)) {
        position.y -= static_cast<float>(speed);
    }
    if (IsKeyDown(KEY_DOWN)) {
        position.y += static_cast<float>(speed);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        position.x += static_cast<float>(speed);
    }
    if (IsKeyDown(KEY_LEFT)) {
        position.x -= static_cast<float>(speed);
    }
}
