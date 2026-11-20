
#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
class Inventario {
private:
    vector<T> elementos;

public:
    void agregar(T item) {
        elementos.push_back(item);
    }

    T extraer(int indice) {
        if (indice < 0 || indice >= static_cast<int>(elementos.size())) {
            throw out_of_range("Indice fuera de rango en Inventario::extraer");
        }
        T item = elementos.at(indice);
        elementos.erase(elementos.begin() + indice);
        return item;
    }

    T obtener(int indice) const {
        if (indice < 0 || indice >= static_cast<int>(elementos.size())) {
            throw out_of_range("Indice fuera de rango en Inventario::obtener");
        }
        return elementos.at(indice);
    }

    int cantidad() const {
        return static_cast<int>(elementos.size());
    }

    bool estaVacio() const {
        return elementos.empty();
    }
};
