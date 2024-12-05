#include <iostream>
#include <string>

using namespace std;

class Jugador {
private:
    string nombre;
    char ficha;

public:
    Jugador(string _nombre, char _ficha) : nombre(_nombre), ficha(_ficha) {}

    string getNombre() const { return nombre; }
    char getFicha() const { return ficha; }
};


class Posicion {
private:
    int fila;
    int columna;

public:
    Posicion(int _fila, int _columna) : fila(_fila), columna(_columna) {}

    int getFila() const { return fila; }
    int getColumna() const { return columna; }
};


class Tablero {
private:
    static const int TAMANO = 3;
    char casillas[TAMANO][TAMANO];

public:
    Tablero() {
        inicializar();
    }

    void inicializar() {
        for(int i = 0; i < TAMANO; i++) {
            for(int j = 0; j < TAMANO; j++) {
                casillas[i][j] = ' ';
            }
        }
    }

    bool esPosicionValida(const Posicion& pos) const {
        return pos.getFila() >= 0 && pos.getFila() < TAMANO &&
               pos.getColumna() >= 0 && pos.getColumna() < TAMANO &&
               casillas[pos.getFila()][pos.getColumna()] == ' ';
    }

    void colocarFicha(const Posicion& pos, char ficha) {
        casillas[pos.getFila()][pos.getColumna()] = ficha;
    }

    void mostrar() const {
        cout << "\n";
        for(int i = 0; i < TAMANO; i++) {
            cout << " " << casillas[i][0] << " | " << casillas[i][1] << " | " << casillas[i][2] << endl;
            if(i < TAMANO-1) cout << "---+---+---" << endl;
        }
        cout << "\n";
    }

    void mostrarGuia() const {
        cout << "\nPosiciones del tablero:\n";
        int pos = 9;
        for(int i = 0; i < TAMANO; i++) {
            cout << " " << (pos-2) << " | " << (pos-1) << " | " << pos << endl;
            if(i < TAMANO-1) cout << "---+---+---" << endl;
            pos -= 3;
        }
        cout << "\n";
    }

    bool estaLleno() const {
        for(int i = 0; i < TAMANO; i++) {
            for(int j = 0; j < TAMANO; j++) {
                if(casillas[i][j] == ' ') return false;
            }
        }
        return true;
    }

    bool hayGanador(char ficha) const {
       
        for(int i = 0; i < TAMANO; i++) {
            if((casillas[i][0] == ficha && casillas[i][1] == ficha && casillas[i][2] == ficha) ||
               (casillas[0][i] == ficha && casillas[1][i] == ficha && casillas[2][i] == ficha)) {
                return true;
            }
        }
        
        return (casillas[0][0] == ficha && casillas[1][1] == ficha && casillas[2][2] == ficha) ||
               (casillas[0][2] == ficha && casillas[1][1] == ficha && casillas[2][0] == ficha);
    }
};


class TresEnRaya {
private:
    Tablero tablero;
    Jugador jugador1;
    Jugador jugador2;
    Jugador* jugadorActual;

public:
    TresEnRaya() : 
        jugador1("Jugador 1", 'X'),
        jugador2("Jugador 2", 'O'),
        jugadorActual(&jugador1) {}

    void cambiarTurno() {
        jugadorActual = (jugadorActual == &jugador1) ? &jugador2 : &jugador1;
    }

    void jugar() {
        cout << "  Bienvenido al juego de Tres en Raya !\n";
        cout << "Elige una posicion del 1 al 9:\n";
        tablero.mostrarGuia();

        bool hayGanador = false;
        
        while(!hayGanador && !tablero.estaLleno()) {
            tablero.mostrar();
            
            cout << "Turno de " << jugadorActual->getNombre() 
                 << " (" << jugadorActual->getFicha() << ")\n";
            
            Posicion pos = pedirMovimiento();
            
            while(!tablero.esPosicionValida(pos)) {
                cout << "Posicion invalida. Intenta de nuevo.\n";
                pos = pedirMovimiento();
            }

            tablero.colocarFicha(pos, jugadorActual->getFicha());

            if(tablero.hayGanador(jugadorActual->getFicha())) {
                tablero.mostrar();
                cout << "¡" << jugadorActual->getNombre() << " ha ganado!\n";
                hayGanador = true;
            } else {
                cambiarTurno();
            }
        }

        if(!hayGanador) {
            tablero.mostrar();
        }
    }

private:
     Posicion pedirMovimiento() {
        int numero;
        cout << "Elige una posicion (1-9): ";
        cin >> numero;
        
        int fila = 2 - (numero - 1) / 3;
        int columna = (numero - 1) % 3;
        
        return Posicion(fila, columna);
    }
};

int main() {
    TresEnRaya juego;
    juego.jugar();
    return 0;
}