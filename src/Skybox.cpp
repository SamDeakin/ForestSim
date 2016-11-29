#include <lodepng/lodepng.h>
#include "Skybox.hpp"

#include <string>
#include <iostream>

using namespace std;

Skybox::Skybox(std::string texture) {
    this->texture = string("Assets/") + texture;
}

void Skybox::init() {
    // We start by creating our filename strings
    // We have no complicated searching logic, filenames have to follow this
    // They should also all have the same dimensions
    string backString = texture + "Back2048.png";
    string downString = texture + "Down2048.png";
    string frontString = texture + "Front2048.png";
    string leftString = texture + "Left2048.png";
    string rightString = texture + "Right2048.png";
    string upString = texture + "Up2048.png";

    // Load the files
    unsigned error;

    error = lodepng::decode(back, width, height, backString);
    if (error) {
        string s = string("back: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    // Now do that another 5 times... so shitty...
    error = lodepng::decode(down, width, height, downString);
    if (error) {
        string s = string("down: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(front, width, height, frontString);
    if (error) {
        string s = string("front: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(left, width, height, leftString);
    if (error) {
        string s = string("left: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(right, width, height, rightString);
    if (error) {
        string s = string("right: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    error = lodepng::decode(up, width, height, upString);
    if (error) {
        string s = string("up: ") + lodepng_error_text(error);
        cout << s << endl;
        throw s;
    }

    // Load the shader program
    // TODO
}

Skybox::~Skybox() {

}

// Render the skybox
void Skybox::render() {

}
