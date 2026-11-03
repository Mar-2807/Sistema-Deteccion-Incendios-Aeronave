#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

/*
===========================================================
 SISTEMA DE DETECCION, ALARMA Y EXTINCION AUTOMATICA
 DE INCENDIOS EN EL MOTOR DE UNA AERONAVE

 ELEMENTOS DE PROGRAMACION:
 1. Programacion Orientada a Objetos
 2. Clases y objetos
 3. Sobrecarga de operadores
 4. Excepciones
 5. Estructura lineal: Lista Enlazada
 6. Arreglos como conjuntos
 7. Operaciones de conjuntos:
      - Pertenencia
      - Union
      - Interseccion
      - Diferencia

 LOGICA DEL PROYECTO:
 LOOP A = sensores de temperatura
 LOOP B = sensores de flama

 NORMAL:
 A = 0 y B = 0

 ADVERTENCIA:
 A = 1 y B = 0

 EMERGENCIA:
 A = 1 y B = 1

 DISCREPANCIA:
 A = 0 y B = 1
===========================================================
*/



class ExcepcionSistema : public exception
{
private:
    string mensaje;

public:
    ExcepcionSistema(const string& msg)
        : mensaje(msg)
    {
    }

    const char* what() const noexcept override
    {
        return mensaje.c_str();
    }
};



class Sensor
{
protected:
    int id;
    string nombre;
    bool activo;
    bool operativo;

public:
    Sensor(int id = 0, string nombre = "")
    {
        this->id = id;
        this->nombre = nombre;
        activo = false;
        operativo = true;
    }

    virtual ~Sensor()
    {
    }

    int getId() const
    {
        return id;
    }

    string getNombre() const
    {
        return nombre;
    }

    bool estaActivo() const
    {
        return activo;
    }

    bool estaOperativo() const
    {
        return operativo;
    }

    void establecerFalla(bool falla)
    {
        operativo = !falla;

        if (falla)
        {
            activo = false;
        }
    }

    virtual void mostrar() const = 0;
};



class SensorTemperatura : public Sensor
{
private:
    double temperatura;

public:
    SensorTemperatura(int id, string nombre)
        : Sensor(id, nombre)
    {
        temperatura = 0;
    }

    void establecerTemperatura(double valor, double umbral)
    {
        if (valor < -100 || valor > 1000)
        {
            throw ExcepcionSistema(
                "Temperatura fuera del rango permitido para la simulacion."
            );
        }

        temperatura = valor;

        if (operativo)
        {
            activo = temperatura >= umbral;
        }
    }

    double getTemperatura() const
    {
        return temperatura;
    }

    void mostrar() const override
    {
        cout << nombre
             << " | Temperatura: "
             << temperatura
             << " C"
             << " | Estado: "
             << (activo ? "ACTIVO" : "NORMAL")
             << " | Operativo: "
             << (operativo ? "SI" : "NO")
             << endl;
    }
};



class SensorFlama : public Sensor
{
private:
    bool detectaFlama;

public:
    SensorFlama(int id, string nombre)
        : Sensor(id, nombre)
    {
        detectaFlama = false;
    }

    void establecerDeteccion(bool deteccion)
    {
        detectaFlama = deteccion;

        if (operativo)
        {
            activo = detectaFlama;
        }
    }

    bool getDeteccion() const
    {
        return detectaFlama;
    }

    void mostrar() const override
    {
        cout << nombre
             << " | Flama: "
             << (detectaFlama ? "DETECTADA" : "NO DETECTADA")
             << " | Estado: "
             << (activo ? "ACTIVO" : "NORMAL")
             << " | Operativo: "
             << (operativo ? "SI" : "NO")
             << endl;
    }
};


// ========================================================
// CLASE CONJUNTO
//
// Se utiliza un arreglo para representar un conjunto.
// Incluye:
// - Pertenencia
// - Union
// - Interseccion
// - Diferencia
//
// Tambien se utiliza SOBRECARGA DE OPERADORES:
// +  = union
// &  = interseccion
// -  = diferencia
// ========================================================

class Conjunto
{
private:
    static const int MAX = 20;
    int elementos[MAX];
    int cantidad;

    bool contieneInterno(int valor) const
    {
        for (int i = 0; i < cantidad; i++)
        {
            if (elementos[i] == valor)
            {
                return true;
            }
        }

        return false;
    }

public:
    Conjunto()
    {
        cantidad = 0;
    }

    void agregar(int valor)
    {
        if (cantidad >= MAX)
        {
            throw ExcepcionSistema(
                "El conjunto alcanzo su capacidad maxima."
            );
        }

        if (!contieneInterno(valor))
        {
            elementos[cantidad] = valor;
            cantidad++;
        }
    }

    // PERTENENCIA
    bool pertenece(int valor) const
    {
        return contieneInterno(valor);
    }

    // UNION
    Conjunto unionCon(const Conjunto& otro) const
    {
        Conjunto resultado;

        for (int i = 0; i < cantidad; i++)
        {
            resultado.agregar(elementos[i]);
        }

        for (int i = 0; i < otro.cantidad; i++)
        {
            resultado.agregar(otro.elementos[i]);
        }

        return resultado;
    }

    // INTERSECCION
    Conjunto interseccionCon(const Conjunto& otro) const
    {
        Conjunto resultado;

        for (int i = 0; i < cantidad; i++)
        {
            if (otro.pertenece(elementos[i]))
            {
                resultado.agregar(elementos[i]);
            }
        }

        return resultado;
    }

    // DIFERENCIA
    Conjunto diferenciaCon(const Conjunto& otro) const
    {
        Conjunto resultado;

        for (int i = 0; i < cantidad; i++)
        {
            if (!otro.pertenece(elementos[i]))
            {
                resultado.agregar(elementos[i]);
            }
        }

        return resultado;
    }

    // SOBRECARGA DE OPERADOR +
    // Representa la UNION
    Conjunto operator+(const Conjunto& otro) const
    {
        return unionCon(otro);
    }

    // SOBRECARGA DE OPERADOR &
    // Representa la INTERSECCION
    Conjunto operator&(const Conjunto& otro) const
    {
        return interseccionCon(otro);
    }

    // SOBRECARGA DE OPERADOR -
    // Representa la DIFERENCIA
    Conjunto operator-(const Conjunto& otro) const
    {
        return diferenciaCon(otro);
    }

    void mostrar() const
    {
        cout << "{ ";

        for (int i = 0; i < cantidad; i++)
        {
            cout << elementos[i];

            if (i < cantidad - 1)
            {
                cout << ", ";
            }
        }

        cout << " }" << endl;
    }
};


// ========================================================
// NODO DE LISTA ENLAZADA
// ========================================================

class Nodo
{
public:
    string dato;
    Nodo* siguiente;

    Nodo(string dato)
    {
        this->dato = dato;
        siguiente = nullptr;
    }
};


// ========================================================
// LISTA ENLAZADA
//
// Se utiliza para almacenar el historial de eventos
// del sistema.
// ========================================================

class ListaEnlazada
{
private:
    Nodo* cabeza;

public:
    ListaEnlazada()
    {
        cabeza = nullptr;
    }

    ~ListaEnlazada()
    {
        limpiar();
    }

    void agregar(string dato)
    {
        Nodo* nuevo = new Nodo(dato);

        if (cabeza == nullptr)
        {
            cabeza = nuevo;
        }
        else
        {
            Nodo* actual = cabeza;

            while (actual->siguiente != nullptr)
            {
                actual = actual->siguiente;
            }

            actual->siguiente = nuevo;
        }
    }

    void mostrar() const
    {
        if (cabeza == nullptr)
        {
            cout << "No hay eventos registrados." << endl;
            return;
        }

        Nodo* actual = cabeza;
        int contador = 1;

        while (actual != nullptr)
        {
            cout << contador
                 << ". "
                 << actual->dato
                 << endl;

            actual = actual->siguiente;
            contador++;
        }
    }

    void limpiar()
    {
        Nodo* actual = cabeza;

        while (actual != nullptr)
        {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }

        cabeza = nullptr;
    }
};


// ========================================================
// CLASE LOOP
//
// Representa cada uno de los dos loops del sistema.
// ========================================================

class Loop
{
private:
    string nombre;
    bool activo;

public:
    Loop(string nombre)
    {
        this->nombre = nombre;
        activo = false;
    }

    void establecerEstado(bool estado)
    {
        activo = estado;
    }

    bool estaActivo() const
    {
        return activo;
    }

    string getNombre() const
    {
        return nombre;
    }

    // SOBRECARGA DE OPERADOR ==
    bool operator==(const Loop& otro) const
    {
        return activo == otro.activo;
    }
};


// ========================================================
// CLASE SISTEMA CONTRA INCENDIOS
// ========================================================

class SistemaContraIncendios
{
private:


    SensorTemperatura sensorTemp1;
    SensorTemperatura sensorTemp2;

    SensorFlama sensorFlama1;
    SensorFlama sensorFlama2;



    Loop loopA;
    Loop loopB;



    bool bombaCombustible;
    bool valvulaCombustible;

    bool valvulaExtintor1;
    bool valvulaExtintor2;

    bool buzzer;

    bool ledVerde;
    bool ledAmarillo;
    bool ledRojo;



    string mensajeLCD;



    string estadoSistema;

    bool necesitaReset;

    double umbralTemperatura;


    ListaEnlazada historial;



    Conjunto conjuntoLoopA;
    Conjunto conjuntoLoopB;


public:

    SistemaContraIncendios(double umbral)
        : sensorTemp1(1, "Sensor de temperatura 1"),
          sensorTemp2(2, "Sensor de temperatura 2"),
          sensorFlama1(3, "Sensor de flama 1"),
          sensorFlama2(4, "Sensor de flama 2"),
          loopA("Loop A"),
          loopB("Loop B")
    {
        if (umbral <= 0)
        {
            throw ExcepcionSistema(
                "El umbral de temperatura debe ser mayor que cero."
            );
        }

        umbralTemperatura = umbral;

        bombaCombustible = true;
        valvulaCombustible = true;

        valvulaExtintor1 = false;
        valvulaExtintor2 = false;

        buzzer = false;

        ledVerde = true;
        ledAmarillo = false;
        ledRojo = false;

        mensajeLCD =
            "Sistema en monitoreo normal.";

        estadoSistema = "NORMAL";

        necesitaReset = false;

        actualizarConjuntos();

        historial.agregar(
            "Sistema iniciado en modo NORMAL."
        );
    }



    void ingresarLecturas()
    {
        double temperatura1;
        double temperatura2;

        int flama1;
        int flama2;

        cout << "\n========================================\n";
        cout << "       INGRESO DE LECTURAS\n";
        cout << "========================================\n";

        cout << "Temperatura del sensor 1 (C): ";
        cin >> temperatura1;

        cout << "Temperatura del sensor 2 (C): ";
        cin >> temperatura2;

        cout << "\nSensor de flama 1:\n";
        cout << "1 = Flama detectada\n";
        cout << "0 = No detectada\n";
        cout << "Seleccion: ";
        cin >> flama1;

        cout << "\nSensor de flama 2:\n";
        cout << "1 = Flama detectada\n";
        cout << "0 = No detectada\n";
        cout << "Seleccion: ";
        cin >> flama2;


        if ((flama1 != 0 && flama1 != 1) ||
            (flama2 != 0 && flama2 != 1))
        {
            throw ExcepcionSistema(
                "Los sensores de flama solamente aceptan 0 o 1."
            );
        }


        sensorTemp1.establecerTemperatura(
            temperatura1,
            umbralTemperatura
        );

        sensorTemp2.establecerTemperatura(
            temperatura2,
            umbralTemperatura
        );

        sensorFlama1.establecerDeteccion(
            flama1 == 1
        );

        sensorFlama2.establecerDeteccion(
            flama2 == 1
        );


        evaluarSistema();
    }


    // ====================================================
    // EVALUAR SISTEMA
    // ====================================================

    void evaluarSistema()
    {
        /*
        LOOP A:
        Se activa cuando existe una deteccion de
        temperatura anormal.

        OR:
        sensorTemp1 OR sensorTemp2
        */

        bool estadoLoopA =
            sensorTemp1.estaActivo() ||
            sensorTemp2.estaActivo();


        /*
        LOOP B:
        Se activa cuando cualquiera de los sensores
        de flama detecta combustion.

        OR:
        sensorFlama1 OR sensorFlama2
        */

        bool estadoLoopB =
            sensorFlama1.estaActivo() ||
            sensorFlama2.estaActivo();


        loopA.establecerEstado(estadoLoopA);
        loopB.establecerEstado(estadoLoopB);


        actualizarConjuntos();




        if (loopA.estaActivo() &&
            loopB.estaActivo())
        {
            modoEmergencia();
        }




        else if (loopA.estaActivo() &&
                 !loopB.estaActivo())
        {
            modoAdvertencia();
        }




        else if (!loopA.estaActivo() &&
                 loopB.estaActivo())
        {
            modoDiscrepancia();
        }



        else
        {
            modoNormal();
        }
    }


    void modoNormal()
    {
        if (necesitaReset)
        {
            cout << "\nEl sistema requiere RESET MANUAL.\n";
            return;
        }

        estadoSistema = "NORMAL";

        ledVerde = true;
        ledAmarillo = false;
        ledRojo = false;

        buzzer = false;

        bombaCombustible = true;
        valvulaCombustible = true;

        valvulaExtintor1 = false;
        valvulaExtintor2 = false;

        mensajeLCD =
            "Sistema en monitoreo normal.";

        historial.agregar(
            "Modo NORMAL: no se detectaron condiciones anormales."
        );
    }


    void modoAdvertencia()
    {
        estadoSistema = "ADVERTENCIA";

        ledVerde = false;
        ledAmarillo = true;
        ledRojo = false;

        bombaCombustible = true;
        valvulaCombustible = true;

        valvulaExtintor1 = false;
        valvulaExtintor2 = false;


        mensajeLCD =
            "Se detecto un riesgo de sobrecalentamiento por parte del Loop A en la zona del motor.";

        historial.agregar(
            "Loop A activo: riesgo de sobrecalentamiento."
        );


        cout << "\n========================================\n";
        cout << "              ADVERTENCIA\n";
        cout << "========================================\n";

        cout << mensajeLCD << endl;

        cout << "\nVerificando la condicion durante 3 segundos...\n";


        for (int i = 3; i >= 1; i--)
        {
            cout << i << "...\n";

            this_thread::sleep_for(
                chrono::seconds(1)
            );
        }


        cout << "\nCondicion persistente.\n";


        // Buzzer de baja frecuencia durante 15 segundos

        buzzer = true;

        cout << "\nBUZZER: CHIME DE BAJA FRECUENCIA\n";
        cout << "Duracion: 15 segundos\n";

        for (int i = 15; i >= 1; i--)
        {
            cout << "\rTiempo restante: "
                 << setw(2)
                 << i
                 << " segundos";

            cout.flush();

            this_thread::sleep_for(
                chrono::seconds(1)
            );
        }

        cout << endl;

        buzzer = false;

        cout << "\nLa condicion de advertencia permanece";
        cout << " mientras la temperatura sea anormal.\n";
    }




    void modoDiscrepancia()
    {
        estadoSistema = "DISCREPANCIA";

        ledVerde = false;
        ledAmarillo = true;
        ledRojo = false;

        buzzer = false;

        bombaCombustible = true;
        valvulaCombustible = true;

        mensajeLCD =
            "Loop B posible discrepancia.";

        historial.agregar(
            "Loop B activo sin activacion del Loop A: posible discrepancia."
        );

        cout << "\n========================================\n";
        cout << "        POSIBLE DISCREPANCIA\n";
        cout << "========================================\n";

        cout << mensajeLCD << endl;
    }


    void modoEmergencia()
    {
        estadoSistema = "EMERGENCIA";

        ledVerde = false;
        ledAmarillo = false;

        ledRojo = true;

        buzzer = true;

        mensajeLCD =
            "Incendio detectado en el motor. Activacion automatica del sistema.";


        historial.agregar(
            "EMERGENCIA: Loop A y Loop B activos. Incendio confirmado."
        );


        cout << "\n";
        cout << "****************************************\n";
        cout << "*              EMERGENCIA              *\n";
        cout << "****************************************\n";

        cout << "\n" << mensajeLCD << endl;


        bombaCombustible = false;
        valvulaCombustible = false;

        historial.agregar(
            "Bomba de combustible desactivada."
        );

        historial.agregar(
            "Valvula de combustible cerrada."
        );


        cout << "\nACCION 1: CORTE DE COMBUSTIBLE\n";
        cout << "Bomba: DESACTIVADA\n";
        cout << "Valvula: CERRADA\n";



        valvulaExtintor1 = true;

        historial.agregar(
            "Botella/extintor 1 activado."
        );

        cout << "\nACCION 2: SISTEMA DE EXTINCION\n";
        cout << "Extintor 1: ACTIVADO\n";


        // Se dispone de dos botellas por motor.
        // La segunda queda disponible para una siguiente
        // activacion si fuese necesaria.

        valvulaExtintor2 = false;

        cout << "Extintor 2: DISPONIBLE\n";



        cout << "\nACCION 3: ALARMA\n";
        cout << "MASTER WARNING: ACTIVADO\n";
        cout << "BUZZER: CHIME CONTINUO/REPETITIVO\n";



        cout << "\n----------------------------------------\n";
        cout << "ACCIONES DEL SISTEMA\n";
        cout << "----------------------------------------\n";

        cout << "1. Incendio confirmado.\n";
        cout << "2. Combustible interrumpido.\n";
        cout << "3. Valvula de combustible cerrada.\n";
        cout << "4. Agente extintor activado.\n";
        cout << "5. MASTER WARNING activo.\n";
        cout << "6. Buzzer activo.\n";
        cout << "----------------------------------------\n";


        cout << "\nEl sistema permanece en condicion de emergencia";
        cout << " hasta que termine la condicion critica.\n";


        // Para efectos de la simulacion se solicita al usuario
        // realizar el reset manual.

        necesitaReset = true;
    }


    void resetSistema()
    {
        sensorTemp1.establecerFalla(false);
        sensorTemp2.establecerFalla(false);

        sensorFlama1.establecerFalla(false);
        sensorFlama2.establecerFalla(false);


        // Los sensores se vuelven a evaluar con sus valores
        // actuales.

        loopA.establecerEstado(false);
        loopB.establecerEstado(false);


        bombaCombustible = true;
        valvulaCombustible = true;

        valvulaExtintor1 = false;
        valvulaExtintor2 = false;

        buzzer = false;

        ledVerde = true;
        ledAmarillo = false;
        ledRojo = false;

        mensajeLCD =
            "Sistema restablecido. Operacion normal.";

        estadoSistema = "NORMAL";

        necesitaReset = false;


        historial.agregar(
            "RESET MANUAL realizado: alarmas limpiadas y sistema restablecido."
        );


        actualizarConjuntos();


        cout << "\n========================================\n";
        cout << "           RESET REALIZADO\n";
        cout << "========================================\n";

        cout << "Alarmas: LIMPIADAS\n";
        cout << "LED VERDE: ENCENDIDO\n";
        cout << "Bomba: ACTIVADA\n";
        cout << "Valvula combustible: ABIERTA\n";
        cout << "Sistema: NORMAL\n";
    }



    void simularFalla()
    {
        int opcion;

        cout << "\n========================================\n";
        cout << "        SIMULACION DE FALLA\n";
        cout << "========================================\n";

        cout << "1. Sensor de temperatura 1\n";
        cout << "2. Sensor de temperatura 2\n";
        cout << "3. Sensor de flama 1\n";
        cout << "4. Sensor de flama 2\n";
        cout << "0. Cancelar\n";

        cout << "\nSeleccion: ";
        cin >> opcion;


        switch (opcion)
        {
        case 1:
            sensorTemp1.establecerFalla(true);
            historial.agregar(
                "Falla simulada en sensor de temperatura 1."
            );
            cout << "Falla simulada correctamente.\n";
            break;

        case 2:
            sensorTemp2.establecerFalla(true);
            historial.agregar(
                "Falla simulada en sensor de temperatura 2."
            );
            cout << "Falla simulada correctamente.\n";
            break;

        case 3:
            sensorFlama1.establecerFalla(true);
            historial.agregar(
                "Falla simulada en sensor de flama 1."
            );
            cout << "Falla simulada correctamente.\n";
            break;

        case 4:
            sensorFlama2.establecerFalla(true);
            historial.agregar(
                "Falla simulada en sensor de flama 2."
            );
            cout << "Falla simulada correctamente.\n";
            break;

        case 0:
            cout << "Operacion cancelada.\n";
            break;

        default:
            throw ExcepcionSistema(
                "Opcion de falla no valida."
            );
        }


        actualizarConjuntos();
    }


    void actualizarConjuntos()
    {
        conjuntoLoopA = Conjunto();
        conjuntoLoopB = Conjunto();


        if (sensorTemp1.estaActivo())
        {
            conjuntoLoopA.agregar(
                sensorTemp1.getId()
            );
        }

        if (sensorTemp2.estaActivo())
        {
            conjuntoLoopA.agregar(
                sensorTemp2.getId()
            );
        }


        if (sensorFlama1.estaActivo())
        {
            conjuntoLoopB.agregar(
                sensorFlama1.getId()
            );
        }

        if (sensorFlama2.estaActivo())
        {
            conjuntoLoopB.agregar(
                sensorFlama2.getId()
            );
        }
    }



    void mostrarEstado() const
    {
        cout << "\n========================================\n";
        cout << "       ESTADO DEL SISTEMA\n";
        cout << "========================================\n";


        cout << "\n--- SENSORES ---\n";

        sensorTemp1.mostrar();
        sensorTemp2.mostrar();

        sensorFlama1.mostrar();
        sensorFlama2.mostrar();


        cout << "\n--- LOOPS ---\n";

        cout << loopA.getNombre()
             << ": "
             << (loopA.estaActivo()
                     ? "ACTIVO"
                     : "INACTIVO")
             << endl;

        cout << loopB.getNombre()
             << ": "
             << (loopB.estaActivo()
                     ? "ACTIVO"
                     : "INACTIVO")
             << endl;


        cout << "\n--- ACTUADORES ---\n";

        cout << "Bomba de combustible: "
             << (bombaCombustible
                     ? "ACTIVADA"
                     : "DESACTIVADA")
             << endl;

        cout << "Valvula de combustible: "
             << (valvulaCombustible
                     ? "ABIERTA"
                     : "CERRADA")
             << endl;

        cout << "Valvula extintor 1: "
             << (valvulaExtintor1
                     ? "ACTIVADA"
                     : "DESACTIVADA")
             << endl;

        cout << "Valvula extintor 2: "
             << (valvulaExtintor2
                     ? "ACTIVADA"
                     : "DESACTIVADA")
             << endl;


        cout << "\n--- INDICADORES ---\n";

        cout << "LED VERDE: "
             << (ledVerde
                     ? "ENCENDIDO"
                     : "APAGADO")
             << endl;

        cout << "MASTER CAUTION / LED AMARILLO: "
             << (ledAmarillo
                     ? "ENCENDIDO"
                     : "APAGADO")
             << endl;

        cout << "MASTER WARNING / LED ROJO: "
             << (ledRojo
                     ? "ENCENDIDO"
                     : "APAGADO")
             << endl;

        cout << "BUZZER: "
             << (buzzer
                     ? "ACTIVO"
                     : "INACTIVO")
             << endl;


        cout << "\n--- LCD ---\n";
        cout << mensajeLCD << endl;


        cout << "\n--- ESTADO GENERAL ---\n";
        cout << estadoSistema << endl;


        if (necesitaReset)
        {
            cout << "\n*** SE REQUIERE RESET MANUAL ***\n";
        }
    }



    void mostrarOperacionesConjuntos()
    {
        actualizarConjuntos();


        cout << "\n========================================\n";
        cout << "        OPERACIONES DE CONJUNTOS\n";
        cout << "========================================\n";


        cout << "\nConjunto Loop A: ";
        conjuntoLoopA.mostrar();


        cout << "Conjunto Loop B: ";
        conjuntoLoopB.mostrar();


        cout << "\n--- PERTENENCIA ---\n";

        cout << "Sensor 1 pertenece al Loop A: "
             << (conjuntoLoopA.pertenece(1)
                     ? "SI"
                     : "NO")
             << endl;

        cout << "Sensor 2 pertenece al Loop A: "
             << (conjuntoLoopA.pertenece(2)
                     ? "SI"
                     : "NO")
             << endl;

        cout << "Sensor 3 pertenece al Loop B: "
             << (conjuntoLoopB.pertenece(3)
                     ? "SI"
                     : "NO")
             << endl;

        cout << "Sensor 4 pertenece al Loop B: "
             << (conjuntoLoopB.pertenece(4)
                     ? "SI"
                     : "NO")
             << endl;



        Conjunto unionConjuntos =
            conjuntoLoopA + conjuntoLoopB;

        cout << "\n--- UNION ---\n";
        cout << "A U B = ";
        unionConjuntos.mostrar();


        Conjunto interseccion =
            conjuntoLoopA & conjuntoLoopB;

        cout << "\n--- INTERSECCION ---\n";
        cout << "A INTERSECCION B = ";
        interseccion.mostrar();



        Conjunto diferencia =
            conjuntoLoopA - conjuntoLoopB;

        cout << "\n--- DIFERENCIA ---\n";
        cout << "A - B = ";
        diferencia.mostrar();
    }



    void mostrarHistorial() const
    {
        cout << "\n========================================\n";
        cout << "          HISTORIAL DEL SISTEMA\n";
        cout << "========================================\n";

        historial.mostrar();
    }



    void mostrarInformacion() const
    {
        cout << "\n========================================\n";
        cout << " SISTEMA DE PROTECCION CONTRA INCENDIOS\n";
        cout << "========================================\n";

        cout << "\nUmbral de temperatura configurado: "
             << umbralTemperatura
             << " C"
             << endl;

        cout << "\nLOGICA:\n";

        cout << "Loop A = Sensor temperatura 1 OR Sensor temperatura 2\n";
        cout << "Loop B = Sensor flama 1 OR Sensor flama 2\n";

        cout << "\nNORMAL:\n";
        cout << "Loop A = 0 AND Loop B = 0\n";

        cout << "\nADVERTENCIA:\n";
        cout << "Loop A = 1 AND Loop B = 0\n";

        cout << "\nEMERGENCIA:\n";
        cout << "Loop A = 1 AND Loop B = 1\n";

        cout << "\nDISCREPANCIA:\n";
        cout << "Loop A = 0 AND Loop B = 1\n";
    }
};


// ========================================================
// FUNCION PRINCIPAL
// ========================================================

int main()
{
    try
    {
        double umbral;

        cout << "========================================\n";
        cout << " SISTEMA DE DETECCION DE INCENDIO\n";
        cout << "      EN MOTOR DE AERONAVE\n";
        cout << "========================================\n";

        cout << "\nIngrese el umbral de temperatura";
        cout << " para considerar anormal la lectura (C): ";

        cin >> umbral;


        if (cin.fail())
        {
            throw ExcepcionSistema(
                "Debe introducir un valor numerico."
            );
        }


        SistemaContraIncendios sistema(umbral);


        int opcion;


        do
        {
            cout << "\n\n========================================\n";
            cout << "              MENU PRINCIPAL\n";
            cout << "========================================\n";

            cout << "1. Ingresar lecturas de sensores\n";
            cout << "2. Mostrar estado del sistema\n";
            cout << "3. Realizar RESET manual\n";
            cout << "4. Mostrar historial\n";
            cout << "5. Mostrar operaciones de conjuntos\n";
            cout << "6. Simular falla de un sensor\n";
            cout << "7. Mostrar informacion de la logica\n";
            cout << "0. Salir\n";

            cout << "\nSeleccione una opcion: ";
            cin >> opcion;


            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');

                throw ExcepcionSistema(
                    "La opcion debe ser un numero."
                );
            }


            switch (opcion)
            {
            case 1:
                sistema.ingresarLecturas();
                break;


            case 2:
                sistema.mostrarEstado();
                break;


            case 3:
                sistema.resetSistema();
                break;


            case 4:
                sistema.mostrarHistorial();
                break;


            case 5:
                sistema.mostrarOperacionesConjuntos();
                break;


            case 6:
                sistema.simularFalla();
                break;


            case 7:
                sistema.mostrarInformacion();
                break;


            case 0:
                cout << "\nPrograma finalizado.\n";
                break;


            default:
                cout << "\nOpcion no valida.\n";
            }

        }
        while (opcion != 0);
    }


    catch (const ExcepcionSistema& e)
    {
        cout << "\n========================================\n";
        cout << "              ERROR\n";
        cout << "========================================\n";

        cout << e.what() << endl;
    }


    catch (const exception& e)
    {
        cout << "\nError inesperado: "
             << e.what()
             << endl;
    }


    return 0;
}