#pragma once
class RegistroPartida {
private:
	float _tiempo;
	int _muertes;
public:
	RegistroPartida();
	RegistroPartida(float tiempo, int muertes);

	float getTiempo() const;
	int getMuertes() const;
};

