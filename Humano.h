#include "Personaje.h"

class Humano : public Personaje {
private:
    int turnos;
    bool UsadoEnBatalla;

public:
    Humano(string nombre, int hpMax, int ataque, int defensa);

    int getTurnos();
    bool getUsadoEnBatalla();

    void reiniciarBatalla();
    int getAtaqueEfectivo();
    void atacar(Personaje& objetivo);
    void habilidadEspecial(Personaje& objetivo);
    void activar();
    void pasarTurno();
    bool tiene();
};
