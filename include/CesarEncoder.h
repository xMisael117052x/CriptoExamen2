#pragma once
#include "Prerequisites.h"
#include <numeric>

/**
 * @class CesarEncoder
 * @brief Proporciona métodos para codificar y decodificar texto utilizando el cifrado César.
 *
 * El desplazamiento (clave) se deriva de un string proporcionado por el usuario,
 * permitiendo una integración sencilla con la lógica existente de la aplicación.
 */
class CesarEncoder {
public:
    /**
     * @brief Constructor por defecto.
     */
    CesarEncoder() = default;

    /**
     * @brief Codifica un texto utilizando el cifrado César.
     * @param text El texto a codificar.
     * @param key La clave en formato string de la cual se derivará el desplazamiento numérico.
     * @return El texto codificado.
     */
    std::string encode(const std::string& text, const std::string& key) {
        int shift = deriveShiftFromKey(key);
        std::string result = "";

        int letter_shift = (shift % 26 + 26) % 26;
        int digit_shift = (shift % 10 + 10) % 10;

        for (char c : text) {
            if (c >= 'A' && c <= 'Z') {
                result += (char)(((c - 'A' + letter_shift) % 26) + 'A');
            } else if (c >= 'a' && c <= 'z') {
                result += (char)(((c - 'a' + letter_shift) % 26) + 'a');
            } else if (c >= '0' && c <= '9') {
                result += (char)(((c - '0' + digit_shift) % 10) + '0');
            } else {
                result += c;
            }
        }
        return result;
    }

    /**
     * @brief Decodifica un texto cifrado con César.
     * @param text El texto cifrado.
     * @param key La clave original usada para codificar.
     * @return El texto decodificado.
     */
    std::string decode(const std::string& text, const std::string& key) {
        int shift = deriveShiftFromKey(key);
        // La decodificación es una codificación con el desplazamiento inverso.
        int letter_shift_decode = 26 - (shift % 26);
        // Para mantener la misma lógica, simplemente codificamos con el desplazamiento inverso.
        // Creamos una "clave" falsa que genere este desplazamiento.
        return encode(text, std::string(1, (char)letter_shift_decode));
    }

private:
    /**
     * @brief Deriva un desplazamiento numérico a partir de una clave de tipo string.
     * @param key La clave de entrada.
     * @return El desplazamiento numérico resultante.
     */
    int deriveShiftFromKey(const std::string& key) {
        if (key.empty()) {
            return 0;
        }
        // Suma los valores ASCII de los caracteres de la clave para obtener el desplazamiento.
        return std::accumulate(key.begin(), key.end(), 0);
    }
};