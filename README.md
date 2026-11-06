# Sistema de Detección y Extinción de Incendios de Motor

## Descripción

Este proyecto consiste en la implementación en C++ de un sistema simulado de detección y extinción de incendios en motores de una aeronave.

El programa representa de forma simplificada la arquitectura de un sistema aeronáutico de detección y extinción de incendios. La información de temperatura es generada automáticamente por el programa y procesada mediante sensores, loops y computadoras de motor.

El sistema permite identificar condiciones normales, sobretemperatura e incendio, además de mostrar las indicaciones correspondientes en cabina y controlar de manera secuencial el sistema de extinción.

La arquitectura principal del programa es:

**Sensor → Loop → ComputadoraMotor → SistemaAvion**

---

## Objetivo

Desarrollar un programa orientado a objetos que simule el funcionamiento básico de un sistema de detección y extinción de incendios de motor, aplicando conceptos de:

- Programación Orientada a Objetos (POO).
- Encapsulamiento.
- Composición entre clases.
- Sobrecarga de operadores.
- Manejo de excepciones.
- Estructuras de datos lineales.
- Arreglos estáticos.
- Generación aleatoria de datos.
- Lógica de detección mediante sensores y loops.

---

## Funcionamiento del sistema

Cada motor cuenta con:

- 2 loops de detección.
- 2 sensores de temperatura por loop.
- 2 botellas de extinción.
- Una válvula SOV para el corte automático del suministro de combustible.

Por lo tanto, cada motor cuenta con 4 sensores de temperatura en total.

Los sensores generan automáticamente valores de temperatura dentro de un rango establecido por el programa. El usuario no introduce manualmente las temperaturas ni los valores de los umbrales.

### Umbrales de temperatura

El programa utiliza los siguientes valores:

| Condición | Temperatura |
|---|---:|
| Normal | Menor a 350 °C |
| Overheat | 350 °C o mayor |
| Fire | 600 °C o mayor |

---

## Lógica de detección

Cada loop contiene dos sensores.

El sistema analiza las condiciones de ambos sensores y determina el estado del loop.

Para confirmar un incendio en el motor se utiliza la lógica:

**Loop A en FIRE AND Loop B en FIRE**

Cuando ambos loops confirman la condición de incendio:

1. Se activa `MASTER WARNING`.
2. El sistema muestra la condición de FIRE.
3. Se cierra automáticamente la válvula SOV.
4. El piloto puede activar el sistema de extinción.

Si existe una condición de sobretemperatura sin incendio confirmado:

`MASTER CAUTION`

Si Loop B detecta una condición de incendio sin que Loop A la confirme, el sistema puede mostrar una indicación de posible discrepancia de Loop B.

---

## Sistema de extinción

Cada motor dispone de dos botellas de extinción:

- SQUIB A
- SQUIB B

Las botellas se utilizan de manera secuencial.

### Primera botella

Cuando existe un incendio confirmado, el piloto puede activar:

**SQUIB A**

La primera botella queda marcada como utilizada.

### Segunda botella

Si después de utilizar SQUIB A el incendio continúa, el piloto puede activar:

**SQUIB B**

SQUIB B solamente puede utilizarse después de que SQUIB A haya sido utilizada.

El programa evita que una misma botella pueda utilizarse más de una vez.

---

## Indicaciones en cabina

El programa no simula físicamente componentes como LEDs, buzzers o válvulas mediante objetos independientes.

En su lugar, representa las indicaciones que recibiría el piloto mediante mensajes en pantalla, por ejemplo:

- `MASTER CAUTION`
- `MASTER WARNING`
- `ENGINE FIRE`
- `FUEL SOV CLOSED`
- `SQUIB A FIRED`
- `SQUIB B FIRED`
- `LOOP B POSSIBLE DISCREPANCY`

De esta manera, la interfaz representa principalmente la información que tendría disponible la tripulación.

---

## Arquitectura de clases

### Sensor

Representa un sensor de temperatura.

Sus principales funciones son:

- Generar una lectura de temperatura.
- Determinar si existe sobretemperatura.
- Determinar si existe condición de incendio.
- Almacenar el identificador del sensor y su temperatura actual.

### Loop

Representa un loop de detección.

Cada loop contiene dos sensores de temperatura.

Sus funciones principales son:

- Obtener las lecturas de sus sensores.
- Determinar el estado del loop.
- Identificar condiciones de overheat.
- Identificar condiciones de fire.

También implementa sobrecarga del operador `==`.

### ComputadoraMotor

Representa la computadora encargada de procesar la información de un motor.

Contiene:

- Loop A.
- Loop B.
- Estado de la válvula SOV.
- Estado de las botellas de extinción.
- Estado de MASTER CAUTION.
- Estado de MASTER WARNING.
- Estado actual del motor.

Sus funciones incluyen:

- Generar las lecturas de los sensores.
- Procesar las condiciones de los loops.
- Confirmar un incendio.
- Activar el corte de combustible.
- Activar las botellas de extinción.
- Ejecutar el auto-test.
- Realizar el reset cuando las condiciones lo permiten.

### SistemaAvion

Representa el sistema general de la aeronave.

Contiene las computadoras correspondientes a los motores y administra:

- Monitoreo de los motores.
- Visualización de información.
- Activación de extinción.
- Auto-test.
- Reset.
- Historial de eventos.

---

## Estructura de datos

El proyecto utiliza una estructura lineal para almacenar el historial de eventos.

Se implementa una **pila (stack)** mediante un arreglo estático.

La pila tiene una capacidad máxima de:

**20 eventos**

Los eventos se almacenan conforme ocurren y pueden consultarse posteriormente siguiendo el principio:

**LIFO — Last In, First Out**

Es decir, el último evento registrado es el primero que se recupera.

No se utilizan operaciones de:

- Pertenencia.
- Unión.
- Intersección.
- Diferencia.

---

## Manejo de excepciones

El programa incluye la clase:

`ExcepcionSistema`

Esta clase permite manejar situaciones no válidas durante la ejecución del sistema.

Por ejemplo:

- Intentar activar una botella que ya fue utilizada.
- Intentar utilizar SQUIB B antes de SQUIB A.
- Intentar realizar un reset cuando todavía existe una condición de FIRE u OVERHEAT.
- Otras operaciones que no sean permitidas por la lógica del sistema.

Las excepciones permiten evitar que una operación incorrecta provoque un comportamiento inesperado del programa.

---

## Sobrecarga de operadores

El proyecto utiliza sobrecarga de operadores como parte de la implementación de POO.

La clase `Loop` utiliza el operador:

`==`

Este operador permite comparar el estado de dos objetos `Loop`.

---

## Auto-test

El sistema cuenta con una función de auto-test.

El auto-test permite comprobar el funcionamiento básico de los componentes principales del sistema y verificar que las condiciones necesarias para la simulación estén disponibles.

---

## Reset del sistema

El reset es manual.

El sistema solamente permite realizar un reset cuando no existe una condición activa de:

- FIRE.
- OVERHEAT.

Esto evita borrar una condición de alarma mientras el problema continúa presente.

---

## Tecnologías utilizadas

- Lenguaje: **C++**
- Paradigma: **Programación Orientada a Objetos**
- Compilador: **g++**
- Entorno recomendado: **MSYS2 / MinGW**
- Control de versiones: **Git y GitHub**

---

## Requisitos

Para ejecutar el proyecto se necesita:

- Windows.
- Compilador C++ compatible con C++11 o superior.
- `g++`.
- Git, si se desea trabajar con el repositorio de GitHub.

---

## Compilación

Desde la terminal, ubicándose en la carpeta donde se encuentra el archivo `.cpp`, se puede compilar con:

```bash
g++ -std=c++11 main.cpp -o sistema_incendios
```

Después de compilar, se ejecuta con:

```bash
./sistema_incendios
```

En Windows también puede ejecutarse como:

```bash
sistema_incendios.exe
```

---

## Menú del programa

El programa cuenta con un menú para realizar las principales operaciones del sistema, entre ellas:

1. Monitorear motores.
2. Consultar información de un motor.
3. Activar una botella de extinción.
4. Ejecutar el auto-test.
5. Realizar reset.
6. Consultar el historial de eventos.
7. Salir.
