#include "Mecanismo.h"

Mecanismo::Mecanismo(int canal) : _canal(canal), _activo(false){}

int Mecanismo::getCanal() const{return _canal;}

bool Mecanismo::estaActivo() const {return _activo;}

void Mecanismo::setActivo(bool activo){_activo = activo;}




