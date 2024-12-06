#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <time.h>

using namespace std;

// Clase para representar a un jugador
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
    queue<Posicion> posicionesJugador1; // Cola para las posiciones del Jugador 1
    queue<Posicion> posicionesJugador2; // Cola para las posiciones del Jugador 2

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
        // Añadir la posición a la cola correspondiente
        if (ficha == 'X') {
            posicionesJugador1.push(pos);
        } else {
            posicionesJugador2.push(pos);
        }
    }

    void mostrar() const {
        cout << "\n";
        for(int i = 0; i < TAMANO; i++) {
            for(int j = 0; j < TAMANO; j++) {
                char ficha = casillas[i][j];
                // Verificar si la ficha es la más antigua y marcarla con color
                if (!posicionesJugador1.empty() && ficha == 'X' && 
                    posicionesJugador1.front().getFila() == i && posicionesJugador1.front().getColumna() == j) {
                    cout << "\033[31m " << ficha << "\033[0m "; // Cambiar a rojo
                } else if (!posicionesJugador2.empty() && ficha == 'O' && 
                           posicionesJugador2.front().getFila() == i && posicionesJugador2.front().getColumna() == j) {
                    cout << "\033[31m " << ficha << "\033[0m "; // Cambiar a rojo
                } else {
                    cout << " " << ficha << " ";
                }
                if (j < TAMANO - 1) cout << "|";
            }
            cout << endl;
            if (i < TAMANO - 1) cout << "---+---+---" << endl;
        }
        cout << "\n";
    }

    void mostrarGuia() const {
        cout << "\nPosiciones del tablero:\n";
        int pos = 7;
        for(int i = 0; i < TAMANO; i++) {
            cout << " " << pos << " | " << (pos+1) << " | " << (pos+2) << endl;
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

    bool hayGanador(char jugador) const {
        // en la horizontal
        for(int i = 0; i < TAMANO; i++) {
            if(casillas[i][0] == jugador && casillas[i][1] == jugador && casillas[i][2] == jugador) {
                return true;
            }
        } // o(1)

        // en la vertical
        for(int i = 0; i < TAMANO; i++) {
            if(casillas[0][i] == jugador && casillas[1][i] == jugador && casillas[2][i] == jugador) {
                return true;
            }
        } // o(1)

        // en la diagonal
        if(casillas[0][0] == jugador && casillas[1][1] == jugador && casillas[2][2] == jugador) {
            return true;
        } // o(1)
        // diagonal inversa
        if(casillas[0][2] == jugador && casillas[1][1] == jugador && casillas[2][0] == jugador) {
            return true;
        } // o(1)

        return false; 
    }

    // Nueva función para contar las fichas de un jugador
    int contarFichas(char ficha) const {
        int contador = 0;
        for(int i = 0; i < TAMANO; i++) {
            for(int j = 0; j < TAMANO; j++) {
                if(casillas[i][j] == ficha) {
                    contador++;
                }
            }
        }
        return contador;
    }

    // Nueva función para eliminar la ficha más antigua
    void eliminarFichaAntigua(char ficha) {
        queue<Posicion>& posiciones = (ficha == 'X') ? posicionesJugador1 : posicionesJugador2;
        if (!posiciones.empty()) {
            Posicion pos = posiciones.front();
            posiciones.pop();
            casillas[pos.getFila()][pos.getColumna()] = ' '; // Eliminar la ficha
        }
    }

    void imprimirPartida(char jugador) const {
        cout << "Guardando partida...\n";
        // obtener la fecha y hora actual como nombre del archivo
        time_t hora = time(nullptr);
        tm* horaLocal = localtime(&hora);
        char fechaHora[80];
        strftime(fechaHora, 80, "%m_%d_%Y %H_%M", horaLocal);
        char nombreArchivo[100];
        sprintf(nombreArchivo, "partida_%s.txt", fechaHora);

        // creamos el archivo
        ofstream archivo;
        archivo.open(nombreArchivo);
        archivo << "\n";
        for(int i = 0; i < TAMANO; i++) {
            archivo << " " << casillas[i][0] << " | " << casillas[i][1] << " | " << casillas[i][2] << endl;
            if(i < TAMANO-1) archivo << "---+---+---" << endl;
        }
        archivo << "\n";
        archivo << "El " << jugador << " ha ganado!\n";
        // cerramos el archivo
        archivo.close();
    }
};

// Clase principal del juego
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
        cout << "¡Bienvenido al juego de Tres en Raya!\n";
        cout << "Elige una posición del 1 al 9:\n";
        tablero.mostrarGuia();

        bool hayGanador = false;
        Posicion pos(0, 0);
        
        while(!hayGanador) {
            tablero.mostrar();
            
            cout << "Turno de " << jugadorActual->getNombre() << " (" << jugadorActual->getFicha() << ")\n";

            // Indicar al jugador qué ficha se eliminará si ya tiene tres fichas
            if(tablero.contarFichas(jugadorActual->getFicha()) == 3) {
                cout << "Nota: Se eliminara la ficha mas antigua antes de colocar una nueva.\n";
            }
            
            // repetir pedir movimiento hasta que sea valido
            while (true) {
                pos = pedirMovimiento();
                if(tablero.esPosicionValida(pos)) {
                    break;
                } else {
                    cout << "Posicion no valida. Intentalo de nuevo.\n";
                }
            }

            // Verificar si el jugador ya tiene tres fichas en el tablero
            if(tablero.contarFichas(jugadorActual->getFicha()) == 3) {
                tablero.eliminarFichaAntigua(jugadorActual->getFicha());
            }

            tablero.colocarFicha(pos, jugadorActual->getFicha());

            // verificar si hay ganador
            if(tablero.hayGanador(jugadorActual->getFicha())) {
                tablero.mostrar();
                cout << "El " << jugadorActual->getNombre() << " ha ganado!\n";
                tablero.imprimirPartida(jugadorActual->getFicha());
                hayGanador = true;
                
            } else {
                cambiarTurno();
            }
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
    cout << "Presiona una tecla";
    cin >> ws;
    return 0;
}