#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#include <windows.h>

using namespace std;

/**
 * @brief Mostra a ajuda do programa no console
 */
void show_help() {
    cout << "Syntax: bf <filename>" << endl;
    cout << endl;
    cout << "   filename    : Brainfuck file" << endl;
}

/**
 * @brief Ponto de entrada
 * 
 * @param argc Número de argumetnos da linha de comando
 * @param argv Argumentos da linha de comando
 * 
 * @return int Código de terminação do programa
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        show_help();
        return 0;
    }

    ifstream file(argv[1], std::ios::binary);

    if (!file.good()) {
        cerr << "Failed to open file: `" << argv[1] << "'" << endl;
        return -0x01;
    }
    // Células do brainfuck
    vector<ifstream::char_type> cells;
    cells.push_back(0);

    // Ponteiro
    decltype(cells)::size_type cellptr = 0;

    // Pilha de posições de colchetes para loops
    stack<ifstream::pos_type> loops;


    for (ifstream::char_type c; file.get(c);) {
        switch (c) {
            case '[':
                if (cells[cellptr] == 0) {
                    for (size_t n = 1; file.get(c) && n > 0;) {
                        switch (c) {
                            case '[': n++; break;
                            case ']': n--; break;
                        }
                    }
                    
                } else {
                    loops.push(file.tellg());
                    file.seekg(loops.top(), ios::beg);
                }

                break;

            case ']':
                if (cells[cellptr] != 0) {
                    if (loops.empty()) {
                        cerr << endl;
                        cerr << "Unbalanced `[]'" << endl;

                        return -0x11;
                    }

                    file.seekg(0, ios::beg);
                    file.seekg(loops.top());

                } else
                    loops.pop();

                break;

            case '+':
                cells[cellptr]++;
                break;

            case '-':
                cells[cellptr]--;
                break;

            case '>':
                if (cellptr >= cells.max_size()){
                    cerr << endl;
                    cerr << "Invalid operation `>': Cell array overflow" << endl;
                    return -0x10;
                }

                cellptr++;
                if (cellptr == cells.size())
                    cells.push_back(0);

                break;

            case '<':
                if (cellptr > 0)
                    cellptr--;

                else {
                    cerr << endl;
                    cerr << "Invalid operation `<': Cell array overflow" << endl;
                    return -0x10;
                }
                
                break;

            case '.':
                cout << (char)cells[cellptr];
                break;

            case ',':
                cin.get(cells[cellptr]);
                break;
        }
    }

    file.close();

    return 0;
}