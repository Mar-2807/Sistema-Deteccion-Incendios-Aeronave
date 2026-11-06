#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <limits>
#include <cctype>
#include <ctime>

using namespace std;

class ExcepcionSistema : public exception {
private:
    string mensaje;

public:
    explicit ExcepcionSistema(const string& _mensaje) : mensaje(_mensaje) {}

    const char* what() const noexcept override {
        return mensaje.c_str();
    }

    string queExcepcion() const {
        return mensaje;
    }
};

class Sensor {
private:
    int id;
    float valorTemperatura;

public:
    explicit Sensor(int idSensor = 0)
        : id(idSensor), valorTemperatura(0.0f) {}

    void leer(mt19937& generador) {
        uniform_int_distribution<int> distribucion(250, 700);
        valorTemperatura = static_cast<float>(distribucion(generador));
    }

    int getId() const {
        return id;
    }

    float getTemperatura() const {
        return valorTemperatura;
    }
};

class Loop {
private:
    string nombre;
    Sensor sensores[2];
    bool esFuego;
    bool esOverheat;

public:
    Loop(const string& nombreLoop = "A",
         int idSensor1 = 1,
         int idSensor2 = 2)
        : nombre(nombreLoop),
          sensores{Sensor(idSensor1), Sensor(idSensor2)},
          esFuego(false),
          esOverheat(false) {}

    void leerSensores(mt19937& generador) {
        sensores[0].leer(generador);
        sensores[1].leer(generador);
    }

    void evaluarLoop(float umbralOverheat, float umbralFuego) {
        bool sensor1Fuego =
            sensores[0].getTemperatura() > umbralFuego;

        bool sensor2Fuego =
            sensores[1].getTemperatura() > umbralFuego;

        bool sensor1Overheat =
            sensores[0].getTemperatura() > umbralOverheat;

        bool sensor2Overheat =
            sensores[1].getTemperatura() > umbralOverheat;

        esFuego = sensor1Fuego && sensor2Fuego;

        esOverheat =
            !esFuego &&
            (sensor1Overheat || sensor2Overheat);
    }

    bool getEsFuego() const {
        return esFuego;
    }

    bool getEsOverheat() const {
        return esOverheat;
    }

    string getNombre() const {
        return nombre;
    }

    const Sensor& getSensor(int indice) const {
        if (indice < 0 || indice > 1) {
            throw ExcepcionSistema("Indice de sensor no valido.");
        }

        return sensores[indice];
    }

    bool operator==(const Loop& otro) const {
        return esFuego == otro.esFuego &&
               esOverheat == otro.esOverheat;
    }
};

class ComputadoraMotor {
private:
    int numeroMotor;
    Loop loopA;
    Loop loopB;

    bool valvulaSOV_Cerrada;
    bool botellaA_Detonada;
    bool botellaB_Detonada;

    bool masterWarning;
    bool masterCaution;

    string pantallaEICAS;

public:
    explicit ComputadoraMotor(int numero = 1)
        : numeroMotor(numero),
          loopA("A", numero * 10 + 1, numero * 10 + 2),
          loopB("B", numero * 10 + 3, numero * 10 + 4),
          valvulaSOV_Cerrada(false),
          botellaA_Detonada(false),
          botellaB_Detonada(false),
          masterWarning(false),
          masterCaution(false),
          pantallaEICAS("NORMAL") {}

    void actualizarEstado(mt19937& generador) {
        loopA.leerSensores(generador);
        loopB.leerSensores(generador);

        loopA.evaluarLoop(350.0f, 600.0f);
        loopB.evaluarLoop(350.0f, 600.0f);

        masterWarning =
            loopA.getEsFuego() &&
            loopB.getEsFuego();

        masterCaution =
            !masterWarning &&
            (
                loopA.getEsOverheat() ||
                loopB.getEsOverheat() ||
                loopA.getEsFuego() ||
                loopB.getEsFuego()
            );

        if (masterWarning) {
            valvulaSOV_Cerrada = true;
            pantallaEICAS =
                "ENG " + to_string(numeroMotor) + " FIRE";
        }
        else if (masterCaution) {
            pantallaEICAS =
                "ENG " + to_string(numeroMotor) + " OVERHEAT";
        }
        else {
            pantallaEICAS = "NORMAL";
        }
    }

    void dispararBotella(char id) {
        id = static_cast<char>(
            toupper(static_cast<unsigned char>(id))
        );

        if (!masterWarning) {
            throw ExcepcionSistema(
                "No se puede disparar una botella: no existe MASTER WARNING."
            );
        }

        if (!valvulaSOV_Cerrada) {
            throw ExcepcionSistema(
                "No se puede disparar la botella: la valvula SOV no esta cerrada."
            );
        }

        if (id == 'A') {
            if (botellaA_Detonada) {
                throw ExcepcionSistema(
                    "La botella SQUIB A ya fue utilizada."
                );
            }

            botellaA_Detonada = true;
        }
        else if (id == 'B') {
            if (!botellaA_Detonada) {
                throw ExcepcionSistema(
                    "SQUIB B solo puede utilizarse despues de SQUIB A."
                );
            }

            if (botellaB_Detonada) {
                throw ExcepcionSistema(
                    "La botella SQUIB B ya fue utilizada."
                );
            }

            botellaB_Detonada = true;
        }
        else {
            throw ExcepcionSistema(
                "Botella no valida. Seleccione A o B."
            );
        }
    }

    void mostrarEICAS() const {
        cout << "\n==============================================\n";
        cout << "             EICAS - MOTOR "
             << numeroMotor << "\n";
        cout << "==============================================\n";

        cout << fixed << setprecision(1);

        cout << "Loop A - Sensor "
             << loopA.getSensor(0).getId()
             << ": "
             << loopA.getSensor(0).getTemperatura()
             << " C\n";

        cout << "Loop A - Sensor "
             << loopA.getSensor(1).getId()
             << ": "
             << loopA.getSensor(1).getTemperatura()
             << " C\n";

        cout << "Loop B - Sensor "
             << loopB.getSensor(0).getId()
             << ": "
             << loopB.getSensor(0).getTemperatura()
             << " C\n";

        cout << "Loop B - Sensor "
             << loopB.getSensor(1).getId()
             << ": "
             << loopB.getSensor(1).getTemperatura()
             << " C\n";

        cout << "----------------------------------------------\n";

        cout << "Loop A: "
             << estadoLoop(loopA)
             << "\n";

        cout << "Loop B: "
             << estadoLoop(loopB)
             << "\n";

        cout << "----------------------------------------------\n";

        cout << "MASTER WARNING: "
             << (masterWarning ? "ACTIVO" : "INACTIVO")
             << "\n";

        cout << "MASTER CAUTION: "
             << (masterCaution ? "ACTIVO" : "INACTIVO")
             << "\n";

        cout << "EICAS: "
             << pantallaEICAS
             << "\n";

        cout << "SOV: "
             << (valvulaSOV_Cerrada ? "CERRADA" : "ABIERTA")
             << "\n";

        cout << "SQUIB A: "
             << (botellaA_Detonada ? "DESCARGADA" : "ARMADA")
             << "\n";

        cout << "SQUIB B: "
             << (botellaB_Detonada ? "DESCARGADA" : "ARMADA")
             << "\n";

        cout << "==============================================\n";
    }

    void resetSistema() {
        if (loopA.getEsFuego() ||
            loopB.getEsFuego() ||
            loopA.getEsOverheat() ||
            loopB.getEsOverheat()) {

            throw ExcepcionSistema(
                "No se puede realizar RESET: aun existe una condicion de FIRE u OVERHEAT."
            );
        }

        masterWarning = false;
        masterCaution = false;
        valvulaSOV_Cerrada = false;
        pantallaEICAS = "NORMAL";
    }

    bool hayFuego() const {
        return masterWarning;
    }

    bool hayOverheat() const {
        return masterCaution;
    }

    bool sovCerrada() const {
        return valvulaSOV_Cerrada;
    }

    string getMensajeEICAS() const {
        return pantallaEICAS;
    }

    bool botellaAUsada() const {
        return botellaA_Detonada;
    }

    bool botellaBUsada() const {
        return botellaB_Detonada;
    }

    int getNumeroMotor() const {
        return numeroMotor;
    }

private:
    string estadoLoop(const Loop& loop) const {
        if (loop.getEsFuego()) {
            return "FIRE";
        }

        if (loop.getEsOverheat()) {
            return "OVERHEAT";
        }

        return "NORMAL";
    }
};

class SistemaAvion {
private:
    ComputadoraMotor motor1;
    ComputadoraMotor motor2;

    string pilaHistorial[20];
    int tope;

    mt19937 generador;

public:
    SistemaAvion()
        : motor1(1),
          motor2(2),
          tope(-1),
          generador(
              static_cast<unsigned int>(time(nullptr))
          ) {}

    void menuPrincipal() {
        int opcion = 0;

        do {
            cout << "\n==============================================\n";
            cout << " SISTEMA DE DETECCION Y EXTINCION DE INCENDIOS\n";
            cout << "==============================================\n";
            cout << "1. Iniciar/Monitorear simulacion\n";
            cout << "2. Mostrar estado actual del motor\n";
            cout << "3. Disparar Botella A (SQUIB A)\n";
            cout << "4. Disparar Botella B (SQUIB B)\n";
            cout << "5. Ejecutar Auto-Test del sistema\n";
            cout << "6. Realizar RESET manual\n";
            cout << "7. Ver Historial de Eventos\n";
            cout << "8. Salir\n";
            cout << "Seleccione una opcion: ";

            try {
                opcion = leerEntero();

                switch (opcion) {
                    case 1:
                        monitorear();
                        break;

                    case 2:
                        mostrarEstadoActual();
                        break;

                    case 3:
                        dispararBotella();
                        break;

                    case 4:
                        dispararBotella();
                        break;

                    case 5:
                        autoTest();
                        break;

                    case 6:
                        reset();
                        break;

                    case 7:
                        mostrarHistorialPila();
                        break;

                    case 8:
                        cout << "Sistema finalizado.\n";
                        break;

                    default:
                        throw ExcepcionSistema(
                            "Opcion no valida."
                        );
                }
            }
            catch (const ExcepcionSistema& e) {
                cout << "ERROR: "
                     << e.queExcepcion()
                     << "\n";

                try {
                    apilarEvento(
                        "ERROR: " + e.queExcepcion()
                    );
                }
                catch (const ExcepcionSistema&) {
                }
            }

        } while (opcion != 8);
    }

private:
    int leerEntero() {
        int valor;

        if (!(cin >> valor)) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            throw ExcepcionSistema(
                "Se debe introducir un numero entero."
            );
        }

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        return valor;
    }

    ComputadoraMotor& seleccionarMotor() {
        cout << "Seleccione el motor (1 o 2): ";

        int numero = leerEntero();

        if (numero == 1) {
            return motor1;
        }

        if (numero == 2) {
            return motor2;
        }

        throw ExcepcionSistema(
            "Motor no valido. Seleccione 1 o 2."
        );
    }

    void monitorear() {
        motor1.actualizarEstado(generador);
        motor2.actualizarEstado(generador);

        registrarEstadoMotor(motor1);
        registrarEstadoMotor(motor2);

        cout << "\nSIMULACION ACTUALIZADA.\n";

        mostrarPanel(motor1);
        mostrarPanel(motor2);
    }

    void mostrarEstadoActual() const {
        motor1.mostrarEICAS();
        motor2.mostrarEICAS();
    }

    void dispararBotella() {
        ComputadoraMotor& motor =
            seleccionarMotor();

        cout << "Seleccione la botella (A o B): ";

        char botella;

        if (!(cin >> botella)) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            throw ExcepcionSistema(
                "Entrada no valida."
            );
        }

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        botella = static_cast<char>(
            toupper(
                static_cast<unsigned char>(botella)
            )
        );

        motor.dispararBotella(botella);

        string evento =
            "ENG " +
            to_string(motor.getNumeroMotor()) +
            " - SQUIB ";

        evento += botella;
        evento += " FIRED";

        apilarEvento(evento);

        cout << "Botella SQUIB "
             << botella
             << " del motor "
             << motor.getNumeroMotor()
             << " activada correctamente.\n";
    }

    void autoTest() {
        cout << "\n==============================================\n";
        cout << "               AUTO-TEST\n";
        cout << "==============================================\n";
        cout << "MASTER WARNING: OK\n";
        cout << "MASTER CAUTION: OK\n";
        cout << "SQUIB A: OK\n";
        cout << "SQUIB B: OK\n";
        cout << "SOV: OK\n";
        cout << "EICAS: OK\n";
        cout << "AUTO-TEST COMPLETADO\n";
        cout << "==============================================\n";

        apilarEvento(
            "AUTO-TEST COMPLETADO"
        );
    }

    void reset() {
        motor1.resetSistema();
        motor2.resetSistema();

        apilarEvento(
            "RESET DEL SISTEMA REALIZADO"
        );

        cout << "RESET realizado correctamente.\n";
    }

    void registrarEstadoMotor(
        const ComputadoraMotor& motor
    ) {
        if (motor.hayFuego()) {
            apilarEvento(
                "ENG " +
                to_string(motor.getNumeroMotor()) +
                " FIRE - MASTER WARNING - FUEL SOV CLOSED"
            );
        }
        else if (motor.hayOverheat()) {
            apilarEvento(
                "ENG " +
                to_string(motor.getNumeroMotor()) +
                " OVERHEAT - MASTER CAUTION"
            );
        }
        else {
            apilarEvento(
                "ENG " +
                to_string(motor.getNumeroMotor()) +
                " NORMAL"
            );
        }
    }

    void mostrarPanel(
        const ComputadoraMotor& motor
    ) const {
        if (motor.hayFuego()) {
            cout << "MASTER WARNING - ENG "
                 << motor.getNumeroMotor()
                 << " FIRE\n";

            cout << "FUEL SOV CLOSED\n";
        }
        else if (motor.hayOverheat()) {
            cout << "MASTER CAUTION - ENG "
                 << motor.getNumeroMotor()
                 << " OVERHEAT\n";
        }
        else {
            cout << "ENG "
                 << motor.getNumeroMotor()
                 << " NORMAL\n";
        }
    }

    void apilarEvento(
        const string& evento
    ) {
        if (tope >= 19) {
            throw ExcepcionSistema(
                "Desbordamiento de pila: no se pueden almacenar mas de 20 eventos."
            );
        }

        ++tope;
        pilaHistorial[tope] = evento;
    }

    void mostrarHistorialPila() const {
        cout << "\n==============================================\n";
        cout << "          HISTORIAL DE EVENTOS - PILA\n";
        cout << "==============================================\n";

        if (tope == -1) {
            cout << "No hay eventos registrados.\n";
        }
        else {
            for (int i = tope; i >= 0; --i) {
                cout << tope - i + 1
                     << ". "
                     << pilaHistorial[i]
                     << "\n";
            }
        }

        cout << "==============================================\n";
    }
};

int main() {
    try {
        SistemaAvion sistema;
        sistema.menuPrincipal();
    }
    catch (const exception& e) {
        cout << "ERROR CRITICO: "
             << e.what()
             << "\n";

        return 1;
    }

    return 0;
}