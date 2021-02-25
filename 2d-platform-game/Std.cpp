#include "Std.hpp"

sf::Vector2f operator / (sf::Vector2f vector, float valor) {
    return (sf::Vector2f(vector.x / valor, vector.y / valor));
}

sf::Vector2f operator * (sf::Vector2f vector, float valor) {
    return (sf::Vector2f(vector.x * valor, vector.y * valor));
}

std::ostream& operator << (std::ostream& out, const sf::Vector2f& vetor) {
    out << vetor.x << ' ';
    out << vetor.y;
    return out;
}

std::istream& operator >> (std::istream& in, sf::Vector2f& vetor) {
    in >> vetor.x;
    in >> vetor.y;
    return in;
}

std::ostream& operator << (std::ostream& out, const std::string str) {
    out << str.c_str();
    return out;
}
