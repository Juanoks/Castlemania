#pragma once
// constexpr: le dice al compilador que el valor es fijo y se conoce al compilar
constexpr float ANCHO_MUNDO = 960.f;
constexpr float ALTO_MUNDO = 540.f;
constexpr unsigned int FPS = 60; // cuadros por segundo (limite de la ventana)

constexpr int MAX_RECORDS = 5; // maximo de records a mostrar en la pantalla de Records

// tamanio de la HITBOX del jugador. El sprite (50x80) se dibuja al 75%: 37.5x60,
// y el cuerpo visible mide unos 30 de ancho, asi que la hitbox coincide con lo que se ve.
constexpr float ANCHO_JUGADOR = 30.f;
constexpr float ALTO_JUGADOR = 60.f;

constexpr float GROSOR_PARED = 20.f;
constexpr float TAMANIO_HUECO = 80.f;
constexpr float Y_PISO = ALTO_MUNDO - GROSOR_PARED; // donde empieza el piso (mismo grosor que las paredes)

constexpr int TERRENO_A_MANO = -2; // el piso no lo arma la matriz

constexpr float VELOCIDAD_LANZA = 700.f;

constexpr int CANTIDAD_LLAVES = 4; // azul, amarilla, verde y roja

constexpr int CANAL_PUERTA_AZUL = 9;      // lo abre tener la llave azul 
constexpr int CANAL_PUERTA_AMARILLA = 10; // lo abre tener la llave amarilla
constexpr int CANAL_PUERTA_VERDE = 11;    // lo abre tener la llave verde

// herramientas para armar salas: coordenadas del mouse, grilla (G) y recargar la sala (R).
// poner en true si se quiere usar
constexpr bool MODO_DEBUG = false;
