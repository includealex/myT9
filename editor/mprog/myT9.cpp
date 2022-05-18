#include <iostream>
#include <fstream>
#include "editor.hpp"

int main() {
    using namespace std::chrono;
    size_t n = 1;
    Editor first;
    first.teach("LOR.txt");
    first.teach("Onegin.txt");
    
    std::ofstream fout("plot.txt");
    for(size_t i = 0; i < 10; ++i) {
        first.setnumofthread(n);
        
        fout << n << "  ";

        auto start = high_resolution_clock::now();
        first.thredit("file.txt");
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        fout << duration.count() << std::endl;

        n += 10;
    }
    fout.close();

    return 0;
}