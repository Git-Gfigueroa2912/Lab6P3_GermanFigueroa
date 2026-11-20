#include <string>
using namespace std;

class Personaje {
protected:
	string nombre;
	int hp;
	int hpMax;
	int ataque;
    int defensa;

public:
    Personaje(string nombre, int hpMax, int ataque, int defensa);
    virtual ~Personaje();

    string getNombre();
    int getHp();
    int getHpMax();
    int getAtaque();
    int getDefensa();

    void setHp(int nuevoHp);

    virtual int getAtaqueEfectivo() ;
    virtual void atacar(Personaje& objetivo);
    virtual void habilidadEspecial(Personaje& objetivo) = 0;
    virtual void recibirAtaque(int cantidad);
    void curar(int cantidad);
    bool estaVivo();
	
};


























































