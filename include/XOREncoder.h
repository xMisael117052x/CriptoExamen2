// Ruta: CriptoExamen/include/XOREncoder.h
// Esta es la clase que proporcionaste.

#pragma once
#include "Prerequisites.h"

class XOREncoder {
public:
    /**
     * @brief Constructor simple. No hace nada.
     */
    XOREncoder() = default;

    /**
     * @brief Codifica o decodifica el texto de entrada usando una clave con la operación XOR.
     * La operación es simétrica, por lo que la misma función sirve para encriptar y desencriptar.
     * @param input El texto a procesar.
     * @param key La contraseña para la operación.
     * @return El texto procesado.
     */
    std::string encode(const std::string& input, const std::string& key) {
        if (key.empty()) {
            return input;
        }
        std::string output = input;
        for (size_t i = 0; i < input.size(); ++i) {
            output[i] = input[i] ^ key[i % key.length()];
        }
        return output;
    }
};
