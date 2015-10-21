#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include "sha256.h"

/*****************************************************************************
 * Marco Fernández Pranno, Granada (2014).
 * Bajo licencia GPLv3.
 *
 * Abstracción de trabajo computacional en la minería de bitcoins.
 * De una forma simplificada intento emular el proceso que lleva a cabo un
 * software de minería por cpu.
 *****************************************************************************/
using namespace std;

/******************************************************************************
 * La ecuación simplificada del proceso es:
 *
 * sha256(sha256(datos+nonce)) < dificultad_actual == True: Paquete resuelto.
 *
 * En caso de no ser true, se itera de nuevo utilizando un nuevo valor en 'nonce'.
 * 'nonce' representa un valor aleatorio generado nuevamente en cada iteración.
 *
 * Voy a obviar los términos básicos y las definiciones de la estructura interna
 * tanto de los bloques de datos como de las transacciones, sin embargo, la
 * documentación referente a éstos se puede encontrar en:
 *
 * https://en.bitcoin.it/wiki/Block
 * https://en.bitcoin.it/wiki/Transactions
 *
 * La dificultad utilizada corresponde a la dificultad real el 2/12/14.
 * Para obtener la dificultad en un momento dado bastaría con agregar un comando
 * shell con wget o curl y una dirección como http://blockexplorer.com/q/getdifficulty
 *
 ******************************************************************************/

string cpu_info() {

    string comando = "./get_cpuinfo.sh";
    const char * cmd = comando.c_str();

    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char *argv[])
{
    cout << "Esta ejecutando el programa sobre un sistema basado en linux?(s/n):";
    char r;
    cin >> r;
    string modelo_cpu;
    if(r=='s')
        modelo_cpu = cpu_info();
    else
        modelo_cpu = "- No determinado - \n";

    long int dificultad_actual = 40300030327.891;

    cout << "Modelo de cpu:\n" << modelo_cpu;

    ifstream entrada("block");
    if(!entrada) {
        cerr << "Error en la apertura de el cauce de entrada.\n";
        exit(-1);
    }

    string bloque= "";
    string linea;
    while(!entrada.eof()) {
        getline(entrada, linea);
        bloque+=linea;
    }

    bloque = sha256(bloque);

    /******************************************************************************
     * Llegados a este punto ya tenemos un bloque emulado con sus respectivas
     * cabeceras y datos. Una vez cargado, lo pasamos por una iteración sha256
     * para simular correctamente lo que recibe el minero al comenzar su trabajo.
     ******************************************************************************/

    srand(time(NULL));

    long int nonce, datos_and_nonce;
    long int res;
    int cont = 0;

    cout << "Inicio del hasheo: \n";

    auto comienzo = chrono::high_resolution_clock::now();

    nonce = rand() * rand();
    datos_and_nonce = atoi(bloque.c_str()) + nonce;

    do {
        nonce = rand() * rand();
        datos_and_nonce = atoi(bloque.c_str()) + nonce;
        res = atoi(sha256(sha256(to_string(datos_and_nonce))).c_str());
        cont++;
    } while(cont < 100000); // < Dificultad.

    cout << cont << endl;
    auto fin = chrono::high_resolution_clock::now();

    cout << "Fin del hasheo.\nNumero de iteraciones: " << cont << endl;
    cout << "Tiempo transcurrido: ";

    float tiempo_transcurrido = (float)chrono::duration_cast<chrono::microseconds>(fin - comienzo).count() / 1000000;
    cout << tiempo_transcurrido << " segundos." << endl;

    float hashes_segundo = (float)cont/tiempo_transcurrido;
    cout << "Hashes por segundo: " << hashes_segundo << endl;


    return 0;
}
