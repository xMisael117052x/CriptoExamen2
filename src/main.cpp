#include "Prerequisites.h"
#include "XOREncoder.h"
#include "CesarEncoder.h"
#include "VigenereEncoder.h"
#include "DESEncoder.h"

// Declaraciones de funciones
void handleUserChoice(const fs::path& desencriptadosPath, const fs::path& encriptadosPath);
void processFiles(const fs::path& inputDir, const fs::path& outputDir, const std::string& inputExt, const std::string& outputExt, const std::string& operation, int cipherChoice);
std::string readFileContents(const fs::path& filePath);
void writeFileContents(const fs::path& filePath, const std::string& content);

/**
 * @brief Lee una línea de la consola y la convierte en una opción numérica.
 * @return El número entero elegido por el usuario, o 0 si la entrada es inválida.
 */
int getChoiceFromUser() {
    std::string line;
    std::getline(std::cin, line);
    try {
        if (line.empty()) {
            return 0; // Si el usuario solo presiona Enter, es una opción inválida.
        }
        return std::stoi(line);
    } catch (const std::exception& e) {
        return 0; // Retorna 0 como indicador de opción inválida.
    }
}

/**
 * @brief Punto de entrada principal de la aplicación.
 */
int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "Error critico: No se pudo determinar la ruta del ejecutable." << std::endl;
        return EXIT_FAILURE;
    }

    fs::path exePath = fs::absolute(fs::path(argv[0]));
    fs::path projectRoot = exePath.parent_path().parent_path();
    fs::path filesEncriptadosDir = projectRoot / "FilesEncriptados";
    fs::path filesDesencriptadosDir = projectRoot / "FilesDesencriptados";

    fs::create_directory(filesEncriptadosDir);
    fs::create_directory(filesDesencriptadosDir);

    handleUserChoice(filesDesencriptadosDir, filesEncriptadosDir);

    return EXIT_SUCCESS;
}

/**
 * @brief Gestiona el bucle principal del menú y las acciones del usuario.
 */
void handleUserChoice(const fs::path& desencriptadosPath, const fs::path& encriptadosPath) {
    int mainChoice = 0;
    while (mainChoice != 3) {
        // --- Bucle del Menú Principal ---
        std::cout << "========================================\n";
        std::cout << "      Aplicacion de Criptografia\n";
        std::cout << "========================================\n\n" << std::flush;

        std::cout << "Por favor, elige una operacion:\n"
                  << "  1. Encriptar archivo(s) (.txt -> .cif)\n"
                  << "  2. Desencriptar archivo(s) (.cif -> .txt)\n"
                  << "  3. Salir\n" << std::endl;
        std::cout << "Tu eleccion: " << std::flush;
        mainChoice = getChoiceFromUser();

        // --- Lógica de Selección ---
        if (mainChoice == 1 || mainChoice == 2) {
            std::string operationStr = (mainChoice == 1) ? "Encriptar" : "Desencriptar";

            // --- Menú de Algoritmos ---
            std::cout << "========================================\n";
            std::cout << "      Aplicacion de Criptografia\n";
            std::cout << "========================================\n\n" << std::flush;

            std::cout << "Selecciona el algoritmo para " << operationStr << ":\n"
                      << "  1. XOR\n"
                      << "  2. Cesar\n"
                      << "  3. Vigenere\n"
                      << "  4. DES\n"
                      << "  5. Volver al menu principal\n" << std::endl;
            std::cout << "Tu eleccion: " << std::flush;
            int cipherChoice = getChoiceFromUser();

            if (cipherChoice >= 1 && cipherChoice <= 4) {
                const auto& inputDir = (mainChoice == 1) ? desencriptadosPath : encriptadosPath;
                const auto& outputDir = (mainChoice == 1) ? encriptadosPath : desencriptadosPath;
                const std::string inputExt = (mainChoice == 1) ? ".txt" : ".cif";
                const std::string outputExt = (mainChoice == 1) ? ".cif" : ".txt";
                std::string op = (mainChoice == 1) ? "encriptar" : "desencriptar";
                processFiles(inputDir, outputDir, inputExt, outputExt, op, cipherChoice);
            } else if (cipherChoice == 5) {
                continue; // Vuelve al inicio del bucle while
            } else {
                 std::cout << "\nOpcion de algoritmo no valida." << std::endl;
            }

        } else if (mainChoice != 3) {
            std::cout << "\nOpcion principal no valida." << std::endl;
        }

        // --- Pausa para Continuar ---
        if (mainChoice != 3) {
            std::cout << "\nPresiona Enter para continuar..." << std::flush;
            std::string dummy;
            std::getline(std::cin, dummy); // Espera a que el usuario presione Enter
        }
    }
    std::cout << "\nSaliendo de la aplicacion. ¡Hasta luego!\n";
}


/**
 * @brief Procesa archivos para encriptar o desencriptar según el algoritmo elegido.
 */
void processFiles(const fs::path& inputDir, const fs::path& outputDir, const std::string& inputExt, const std::string& outputExt, const std::string& operation, int cipherChoice) {
    std::cout << "========================================\n";
    std::cout << "      Aplicacion de Criptografia\n";
    std::cout << "========================================\n\n" << std::flush;

    std::cout << "Operacion: " << operation << " con algoritmo " << (cipherChoice == 1 ? "XOR" : cipherChoice == 2 ? "Cesar" : cipherChoice == 3 ? "Vigenere" : "DES") << "\n";

    std::cout << "\nIntroduce el/los nombre(s) de archivo(s) en '" << inputDir.string() << "' (separados por espacios, sin extension):\n> " << std::flush;
    std::string line;
    std::getline(std::cin, line);

    std::cout << "Introduce la contrasena/clave:\n> " << std::flush;
    std::string password;
    std::getline(std::cin, password);

    if (password.empty()) {
        std::cout << "La contrasena no puede estar vacia. Operacion cancelada.\n";
        return;
    }

    std::stringstream ss(line);
    std::string filename;
    int successCount = 0;
    int failCount = 0;

    XOREncoder xorEncoder;
    CesarEncoder cesarEncoder;
    VigenereEncoder vigenereEncoder;
    DESEncoder desEncoder;

    while (ss >> filename) {
        fs::path inputFile = inputDir / (filename + inputExt);

        if (!fs::exists(inputFile)) {
            std::cerr << "Error: El archivo '" << inputFile.string() << "' no existe. Omitiendo.\n";
            failCount++;
            continue;
        }

        std::string content = readFileContents(inputFile);
        if (content.empty() && fs::is_regular_file(inputFile) && fs::file_size(inputFile) > 0) {
             std::cerr << "Error: No se pudo leer el contenido de '" << inputFile.string() << "'. Omitiendo.\n";
             failCount++;
             continue;
        }

        std::string processedContent;

        switch(cipherChoice) {
            case 1:
                processedContent = xorEncoder.encode(content, password);
                break;
            case 2:
                processedContent = (operation == "encriptar") ? cesarEncoder.encode(content, password) : cesarEncoder.decode(content, password);
                break;
            case 3:
                processedContent = (operation == "encriptar") ? vigenereEncoder.encode(content, password) : vigenereEncoder.decode(content, password);
                break;
            case 4:
                processedContent = (operation == "encriptar") ? desEncoder.encode(content, password) : desEncoder.decode(content, password);
                break;
        }

        fs::path baseOutputFile = outputDir / (filename + outputExt);
        fs::path outputFile = baseOutputFile;
        int suffix = 1;
        while (fs::exists(outputFile)) {
            outputFile = outputDir / (filename + "-" + std::to_string(suffix) + outputExt);
            suffix++;
        }
        writeFileContents(outputFile, processedContent);

        std::cout << "Proceso completado: '" << inputFile.string() << "' -> '" << outputFile.string() << "'\n";
        successCount++;
    }

    std::cout << "\n--- Resumen de la operacion ---\n";
    std::cout << "Archivos procesados con exito: " << successCount << "\n";
    std::cout << "Archivos fallidos: " << failCount << "\n";
}


/**
 * @brief Lee todo el contenido de un archivo y lo devuelve como un string.
 */
std::string readFileContents(const fs::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


/**
 * @brief Escribe un string en un archivo, sobreescribiéndolo si ya existe.
 */
void writeFileContents(const fs::path& filePath, const std::string& content) {
    std::ofstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file << content;
    }
}