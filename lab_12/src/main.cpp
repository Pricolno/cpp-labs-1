#include <iostream>
#include <matrix.h>

const int regs_num = 10;
Matrix regs[regs_num];

std::size_t to_index(const std::string &reg_name) {
    return static_cast<std::size_t>(reg_name[1] - '0');
}

void handle_load() {
    std::string reg_name, file_name;
    std::cin >> reg_name >> file_name;
    std::size_t ind = to_index(reg_name);
    std::ifstream fin(file_name);

    fin >> regs[ind];
}

void handle_add() {
    std::string reg1, reg2;
    std::cin >> reg1 >> reg2;

    auto ind1 = to_index(reg1);
    auto ind2 = to_index(reg2);

    regs[ind1] += regs[ind2];
}

void handle_mul() {
    std::string reg1, reg2;
    std::cin >> reg1 >> reg2;

    auto ind1 = to_index(reg1);
    auto ind2 = to_index(reg2);

    regs[ind1] *= regs[ind2];
}

void handle_elem() {
    std::string reg;
    std::size_t i, j;
    std::cin >> reg >> i >> j;
    std::size_t ind = to_index(reg);

    std::cout << regs[ind].get(i, j) << std::endl;
}

void handle_print() {
    std::string reg;
    std::cin >> reg;

    std::cout << regs[to_index(reg)];
}

int main() {
    std::string cmd;
    while (std::cin >> cmd) {
        try {
            if (cmd == "load")
                handle_load();
            else if (cmd == "add")
                handle_add();
            else if (cmd == "mul")
                handle_mul();
            else if (cmd == "elem")
                handle_elem();
            else if (cmd == "print")
                handle_print();
            else if (cmd == "exit")
                break;
        } catch (MatrixException &e) {
            std::cout << e.what() << std::endl;
        }
    }
}
