#include <iostream>

#include "editor.hpp"

int main() {
    Editor first;
    first.teach("LOR.txt");
    first.teach("Onegin.txt");
    first.thredit("file.txt");

    return 0;
}