#include "Personaje.h"

class Super : public Personaje {
private:
    char rango;
    string poder;
    double multiplicador;

    int aleatorio(int valorMinimo, int valorMaximo);

public:
    Super(string nombre, char rango, string poder);

    char getRango();
    string getPoder();
    double getMultiplicador();
    string getRangoTexto();

    void atacar(Personaje& objetivo);
    void habilidadEspecial(Personaje& objetivo);
}
