#include "raylib-cpp.hpp"

class Player {
    public:
    Player();

    void Draw() const;
    void Update();

    raylib::Rectangle position;
    int speed{3};
};
