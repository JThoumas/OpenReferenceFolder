#include "application.h"

int main() {
    orf::Application app;
    if (!app.init(1280, 720, "OpenReferenceFolder"))
        return -1;
    app.run();
    app.shutdown();
    return 0;
}
