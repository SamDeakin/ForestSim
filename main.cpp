#include "Window.hpp"
#include "Forest.hpp"
#include "scene_lua.hpp"
#include "RenderList.hpp"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    string script = "Assets/geometryForest.lua";

    if (argc > 1) {
        script = argv[1];
    }

    RenderList* list = run_lua(script);

    cout << "Returned size: " << list->size() << endl;

    const string title = "Actual Cannibal";
    Window::launch(argc, argv, new Forest(list), 1920, 1080, title);
    return 0;
}
