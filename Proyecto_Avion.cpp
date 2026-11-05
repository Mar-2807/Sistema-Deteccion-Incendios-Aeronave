#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <limits>
#include <iomanip>

using namespace std;


namespace Configuracion
{
    const double TEMPERATURA_TOLERANCIA = 80.0;

    const double TEMPERATURA_MINIMA = 60.0;
    const double TEMPERATURA_MAXIMA = 100.0;

    const int TIEMPO_VERIFICACION = 3;

    const int MAX_ELEMENTOS_CONJUNTO = 20;
}


namespace Interfaz
{
    const string RESET = "\033[0m";
    const string ROJO = "\033[31m";
    const string VERDE = "\033[32m";
    const string AMARILLO = "\033[33m";
    const string AZUL = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN = "\033[36m";
    const string BLANCO = "\033[37m";
    const string NEGRITA = "\033[1m";
}



class ExcepcionSistema : public exception
{
private:
    string mensaje;

public:

    explicit ExcepcionSistema(const string& mensaje)
        : mensaje(mensaje)
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
    bool operativo;

public:

    Sensor(int id, const string& nombre)
        : id(id),
          nombre(nombre),
          operativo(true)
    {
    }

    virtual ~Sensor() = default;

    int obtenerId() const
    {
        return id;
    }

    string obtenerNombre() const
    {
        return nombre;
    }

    bool estaOperativo() const
    {
        return operativo;
    }

    virtual void mostrar() const = 0;
};


class SensorTemperatura : public Sensor
{
private:

    double temperatura;

public:

    explicit SensorTemperatura(int id)
        : Sensor(id, "Sensor de temperatura"),
          temperatura(0.0)
    {
    }

    void establecerTemperatura(double valor)
    {
        if (valor < -100.0 || valor > 1000.0)
        {
            throw ExcepcionSistema(
                "Lectura de temperatura fuera del rango permitido."
            );
        }

        temperatura = valor;
    }

    double obtenerTemperatura() const
    {
        return temperatura;
    }

    bool superaTolerancia(double tolerancia) const
    {
        return temperatura > tolerancia;
    }

    void mostrar() const override
    {
        cout << fixed << setprecision(1)
             << "Sensor " << id
             << "  |  "
             << temperatura
             << " °C";

        if (temperatura > Configuracion::TEMPERATURA_TOLERANCIA)
        {
            cout << "  |  SOBRE TOLERANCIA";
        }
        else
        {
            cout << "  |  NORMAL";
        }
    }
};


class Loop
{
private:

    string nombre;
    int sensor1;
    int sensor2;
    bool activo;

public:

    Loop(
        const string& nombre,
        int sensor1,
        int sensor2
    )
        : nombre(nombre),
          sensor1(sensor1),
          sensor2(sensor2),
          activo(false)
    {
    }

    void establecerEstado(bool estado)
    {
        activo = estado;
    }

    bool estaActivo() const
    {
        return activo;
    }

    string obtenerNombre() const
    {
        return nombre;
    }

    int obtenerSensor1() const
    {
        return sensor1;
    }

    int obtenerSensor2() const
    {
        return sensor2;
    }

    

    bool operator==(const Loop& otro) const
    {
        return nombre == otro.nombre &&
               sensor1 == otro.sensor1 &&
               sensor2 == otro.sensor2;
    }

    void mostrar() const
    {
        cout << nombre << " : ";

        if (activo)
        {
            cout << Interfaz::ROJO
                 << "ACTIVO"
                 << Interfaz::RESET;
        }
        else
        {
            cout << Interfaz::VERDE
                 << "NORMAL"
                 << Interfaz::RESET;
        }

        cout << endl;

        cout << "       Sensores asociados: "
             << sensor1 << " y "
             << sensor2 << endl;
    }
};


class Conjunto
{
private:

    int elementos[Configuracion::MAX_ELEMENTOS_CONJUNTO];
    int cantidad;

public:

    Conjunto()
        : cantidad(0)
    {
    }


    bool pertenece(int valor) const
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


    void agregar(int valor)
    {
        if (cantidad >= Configuracion::MAX_ELEMENTOS_CONJUNTO)
        {
            throw ExcepcionSistema(
                "Se alcanzo el limite maximo del conjunto."
            );
        }

        if (!pertenece(valor))
        {
            elementos[cantidad] = valor;
            cantidad++;
        }
    }

    // --------------------------------------------------------------
    // UNION
    // --------------------------------------------------------------

    Conjunto unionCon(const Conjunto& otro) const
    {
        Conjunto resultado = *this;

        for (int i = 0; i < otro.cantidad; i++)
        {
            resultado.agregar(otro.elementos[i]);
        }

        return resultado;
    }

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


    // + = UNION
    Conjunto operator+(const Conjunto& otro) const
    {
        return unionCon(otro);
    }

    // & = INTERSECCION
    Conjunto operator&(const Conjunto& otro) const
    {
        return interseccionCon(otro);
    }

    // - = DIFERENCIA
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

        cout << " }";
    }
};


class Nodo
{
public:

    string dato;
    Nodo* siguiente;

    explicit Nodo(const string& dato)
        : dato(dato),
          siguiente(nullptr)
    {
    }
};


class ListaEnlazada
{
private:

    Nodo* cabeza;

public:

    ListaEnlazada()
        : cabeza(nullptr)
    {
    }

    ~ListaEnlazada()
    {
        Nodo* actual = cabeza;

        while (actual != nullptr)
        {
            Nodo* temporal = actual;

            actual = actual->siguiente;

            delete temporal;
        }
    }


    void insertar(const string& dato)
    {
        Nodo* nuevo = new Nodo(dato);

        if (cabeza == nullptr)
        {
            cabeza = nuevo;
            return;
        }

        Nodo* actual = cabeza;

        while (actual->siguiente != nullptr)
        {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }


    void mostrar() const
    {
        if (cabeza == nullptr)
        {
            cout << "No existen eventos registrados.\n";
            return;
        }

        Nodo* actual = cabeza;

        int contador = 1;

        while (actual != nullptr)
        {
            cout << "  "
                 << contador
                 << ". "
                 << actual->dato
                 << endl;

            actual = actual->siguiente;

            contador++;
        }
    }
};



class Deteccion
{
private:

   
    SensorTemperatura sensor1;
    SensorTemperatura sensor2;



    SensorTemperatura sensor3;
    SensorTemperatura sensor4;

    Loop loopA;
    Loop loopB;

    double temperaturaTolerancia;

    random_device dispositivoAleatorio;
    mt19937 generador;

    uniform_real_distribution<double>
        distribucionTemperatura;

public:

    explicit Deteccion(double tolerancia)
        : sensor1(1),
          sensor2(2),
          sensor3(3),
          sensor4(4),

          loopA("LOOP A", 1, 2),
          loopB("LOOP B", 3, 4),

          temperaturaTolerancia(tolerancia),

          generador(dispositivoAleatorio()),

          distribucionTemperatura(
              Configuracion::TEMPERATURA_MINIMA,
              Configuracion::TEMPERATURA_MAXIMA
          )
    {
        if (tolerancia <= 0)
        {
            throw ExcepcionSistema(
                "La temperatura de tolerancia debe ser mayor que cero."
            );
        }
    }

    void generarLecturas()
    {
        sensor1.establecerTemperatura(
            distribucionTemperatura(generador)
        );

        sensor2.establecerTemperatura(
            distribucionTemperatura(generador)
        );

        sensor3.establecerTemperatura(
            distribucionTemperatura(generador)
        );

        sensor4.establecerTemperatura(
            distribucionTemperatura(generador)
        );
    }


    void evaluarLoops()
    {
        bool s1 =
            sensor1.superaTolerancia(
                temperaturaTolerancia
            );

        bool s2 =
            sensor2.superaTolerancia(
                temperaturaTolerancia
            );

        bool s3 =
            sensor3.superaTolerancia(
                temperaturaTolerancia
            );

        bool s4 =
            sensor4.superaTolerancia(
                temperaturaTolerancia
            );


        loopA.establecerEstado(
            s1 && s2
        );

        loopB.establecerEstado(
            s3 && s4
        );
    }

    bool loopAActivo() const
    {
        return loopA.estaActivo();
    }


    bool loopBActivo() const
    {
        return loopB.estaActivo();
    }


    void mostrarLecturas() const
    {
        cout << "\n";
        cout << Interfaz::CYAN
             << "--------------------------------------------------\n"
             << "              MONITOREO DEL MOTOR\n"
             << "--------------------------------------------------\n"
             << Interfaz::RESET;

        cout << "Temperatura de tolerancia: "
             << temperaturaTolerancia
             << " °C\n\n";

        sensor1.mostrar();
        cout << endl;

        sensor2.mostrar();
        cout << endl;

        sensor3.mostrar();
        cout << endl;

        sensor4.mostrar();
        cout << endl;
    }

    void mostrarLoops() const
    {
        cout << "\n";
        cout << "Estado de los loops:\n";

        loopA.mostrar();
        loopB.mostrar();
    }


    Conjunto obtenerConjuntoLoopA() const
    {
        Conjunto A;

        A.agregar(1);
        A.agregar(2);

        return A;
    }


    Conjunto obtenerConjuntoLoopB() const
    {
        Conjunto B;

        B.agregar(3);
        B.agregar(4);

        return B;
    }
};



class Extincion
{
private:

    bool botella1Disponible;
    bool botella2Disponible;

    bool botella1Activada;
    bool botella2Activada;

public:

    Extincion()
        : botella1Disponible(true),
          botella2Disponible(true),
          botella1Activada(false),
          botella2Activada(false)
    {
    }


    void activarBotella1()
    {
        if (!botella1Disponible)
        {
            throw ExcepcionSistema(
                "La botella 1 ya fue utilizada."
            );
        }

        botella1Disponible = false;
        botella1Activada = true;
    }


    void activarBotella2()
    {
        if (!botella2Disponible)
        {
            throw ExcepcionSistema(
                "La botella 2 ya fue utilizada."
            );
        }

        botella2Disponible = false;
        botella2Activada = true;
    }

    bool botella1DisponibleEstado() const
    {
        return botella1Disponible;
    }

    bool botella2DisponibleEstado() const
    {
        return botella2Disponible;
    }


    void mostrarEstado() const
    {
        cout << "\n";
        cout << Interfaz::CYAN
             << "--------------------------------------------------\n"
             << "              SISTEMA DE EXTINCION\n"
             << "--------------------------------------------------\n"
             << Interfaz::RESET;

        cout << "Botella 1: ";

        if (botella1Disponible)
        {
            cout << Interfaz::VERDE
                 << "DISPONIBLE"
                 << Interfaz::RESET;
        }
        else
        {
            cout << Interfaz::ROJO
                 << "UTILIZADA"
                 << Interfaz::RESET;
        }

        cout << endl;

        cout << "Botella 2: ";

        if (botella2Disponible)
        {
            cout << Interfaz::VERDE
                 << "DISPONIBLE"
                 << Interfaz::RESET;
        }
        else
        {
            cout << Interfaz::ROJO
                 << "UTILIZADA"
                 << Interfaz::RESET;
        }

        cout << endl;
    }
};


enum class EstadoSistema
{
    NORMAL,
    ADVERTENCIA,
    DISCREPANCIA,
    EMERGENCIA
};


class SistemaContraIncendios
{
private:

    Deteccion deteccion;
    Extincion extincion;

    ListaEnlazada historial;

    EstadoSistema estadoActual;

public:

    explicit SistemaContraIncendios(double tolerancia)
        : deteccion(tolerancia),
          estadoActual(EstadoSistema::NORMAL)
    {
    }


    void ejecutarCiclo()
    {
        cout << "\n";
        cout << Interfaz::AZUL
             << "==================================================\n"
             << "              CICLO DE MONITOREO\n"
             << "==================================================\n"
             << Interfaz::RESET;

        deteccion.generarLecturas();

        deteccion.evaluarLoops();

        deteccion.mostrarLecturas();

        evaluarEstado();
    }


    void evaluarEstado()
    {
        bool A = deteccion.loopAActivo();
        bool B = deteccion.loopBActivo();

        cout << "\n";
        cout << Interfaz::CYAN
             << "--------------------------------------------------\n"
             << "              INDICACIONES DE CABINA\n"
             << "--------------------------------------------------\n"
             << Interfaz::RESET;


        if (!A && !B)
        {
            estadoActual = EstadoSistema::NORMAL;

            cout << Interfaz::VERDE
                 << "ESTADO: NORMAL\n"
                 << Interfaz::RESET;

            cout << "MASTER CAUTION : APAGADO\n";
            cout << "MASTER WARNING : APAGADO\n";

            cout << "\nNo se detectan condiciones anormales.\n";

            historial.insertar(
                "Sistema en estado normal."
            );
        }

   
        else if (A && !B)
        {
            estadoActual = EstadoSistema::ADVERTENCIA;

            cout << Interfaz::AMARILLO
                 << "MASTER CAUTION\n"
                 << Interfaz::RESET;

            cout << "Incremento anomalo de temperatura.\n";
            cout << "Loop A activo.\n";

            cout << "\nVerificando condicion durante "
                 << Configuracion::TIEMPO_VERIFICACION
                 << " segundos...\n";

            /*
                Se mantiene la verificacion de 3 segundos
                indicada en la logica del proyecto.
            */

            this_thread::sleep_for(
                chrono::seconds(
                    Configuracion::TIEMPO_VERIFICACION
                )
            );

            cout << "Condicion verificada.\n";
            cout << "MASTER CAUTION permanece activa.\n";

            historial.insertar(
                "MASTER CAUTION: incremento anomalo de temperatura detectado por Loop A."
            );
        }


        else if (!A && B)
        {
            estadoActual = EstadoSistema::DISCREPANCIA;

            cout << Interfaz::AMARILLO
                 << "LOOP B POSIBLE DISCREPANCIA\n"
                 << Interfaz::RESET;

            cout << "Loop B presenta una condicion de deteccion.\n";
            cout << "Loop A permanece dentro del rango normal.\n";

            historial.insertar(
                "Posible discrepancia detectada en Loop B."
            );
        }

        else
        {
            estadoActual = EstadoSistema::EMERGENCIA;

            cout << Interfaz::ROJO
                 << "**************************************************\n"
                 << "                 MASTER WARNING\n"
                 << "**************************************************\n"
                 << Interfaz::RESET;

            cout << Interfaz::ROJO
                 << "INCENDIO DETECTADO EN EL MOTOR\n"
                 << Interfaz::RESET;

            cout << "\n";
            cout << "Loop A : ACTIVO\n";
            cout << "Loop B : ACTIVO\n";

            cout << "\n";
            cout << Interfaz::ROJO
                 << "CONDICION DE INCENDIO CONFIRMADA\n"
                 << Interfaz::RESET;

            cout << "El sistema presenta la indicacion de emergencia.\n";

            historial.insertar(
                "MASTER WARNING: incendio confirmado por ambos loops."
            );

            mostrarEstadoExtincion();
        }
    }


    void mostrarEstadoExtincion()
    {
        cout << "\n";
        cout << Interfaz::CYAN
             << "--------------------------------------------------\n"
             << "             RECURSOS DE EXTINCION\n"
             << "--------------------------------------------------\n"
             << Interfaz::RESET;

        extincion.mostrarEstado();

        cout << "\nEl piloto puede seleccionar una botella de extincion.\n";
    }


    void seleccionarBotella()
    {
        if (estadoActual != EstadoSistema::EMERGENCIA)
        {
            cout << "\n";
            cout << Interfaz::AMARILLO
                 << "No existe una condicion de incendio confirmada.\n"
                 << Interfaz::RESET;

            return;
        }

        int opcion;

        do
        {
            extincion.mostrarEstado();

            cout << "\n";
            cout << "1. Activar botella 1\n";
            cout << "2. Activar botella 2\n";
            cout << "0. Regresar\n";

            cout << "\nSeleccione una opcion: ";

            cin >> opcion;

            if (cin.fail())
            {
                cin.clear();

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                throw ExcepcionSistema(
                    "La opcion debe ser un valor numerico."
                );
            }

            try
            {
                switch (opcion)
                {
                    case 1:

                        extincion.activarBotella1();

                        cout << "\n";
                        cout << Interfaz::VERDE
                             << "BOTELLA 1 ACTIVADA\n"
                             << Interfaz::RESET;

                        cout << "Se ha utilizado el primer disparo "
                                "de extincion.\n";

                        historial.insertar(
                            "Botella 1 de extincion activada."
                        );

                        break;

                    case 2:

                        extincion.activarBotella2();

                        cout << "\n";
                        cout << Interfaz::VERDE
                             << "BOTELLA 2 ACTIVADA\n"
                             << Interfaz::RESET;

                        cout << "Se ha utilizado el segundo disparo "
                                "de extincion.\n";

                        historial.insertar(
                            "Botella 2 de extincion activada."
                        );

                        break;

                    case 0:

                        break;

                    default:

                        cout << "\n";
                        cout << Interfaz::AMARILLO
                             << "Opcion no valida.\n"
                             << Interfaz::RESET;
                }
            }
            catch (const ExcepcionSistema& e)
            {
                cout << "\n";
                cout << Interfaz::ROJO
                     << "ERROR: "
                     << e.what()
                     << Interfaz::RESET
                     << endl;
            }

        } while (opcion != 0);
    }


    void mostrarExtincion() const
    {
        extincion.mostrarEstado();
    }


    void mostrarHistorial() const
    {
        cout << "\n";
        cout << Interfaz::CYAN
             << "--------------------------------------------------\n"
             << "                HISTORIAL DEL SISTEMA\n"
             << "--------------------------------------------------\n"
             << Interfaz::RESET;

        historial.mostrar();
    }


    void mostrarConjuntos() const
    {
        Conjunto A =
            deteccion.obtenerConjuntoLoopA();

        Conjunto B =
            deteccion.obtenerConjuntoLoopB();

        cout << "\n";
        cout << Interfaz::CYAN
             << "--------------------------------------------------\n"
             << "           ESTRUCTURA DE CONJUNTOS\n"
             << "--------------------------------------------------\n"
             << Interfaz::RESET;

        cout << "\nLoop A = ";
        A.mostrar();

        cout << "\nLoop B = ";
        B.mostrar();

        cout << "\n\n";


        cout << "PERTENENCIA\n";

        cout << "Sensor 1 pertenece a Loop A: ";

        if (A.pertenece(1))
        {
            cout << "SI\n";
        }
        else
        {
            cout << "NO\n";
        }

        cout << "Sensor 4 pertenece a Loop A: ";

        if (A.pertenece(4))
        {
            cout << "SI\n";
        }
        else
        {
            cout << "NO\n";
        }


        Conjunto unionAB = A + B;

        cout << "\nUNION A + B = ";
        unionAB.mostrar();


        Conjunto interseccionAB = A & B;

        cout << "\nINTERSECCION A & B = ";
        interseccionAB.mostrar();


        Conjunto diferenciaAB = A - B;

        cout << "\nDIFERENCIA A - B = ";
        diferenciaAB.mostrar();

        cout << "\n";
    }


    void reset()
    {
        estadoActual = EstadoSistema::NORMAL;

        cout << "\n";
        cout << Interfaz::VERDE
             << "--------------------------------------------------\n"
             << "                SISTEMA RESTABLECIDO\n"
             << "--------------------------------------------------\n"
             << Interfaz::RESET;

        cout << "Las indicaciones de emergencia han sido reiniciadas.\n";

        historial.insertar(
            "Sistema restablecido mediante procedimiento de reset."
        );
    }
};


void mostrarEncabezado()
{
    cout << "\n";

    cout << Interfaz::AZUL
         << "==================================================\n"
         << "       SISTEMA DE DETECCION Y EXTINCION\n"
         << "                  DE AERONAVE\n"
         << "==================================================\n"
         << Interfaz::RESET;

    cout << "             MONITOREO DEL MOTOR\n";
}



int main()
{
    try
    {
        mostrarEncabezado();


        SistemaContraIncendios sistema(
            Configuracion::TEMPERATURA_TOLERANCIA
        );

        int opcion;

        do
        {
            cout << "\n";
            cout << Interfaz::CYAN
                 << "==================================================\n"
                 << "                    MENU\n"
                 << "==================================================\n"
                 << Interfaz::RESET;

            cout << "1. Ejecutar ciclo de monitoreo\n";
            cout << "2. Sistema de extincion\n";
            cout << "3. Seleccionar botella de extincion\n";
            cout << "4. Mostrar historial de eventos\n";
            cout << "5. Mostrar conjuntos de sensores\n";
            cout << "6. Restablecer sistema\n";
            cout << "0. Salir\n";

            cout << "\nSeleccione una opcion: ";

            cin >> opcion;

            if (cin.fail())
            {
                cin.clear();

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                throw ExcepcionSistema(
                    "La opcion debe ser numerica."
                );
            }

            switch (opcion)
            {

                case 1:

                    sistema.ejecutarCiclo();

                    break;

                case 2:

                    sistema.mostrarExtincion();

                    break;

             

                case 3:

                    sistema.seleccionarBotella();

                    break;


                case 4:

                    sistema.mostrarHistorial();

                    break;

   

                case 5:

                    sistema.mostrarConjuntos();

                    break;

        
                case 6:

                    sistema.reset();

                    break;


                case 0:

                    cout << "\n";
                    cout << Interfaz::VERDE
                         << "Sistema finalizado correctamente.\n"
                         << Interfaz::RESET;

                    break;


                default:

                    cout << "\n";
                    cout << Interfaz::AMARILLO
                         << "La opcion seleccionada no existe.\n"
                         << Interfaz::RESET;
            }

        } while (opcion != 0);
    }
    catch (const ExcepcionSistema& e)
    {
        cout << "\n";
        cout << Interfaz::ROJO
             << "ERROR DEL SISTEMA\n"
             << Interfaz::RESET;

        cout << e.what() << endl;
    }
    catch (const exception& e)
    {
        cout << "\n";
        cout << Interfaz::ROJO
             << "ERROR INESPERADO\n"
             << Interfaz::RESET;

        cout << e.what() << endl;
    }

    return 0;
}
