#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <time.h>
#include <unistd.h>

// Nota: si todos los cin y cout marcan error, quitar y poner otra vez esta linea para recargar el proyecto
// Nota2: cosas de c++ que no entiendo, pero funciona
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
    bool hayGanador = false;

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
        // Añadir la posicion a la cola correspondiente
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

    void buscarGanador(char jugador) {
        // en la horizontal
        for(int i = 0; i < TAMANO; i++) {
            if(casillas[i][0] == jugador && casillas[i][1] == jugador && casillas[i][2] == jugador) {
                hayGanador = true;
                return;
            }
        } // o(1)

        // en la vertical
        for(int i = 0; i < TAMANO; i++) {
            if(casillas[0][i] == jugador && casillas[1][i] == jugador && casillas[2][i] == jugador) {
                hayGanador = true;
                return;
            }
        } // o(1)

        // en la diagonal
        if(casillas[0][0] == jugador && casillas[1][1] == jugador && casillas[2][2] == jugador) {
            hayGanador = true;
            return;
        } // o(1)

        // diagonal inversa
        if(casillas[0][2] == jugador && casillas[1][1] == jugador && casillas[2][0] == jugador) {
            hayGanador = true;
            return;
        } // o(1)
    }

    // Nueva funcion para contar las fichas de un jugador
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

    // Nueva funcion para eliminar la ficha más antigua
    void eliminarFichaAntigua(char ficha) {
        queue<Posicion>& posiciones = (ficha == 'X') ? posicionesJugador1 : posicionesJugador2;
        if (!posiciones.empty()) {
            Posicion pos = posiciones.front();
            posiciones.pop();
            casillas[pos.getFila()][pos.getColumna()] = ' '; // Eliminar la ficha
        }
    }

    void imprimirPartida(char jugador) const {
        cout << "Guardando partida\n";
        // obtener la fecha y hora actual como nombre del archivo
        time_t hora = time(nullptr);
        tm* horaLocal = localtime(&hora);
        char fechaHora[80];
        strftime(fechaHora, 80, "%m_%d_%Y %H_%M_%S", horaLocal);
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
    } // o(1)
};

// Clase principal del juego
class TresEnRaya {
private:
    Tablero tablero;
    Jugador jugador1;
    Jugador jugador2;
    Jugador* jugadorActual;
    bool hayGanador;

public:
    TresEnRaya() :
        jugador1("Jugador 1", 'X'),
        jugador2("Jugador 2", 'O'),
        jugadorActual(&jugador1),
        hayGanador(false) {}

    void cambiarTurno() {
        jugadorActual = (jugadorActual == &jugador1) ? &jugador2 : &jugador1;
    }

    void jugadorVSjugador() {
        cout << "Elige una posicion del 1 al 9:\n";

        while(tablero.hayGanador == false) {
            system("cls");
            tablero.mostrarGuia();
            mueveElJugador();

            tablero.buscarGanador(jugadorActual->getFicha());

            // verificar si hay ganador
            if(tablero.hayGanador) {
                tablero.mostrar();
                cout << "El " << jugadorActual->getNombre() << " ha ganado!\n";
                tablero.imprimirPartida(jugadorActual->getFicha());
                if (!preguntarRepetir()) return; // Preguntar si quiere jugar de nuevo
            } else {
                cambiarTurno();
            }
        }
    }

    void jugarVScomputadora() {
        while (cin.get() != '\n');
        cout << "Elige una posicion del 1 al 9:\n";

        while(tablero.hayGanador == false) {
            system("cls");
            tablero.mostrarGuia();
            mueveElJugador();

            tablero.buscarGanador(jugadorActual->getFicha());

            // verificar si hay ganador
            if(tablero.hayGanador) {
                tablero.mostrar();
                cout << "El " << jugadorActual->getNombre() << " ha ganado!\n";
                tablero.imprimirPartida(jugadorActual->getFicha());
                if (!preguntarRepetir()) return; // Preguntar si quiere jugar de nuevo
            } else {
                tablero.mostrar();
                mueveLaComputadora('O');
            }
        }
    }

    void computadoraVScomputadora() {
        while (cin.get() != '\n');
        system("cls");
        tablero.mostrarGuia();

        char fichaActual = 'X';
        char fichaEnEspera = 'O';

        while(tablero.hayGanador == false) {
            system("cls");
            cout << "Turno de la computadora " << fichaActual << "\n";
            tablero.mostrar();
            mueveLaComputadora(fichaActual);
            swap(fichaActual, fichaEnEspera);
        }

        // Preguntar si quiere jugar de nuevo
        if (!preguntarRepetir()) return;
    }
    
    void mueveElJugador() {
        Posicion pos(0, 0);
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
                cout << "\033[31m" << "Posicion no valida. Intentalo de nuevo.\n" << "\033[0m";
            }
        }

        // Verificar si el jugador ya tiene tres fichas en el tablero
        if(tablero.contarFichas(jugadorActual->getFicha()) == 3) {
            tablero.eliminarFichaAntigua(jugadorActual->getFicha());
        }

        tablero.colocarFicha(pos, jugadorActual->getFicha());
    }
    
    void mueveLaComputadora(char ficha) {
        sleep(3); // son segundos, no milisegundos
        srand(static_cast<unsigned int>(time(0)));
        Posicion pos(0, 0);
        do {
            int fila = rand() % 3;
            int columna = rand() % 3;
            pos = Posicion(fila, columna);
        } while (!tablero.esPosicionValida(pos)); // o(1)

        if (tablero.contarFichas(ficha == 3)) {
            tablero.eliminarFichaAntigua(ficha);
        }

        // Verificar si el jugador ya tiene tres fichas en el tablero
        if(tablero.contarFichas(ficha) == 3) {
            tablero.eliminarFichaAntigua(ficha);
        }

        tablero.colocarFicha(pos, ficha);
        tablero.buscarGanador(ficha);

        if(tablero.hayGanador) {
            tablero.mostrar();
            cout << "La computadora " << ficha << " ha ganado!\n";
            tablero.imprimirPartida(ficha);
        } 
    }

    Posicion pedirMovimiento() {
        int numero;
        string entrada;

        while (true) {
            cout << "Elige una posicion (1-9): ";            
            cin >> entrada;

            if (isdigit(entrada[0]) && entrada.length() == 1) {
                numero = stoi(entrada);
                if (numero >= 1 && numero <= 9) {
                    break;
                }
            }

            cout << "\033[31m" << "Posicion no valida. Intentalo de nuevo.\n" << "\033[0m";
        }

        int fila = 2 - (numero - 1) / 3;
        int columna = (numero - 1) % 3;

        return Posicion(fila, columna);
    }

    // Nueva función para preguntar si quiere jugar de nuevo 
    bool preguntarRepetir() {
        char opcion;
        cout << "Quieres jugar de nuevo? (1: Si, 2: No): ";
        while (cin.get() != '\n');
        cin >> opcion;
        while (cin.get() != '\n');

        if (opcion == '1') {
            // Reiniciar el juego
            tablero = Tablero(); // Reiniciar el tablero
            hayGanador = false; // Reiniciar el estado de ganador
            return true;
        } else {
            exit(0); // mata el programa regresando 0
        }
        
        while (cin.get() != '\n');
        return false;
	}
};

int main() {
    TresEnRaya juego;
    string entrada;
    int num;

    cout << "  Bienvenido al juego de Tres en Raya!\n";
    cout << "  Modo de Juego:\n";
    cout << "  1. Humano vs Humano\n";
    cout << "  2. Humano vs Computadora\n";
    cout << "  3. Computadora vs Computadora\n\n";

    do {
        // clear opcion buffer
        cout << ">  Elige una opcion: ";
        cin >> entrada;
        if (isdigit(entrada[0]) && entrada.length() == 1) {
            num = stoi(entrada);
            if (num >= 1 && num <= 3) {
                break;
            }
        }
    } while (true);

    // Lógica para reiniciar el juego o cambiar de modo
    while (true) {
        switch(num) {
            case 1:
                juego.jugadorVSjugador();
                break;

            case 2:
                juego.jugarVScomputadora();
                break;

            case 3:
                juego.computadoraVScomputadora();
                break;
        }
    }

    system("pause");
    return 0;
}