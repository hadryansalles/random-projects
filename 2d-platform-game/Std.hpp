#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ostream>
#include <fstream>
#include <climits>
#include <cfloat>
#include <ctime>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "EnumeradorCodigos.hpp"

// permite realizar operacoes de multiplicacao escalar de Vector2f
sf::Vector2f operator / (sf::Vector2f vector, float valor);
sf::Vector2f operator * (sf::Vector2f vector, float valor);

// permite salvar e obter um sf::Vector2f
std::ostream& operator << (std::ostream& out, const sf::Vector2f& vetor);
std::istream& operator >> (std::istream& in, sf::Vector2f& vetor);

std::ostream& operator << (std::ostream& out, const std::string str);
