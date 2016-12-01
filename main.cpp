#include "Window.hpp"
#include "Forest.hpp"
#include "scene_lua.hpp"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    string script = "Assets/geometryForest.lua";
    bool a = run_lua(script);
    cout << a << endl;

    const string title = "Actual Cannibal";
//    Window::launch(argc, argv, new Forest(), 1920, 1080, title);
    return 0;
}
