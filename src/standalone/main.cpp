#include "gui.h"

#include <algorithm>
#include <iostream>

int main(){
    auto gui = std::make_unique<MyGui>();

    return gui->runExample();
}
