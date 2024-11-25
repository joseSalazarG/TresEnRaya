#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Función para imprimir el tablero
void imprimirTablero(const vector<vector<char>>& tablero) {
    // para cada fila
    for (int i = 0; i < tablero.size(); i++) {
        // para cada columna
        for (int j = 0; j < tablero[i].size(); j++) {
            cout << tablero[i][j];
            if (j < tablero[i].size() - 1) {
                cout << "|";
            }
        }
        cout << endl;
    }
}

// Función para verificar si alguien ha ganado
bool hayGanador(const vector<vector<string>>& tablero) {
    // Verificar filas

    // Verificar columnas

    // Verificar diagonales
 
    return false;
}

int main() {

    vector<vector<char>> tablero(3, vector<char>(3, ' ')); // Crear un tablero vacío
    char jugadorActual = 'X'; // Jugador actual (X o O)
    int fila, columna;

    // Bucle principal del juego
    while (true) {
        // Imprimir el tablero
        imprimirTablero(tablero);

        // Pedir al jugador que ingrese la posición
        cout << "Turno del jugador de las " << jugadorActual << ". Ingrese la fila: ";
        cin >> fila;
        cout << "Turno del jugador de las" << jugadorActual << ". Ingrese la columna: ";
        cin >> columna;

        // Verificar si la posición es válida
        //si esta fuera del tablero
        if (fila < 1 || fila > 3 || columna < 1 || columna > 3) {
            cout << "Posicion invalida. Intentalo de nuevo." << endl;
            continue;
        } else if (tablero[fila - 1][columna - 1] != ' ') {
            cout << "Esa posicion ya esta ocupada. Intentalo de nuevo." << endl;
            continue;
        }

        // Actualizar el tablero con la jugada del jugador
        tablero[fila - 1][columna - 1] = jugadorActual;

        // Verificar si el jugador actual ha ganado
        /*
        if (hayGanador(tablero)) {
            cout << "El jugador " << jugadorActual << " ha ganado!" << endl;
            break;
        }*/

        // Cambiar al siguiente jugador
        if (jugadorActual == 'X') {
            jugadorActual = 'O';
        } else {
            jugadorActual = 'X';
        }
    }

    // Imprimir el tablero final
    imprimirTablero(tablero);

    return 0; 
} 