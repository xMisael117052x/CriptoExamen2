#pragma once
#include "Prerequisites.h"

/**
 * @class VigenereEncoder
 * @brief Proporciona métodos para codificar y decodificar texto utilizando el cifrado Vigenère.
 *
 * Utiliza una clave de texto para aplicar desplazamientos variables, ofreciendo una
 * seguridad mayor que el cifrado César simple.
 */
class VigenereEncoder {
public:
    /**
     * @brief Constructor por defecto.
     */
    VigenereEncoder() = default;

    /**
     * @brief Codifica un texto utilizando el cifrado Vigenère.
     * @param text El texto a codificar.
     * @param key La clave de cifrado. Solo se usarán los caracteres alfabéticos.
     * @return El texto codificado.
     */
    std::string encode(const std::string& text, const std::string& rawKey) {
        std::string key = normalizeKey(rawKey);
        if (key.empty()) {
            // Si la clave no tiene letras, no se puede cifrar. Devuelve el texto original.
            return text;
        }

        std::string result;
        result.reserve(text.size());
        unsigned int key_idx = 0;

        for (char c : text) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                bool isLower = std::islower(static_cast<unsigned char>(c));
                char base = isLower ? 'a' : 'A';
                int shift = key[key_idx % key.size()] - 'A';

                result += static_cast<char>((c - base + shift) % 26 + base);
                key_idx++;
            } else {
                result += c;
            }
        }
        return result;
    }

    /**
     * @brief Decodifica un texto cifrado con Vigenère.
     * @param text El texto cifrado.
     * @param key La clave original usada para codificar.
     * @return El texto decodificado.
     */
    std::string decode(const std::string& text, const std::string& rawKey) {
        std::string key = normalizeKey(rawKey);
        if (key.empty()) {
            return text;
        }

        std::string result;
        result.reserve(text.size());
        unsigned int key_idx = 0;

        for (char c : text) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                bool isLower = std::islower(static_cast<unsigned char>(c));
                char base = isLower ? 'a' : 'A';
                int shift = key[key_idx % key.size()] - 'A';

                result += static_cast<char>(((c - base) - shift + 26) % 26 + base);
                key_idx++;
            } else {
                result += c;
            }
        }
        return result;
    }

private:
    /**
     * @brief Normaliza una clave para que contenga solo letras mayúsculas.
     * @param rawKey La clave original.
     * @return La clave normalizada.
     */
    std::string normalizeKey(const std::string& rawKey) {
        std::string k;
        for (char c : rawKey) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                k += std::toupper(static_cast<unsigned char>(c));
            }
        }
        return k;
    }
};