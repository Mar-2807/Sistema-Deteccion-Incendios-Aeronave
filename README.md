# Sistema de Detección, Alarma y Extinción Automática de Incendios en el Motor de una Aeronave

## Descripción

Este proyecto implementa en C++ una simulación del sistema de detección, alarma y extinción automática de incendios en el motor de una aeronave, siguiendo la lógica y el funcionamiento descritos en el proyecto.

El sistema utiliza dos loops principales:

- **Loop A:** formado por dos sensores de temperatura para detectar un aumento anormal de temperatura en la zona del motor.
- **Loop B:** formado por dos sensores de flama para detectar radiación infrarroja asociada con la combustión.

Dependiendo de las señales recibidas por ambos loops, el sistema puede encontrarse en diferentes condiciones: funcionamiento normal, advertencia, emergencia o posible discrepancia.

---

## Objetivo

Desarrollar una simulación en C++ que represente el funcionamiento del sistema de detección y protección contra incendios del motor, incorporando conceptos de Programación Orientada a Objetos y Estructuras de Datos.

El programa permite introducir las lecturas de los sensores, determinar el estado de los loops, activar las diferentes alarmas y representar las acciones que realizaría el sistema ante una condición de emergencia.

---

## Funcionamiento del sistema

### Loop A

El Loop A utiliza dos sensores de temperatura.

La lógica utilizada es:

```text
Sensor de temperatura 1 OR Sensor de temperatura 2
```

Si alguno de los sensores detecta una temperatura igual o superior al umbral configurado, el Loop A se considera activo.

### Loop B

El Loop B utiliza dos sensores de flama.

La lógica utilizada es:

```text
Sensor de flama 1 OR Sensor de flama 2
```

Si alguno de los sensores detecta flama, el Loop B se considera activo.

---

## Modos de operación

### Modo normal

Se presenta cuando:

```text
Loop A = 0
Loop B = 0
```

En esta condición:

- Los sensores continúan siendo monitoreados.
- El LED verde permanece encendido.
- No se activa ninguna alarma.
- La bomba de combustible permanece activa.
- La válvula de combustible permanece abierta.
- La pantalla muestra que el sistema se encuentra en monitoreo normal.

### Modo de advertencia

Se presenta cuando:

```text
Loop A = 1
Loop B = 0
```

El sistema interpreta esta condición como un posible sobrecalentamiento.

Se realiza una verificación de la condición durante 3 segundos. Si la condición persiste:

- Se activa el LED amarillo.
- Se activa la indicación **MASTER CAUTION**.
- Se activa el buzzer durante 15 segundos.
- La pantalla muestra:

```text
Se detecto un riesgo de sobrecalentamiento por parte del Loop A en la zona del motor.
```

### Modo de emergencia

Se presenta cuando:

```text
Loop A = 1
Loop B = 1
```

La activación simultánea de ambos loops confirma una condición de incendio.

El sistema realiza la secuencia de emergencia:

1. Activa la indicación **MASTER WARNING**.
2. Activa el buzzer de forma continua/repetitiva.
3. Desactiva la bomba de combustible.
4. Cierra la válvula de combustible.
5. Activa el sistema de extinción.
6. Registra las acciones realizadas.
7. Mantiene la condición de emergencia hasta realizar un reset manual.

La pantalla muestra:

```text
Incendio detectado en el motor. Activacion automatica del sistema.
```

### Posible discrepancia

Si:

```text
Loop A = 0
Loop B = 1
```

el sistema muestra:

```text
Loop B posible discrepancia.
```

Esto representa la condición en la que el Loop B detecta una posible presencia de fuego mientras el Loop A no detecta un aumento anormal de temperatura.

---

# Programación Orientada a Objetos

El proyecto incorpora Programación Orientada a Objetos mediante diferentes clases.

## Clases principales

### `Sensor`

Es la clase base para representar los sensores del sistema.

Contiene información como:

- Identificador.
- Nombre.
- Estado de activación.
- Estado operativo.

### `SensorTemperatura`

Hereda de la clase `Sensor` y representa los sensores encargados de medir la temperatura del motor.

### `SensorFlama`

Hereda de `Sensor` y representa los sensores encargados de detectar flama.

### `Loop`

Representa cada uno de los loops del sistema:

- Loop A.
- Loop B.

### `SistemaContraIncendios`

Es la clase principal encargada de coordinar los sensores, loops, actuadores, alarmas, pantalla, historial y estados del sistema.

### `Conjunto`

Representa conjuntos mediante arreglos y permite realizar diferentes operaciones entre ellos.

### `ListaEnlazada`

Implementa una estructura de datos lineal mediante nodos enlazados para almacenar el historial de eventos del sistema.

---

# Estructuras de datos

## Lista enlazada

El programa utiliza una **lista enlazada** para almacenar el historial de eventos.

Cada nodo contiene:

```text
Dato
Puntero al siguiente nodo
```

La lista permite agregar y mostrar los eventos registrados durante la ejecución del programa.

Ejemplos de eventos almacenados:

```text
Sistema iniciado en modo NORMAL.
Loop A activo: riesgo de sobrecalentamiento.
EMERGENCIA: Loop A y Loop B activos. Incendio confirmado.
Bomba de combustible desactivada.
Valvula de combustible cerrada.
RESET MANUAL realizado.
```

---

# Arreglos como conjuntos

La clase `Conjunto` utiliza un arreglo para representar los elementos de un conjunto.

Los sensores están identificados mediante números:

```text
1 = Sensor de temperatura 1
2 = Sensor de temperatura 2
3 = Sensor de flama 1
4 = Sensor de flama 2
```

El programa implementa las siguientes operaciones:

### Pertenencia

Permite comprobar si un elemento pertenece a un conjunto.

```cpp
pertenece(valor)
```

### Unión

Combina los elementos de dos conjuntos.

```cpp
A + B
```

### Intersección

Obtiene los elementos que pertenecen a ambos conjuntos.

```cpp
A & B
```

### Diferencia

Obtiene los elementos que pertenecen al primer conjunto pero no al segundo.

```cpp
A - B
```

---

# Sobrecarga de operadores

El proyecto utiliza sobrecarga de operadores para trabajar con los conjuntos.

| Operador | Operación |
|---|---|
| `+` | Unión |
| `&` | Intersección |
| `-` | Diferencia |
| `==` | Comparación del estado de dos loops |

Ejemplo:

```cpp
Conjunto unionConjuntos = conjuntoLoopA + conjuntoLoopB;
```

---

# Manejo de excepciones

El programa incluye una clase personalizada llamada:

```cpp
ExcepcionSistema
```

Esta clase permite controlar situaciones que podrían producir errores durante la ejecución.

Se utiliza `try`, `throw` y `catch`.

Algunos ejemplos de validaciones son:

- Temperaturas fuera del rango permitido para la simulación.
- Valores diferentes de `0` y `1` para los sensores de flama.
- Umbral de temperatura no válido.
- Opciones incorrectas del menú.
- Capacidad máxima del conjunto.

---

# Menú del programa

El programa cuenta con un menú principal:

```text
1. Ingresar lecturas de sensores
2. Mostrar estado del sistema
3. Realizar RESET manual
4. Mostrar historial
5. Mostrar operaciones de conjuntos
6. Simular falla de un sensor
7. Mostrar información de la lógica
0. Salir
```

## Ingresar lecturas

Permite introducir:

- Temperatura del sensor 1.
- Temperatura del sensor 2.
- Estado del sensor de flama 1.
- Estado del sensor de flama 2.

## Mostrar estado

Muestra:

- Estado de los sensores.
- Estado de los loops.
- Bomba de combustible.
- Válvula de combustible.
- Válvulas del sistema de extinción.
- LEDs.
- Buzzer.
- Mensaje LCD.
- Estado general del sistema.

## Reset manual

Permite restablecer el sistema después de una condición de emergencia.

Al realizar el reset:

- Se limpian las alarmas.
- Se enciende el LED verde.
- Se reactiva la bomba de combustible.
- Se abre nuevamente la válvula de combustible.
- El sistema vuelve al estado normal.

## Historial

Muestra los eventos registrados mediante la lista enlazada.

## Operaciones de conjuntos

Permite visualizar:

- Pertenencia.
- Unión.
- Intersección.
- Diferencia.

## Simulación de fallas

Permite simular una falla en alguno de los sensores para realizar pruebas del sistema.

---

# Tecnologías utilizadas

- **Lenguaje:** C++
- **Programación Orientada a Objetos**
- **Clases y objetos**
- **Herencia**
- **Encapsulamiento**
- **Sobrecarga de operadores**
- **Manejo de excepciones**
- **Lista enlazada**
- **Arreglos**
- **Operaciones de conjuntos**

---

# Compilación

El programa utiliza características compatibles con C++17.

Para compilarlo con `g++`:

```bash
g++ -std=c++17 proyecto_avion.cpp -o proyecto_avion
```

En Windows:

```bash
g++ -std=c++17 proyecto_avion.cpp -o proyecto_avion.exe
```

Para ejecutarlo:

```bash
./proyecto_avion
```

En Windows también puede ejecutarse como:

```bash
./proyecto_avion.exe
```

---

# Archivos del proyecto

```text
Sistema-Deteccion-Incendios-Aeronave/
│
├── proyecto_avion.cpp
│
└── README.md
```

---

# Consideración sobre el umbral de temperatura

El programa solicita al usuario el umbral de temperatura al iniciar.

Esto se realiza porque la información proporcionada para el proyecto describe la lógica de activación del Loop A, pero no establece de forma inequívoca un único valor de temperatura que deba utilizarse como umbral fijo del programa.

Por esta razón, el valor se configura al comenzar la simulación en lugar de establecer arbitrariamente un valor como si fuera un requisito del sistema.

---

# Alcance del proyecto

Este programa es una **simulación académica en C++** de la lógica de funcionamiento descrita para el sistema de protección contra incendios del motor.

No controla directamente sensores, bombas, válvulas, LEDs, buzzers o componentes físicos de una aeronave.

Las entradas y salidas del sistema son representadas mediante la consola del programa.
