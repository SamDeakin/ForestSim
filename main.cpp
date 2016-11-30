#include "Window.hpp"
#include "Forest.hpp"

#include <string>

using namespace std;

int main(int argc, char **argv) {
    const string title = "Actual Cannibal";
    Window::launch(argc, argv, new Forest(), 1920, 1080, title);
    return 0;
}
