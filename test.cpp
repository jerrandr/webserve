#include <fstream>
#include <iostream>

int main() {
    std::ifstream src("test.png", std::ios::binary);
    std::ofstream dest("copie.png", std::ios::binary);

    if (!src) {
        std::cerr << "Erreur : impossible d'ouvrir image.png" << std::endl;
        return 1;
    }

    if (!dest) {
        std::cerr << "Erreur : impossible de créer copie.png" << std::endl;
        return 1;
    }

    dest << src.rdbuf();

    std::cout << "Copie terminée" << std::endl;
    return 0;
}
