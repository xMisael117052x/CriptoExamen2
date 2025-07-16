#pragma once
#include "Prerequisites.h"
#include <bitset>
#include <vector>

/**
 * @class DESEncoder
 * @brief Implementa el cifrado DES (Data Encryption Standard) para strings.
 *
 * Maneja el cifrado y descifrado de datos de longitud variable mediante el
 * procesamiento en bloques de 64 bits y la aplicación de relleno PKCS#7.
 */
class DESEncoder {
public:
    /**
     * @brief Constructor por defecto.
     */
    DESEncoder() = default;

    /**
     * @brief Codifica un texto utilizando DES.
     * @summary Aplica padding PKCS#7, procesa el texto en bloques de 8 bytes (64 bits)
     * y los codifica usando la clave proporcionada.
     * @param text El texto a codificar.
     * @param key La clave de cifrado (se ajustará a 8 bytes).
     * @return El texto codificado en formato binario (string).
     */
    std::string encode(const std::string& text, const std::string& key) {
        std::string padded_text = apply_pkcs7_padding(text);
        std::bitset<64> des_key = stringToBitset64(normalizeKey(key));
        generateSubkeys(des_key);

        std::string result = "";
        for (size_t i = 0; i < padded_text.length(); i += 8) {
            std::string block_str = padded_text.substr(i, 8);
            std::bitset<64> block_bits = stringToBitset64(block_str);
            std::bitset<64> encrypted_block = encode_block(block_bits);
            result += bitset64ToString(encrypted_block);
        }
        return result;
    }

    /**
     * @brief Decodifica un texto cifrado con DES.
     * @summary Procesa el texto cifrado en bloques de 8 bytes, los decodifica y
     * elimina el padding PKCS#7 del resultado final.
     * @param text El texto cifrado.
     * @param key La clave original usada para codificar.
     * @return El texto original decodificado.
     */
    std::string decode(const std::string& text, const std::string& key) {
        std::bitset<64> des_key = stringToBitset64(normalizeKey(key));
        generateSubkeys(des_key);

        std::string decrypted_padded_text = "";
        for (size_t i = 0; i < text.length(); i += 8) {
            std::string block_str = text.substr(i, 8);
            std::bitset<64> block_bits = stringToBitset64(block_str);
            std::bitset<64> decrypted_block = decode_block(block_bits);
            decrypted_padded_text += bitset64ToString(decrypted_block);
        }

        return remove_pkcs7_padding(decrypted_padded_text);
    }

private:
    std::vector<std::bitset<48>> subkeys;

    // --- LÓGICA DE PADDING (RELLENO) ---

    std::string apply_pkcs7_padding(const std::string& data) {
        size_t padding_len = 8 - (data.length() % 8);
        char padding_char = static_cast<char>(padding_len);
        std::string padded_data = data;
        padded_data.append(padding_len, padding_char);
        return padded_data;
    }

    std::string remove_pkcs7_padding(const std::string& data) {
        if (data.empty()) {
            return "";
        }
        size_t padding_len = static_cast<size_t>(data.back());
        if (padding_len > 8 || padding_len > data.length()) {
            // Padding inválido, devolver datos tal cual.
            return data;
        }
        return data.substr(0, data.length() - padding_len);
    }

    // --- LÓGICA DE MANEJO DE CLAVE ---

    std::string normalizeKey(const std::string& key) {
        std::string normalized = key;
        normalized.resize(8, '\0'); // Trunca o rellena la clave a 8 bytes.
        return normalized;
    }

    void generateSubkeys(const std::bitset<64>& key) {
        subkeys.clear();
        for (int i = 0; i < 16; ++i) {
            std::bitset<48> subkey((key.to_ullong() >> (i * 1)) & 0xFFFFFFFFFFFF); // Clave simple para ejemplo
            subkeys.push_back(subkey);
        }
    }

    // --- LÓGICA CENTRAL DE DES (POR BLOQUE) ---

    std::bitset<64> encode_block(const std::bitset<64>& plaintext) {
        auto data = iPermutation(plaintext);
        std::bitset<32> left(data.to_string().substr(0, 32));
        std::bitset<32> right(data.to_string().substr(32, 32));

        for (int round = 0; round < 16; round++) {
            auto newRight = left ^ feistel(right, subkeys[round]);
            left = right;
            right = newRight;
        }

        std::string combined_str = right.to_string() + left.to_string();
        return fPermutation(std::bitset<64>(combined_str));
    }

    std::bitset<64> decode_block(const std::bitset<64>& ciphertext) {
        auto data = iPermutation(ciphertext);
        std::bitset<32> left(data.to_string().substr(0, 32));
        std::bitset<32> right(data.to_string().substr(32, 32));

        for (int round = 15; round >= 0; --round) {
            auto newRight = left ^ feistel(right, subkeys[round]);
            left = right;
            right = newRight;
        }

        std::string combined_str = right.to_string() + left.to_string();
        return fPermutation(std::bitset<64>(combined_str));
    }

    // --- FUNCIONES AUXILIARES DE DES ---

    const int EXPANSION_TABLE[48] = {
        32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
    };
    const int P_TABLE[32] = {
        16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
    };
    const int SBOX[4][16] = {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0}, {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    };

    std::bitset<64> iPermutation(const std::bitset<64>& input) { return input; } // Simplificado
    std::bitset<64> fPermutation(const std::bitset<64>& input) { return input; } // Simplificado

    std::bitset<48> expand(const std::bitset<32>& halfBlock) {
        std::string expanded_str = "";
        for (int i = 0; i < 48; i++) {
            expanded_str += halfBlock.to_string()[EXPANSION_TABLE[i] - 1];
        }
        return std::bitset<48>(expanded_str);
    }

    std::bitset<32> substitute(const std::bitset<48>& input) {
        std::string result_str = "";
        for (int i = 0; i < 8; i++) {
            std::string six_bits = input.to_string().substr(i * 6, 6);
            int row = std::stoi(std::string() + six_bits[0] + six_bits[5], nullptr, 2);
            int col = std::stoi(six_bits.substr(1, 4), nullptr, 2);
            result_str += std::bitset<4>(SBOX[row][col]).to_string();
        }
        return std::bitset<32>(result_str);
    }

    std::bitset<32> permutedP(const std::bitset<32>& input) {
        std::string p_str = "";
        for (int i = 0; i < 32; i++) {
            p_str += input.to_string()[P_TABLE[i] - 1];
        }
        return std::bitset<32>(p_str);
    }

    std::bitset<32> feistel(const std::bitset<32>& right, const std::bitset<48>& subkey) {
        return permutedP(substitute(expand(right) ^ subkey));
    }

    std::bitset<64> stringToBitset64(const std::string& block) {
        uint64_t val = 0;
        for (size_t i = 0; i < 8; ++i) {
            val |= static_cast<uint64_t>(static_cast<unsigned char>(block[i])) << (8 * (7 - i));
        }
        return std::bitset<64>(val);
    }

    std::string bitset64ToString(const std::bitset<64>& bits) {
        std::string result(8, '\0');
        uint64_t val = bits.to_ullong();
        for (size_t i = 0; i < 8; ++i) {
            result[i] = static_cast<char>((val >> (8 * (7 - i))) & 0xFF);
        }
        return result;
    }
};