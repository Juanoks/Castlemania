#pragma once
// constexpr: le dice al compilador que el valor es fijo y se conoce al compilar
constexpr float ANCHO_MUNDO = 960.f;
constexpr float ALTO_MUNDO = 540.f;
constexpr unsigned int FPS = 60; // cuadros por segundo (limite de la ventana)

constexpr int MAX_RECORDS = 5; // maximo de records a mostrar en la pantalla de Records

constexpr float ANCHO_JUGADOR = 24.f;
constexpr float ALTO_JUGADOR = 40.f;

constexpr float GROSOR_PARED = 20.f;
constexpr float TAMANIO_HUECO = 80.f;
constexpr float Y_PISO = ALTO_MUNDO - GROSOR_PARED; // donde empieza el piso (mismo grosor que las paredes)

constexpr float VELOCIDAD_FLECHA = 700.f;
constexpr int MAX_FLECHAS = 3; // flechas que pueden existir a la vez (al disparar otra, se borra la mas vieja)

