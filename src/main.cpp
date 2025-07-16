#include "Prerequisites.h"
#include "XOREncoder.h"

void displayHeader();
void displayMenu();
void handleUserChoice(const fs::path& desencriptadosPath, const fs::path& encriptadosPath);
void processFiles(const fs::path& inputDir, const fs::path& outputDir, const std::string& inputExt, const std::string& outputExt, const std::string& operation);
std::string readFileContents(const fs::path& filePath);
void writeFileContents(const fs::path& filePath, const std::string& content);


/**
 * @brief Punto de entrada principal de la aplicación.
 * @param argc Número de argumentos de la línea de comandos.
 * @param argv Array de argumentos de la línea de comandos.
 * @return Código de salida del programa.
 */
int main(int argc, char* argv[]) {
    // Verificación básica de que tenemos el argumento del path del ejecutable.
    if (argc < 1) {
        std::cerr << "Error critico: No se pudo determinar la ruta del ejecutable." << std::endl;
        return EXIT_FAILURE;
    }

    // --- LÓGICA PARA ENCONTRAR LAS CARPETAS ---
    // 1. Obtiene la ruta completa del ejecutable (ej: C:\...\CriptoExamen2\build\CriptoExamen2.exe)
    fs::path exePath = fs::absolute(fs::path(argv[0]));
    // 2. Sube un nivel para llegar al directorio 'build'.
    // 3. Sube otro nivel para llegar al directorio raíz del proyecto.
    fs::path projectRoot = exePath.parent_path().parent_path();

    // 4. Construye las rutas absolutas a las carpetas de trabajo.
    fs::path filesEncriptadosDir = projectRoot / "FilesEncriptados";
    fs::path filesDesencriptadosDir = projectRoot / "FilesDesencriptados";

    // Asegurarse de que los directorios necesarios existan.
    fs::create_directory(filesEncriptadosDir);
    fs::create_directory(filesDesencriptadosDir);

    // Inicia el menú principal, pasando las rutas correctas.
    handleUserChoice(filesDesencriptadosDir, filesEncriptadosDir);

    return EXIT_SUCCESS;
}


/**
 * @brief Muestra una cabecera o banner para la aplicación.
 */
void displayHeader() {
    std::cout << "========================================\n";
    std::cout << "      Aplicacion de Cifrado XOR\n";
    std::cout << "========================================\n\n";
}


/**
 * @brief Muestra el menú de opciones al usuario.
 */
void displayMenu() {
    std::cout << "Por favor, elige una opcion:\n";
    std::cout << "  1. Encriptar archivo(s) (.txt -> .cif)\n";
    std::cout << "  2. Desencriptar archivo(s) (.cif -> .txt)\n";
    std::cout << "  3. Salir\n";
    std::cout << "\nTu eleccion: ";
}


/**
 * @brief Gestiona el bucle principal del menú y las acciones del usuario.
 * @param desencriptadosPath Ruta absoluta a la carpeta de archivos desencriptados.
 * @param encriptadosPath Ruta absoluta a la carpeta de archivos encriptados.
 */
void handleUserChoice(const fs::path& desencriptadosPath, const fs::path& encriptadosPath) {
    int choice = 0;
    while (choice != 3) {
        displayHeader();
        displayMenu();
        std::cin >> choice;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                processFiles(desencriptadosPath, encriptadosPath, ".txt", ".cif", "encriptar");
                break;
            case 2:
                processFiles(encriptadosPath, desencriptadosPath, ".cif", ".txt", "desencriptar");
                break;
            case 3:
                std::cout << "Saliendo de la aplicacion. ¡Hasta luego!\n";
                break;
            default:
                std::cout << "\nOpcion no valida. Por favor, intenta de nuevo.\n";
                break;
        }

        if (choice != 3) {
            std::cout << "\nPresiona Enter para continuar...";
            std::cin.get();
        }
    }
}


/**
 * @brief Procesa archivos para encriptar o desencriptar.
 * @param inputDir Directorio de entrada (ruta absoluta).
 * @param outputDir Directorio de salida (ruta absoluta).
 * @param inputExt Extensión de archivo de entrada.
 * @param outputExt Extensión de archivo de salida.
 * @param operation Descripción de la operación (para los mensajes al usuario).
 */
void processFiles(const fs::path& inputDir, const fs::path& outputDir, const std::string& inputExt, const std::string& outputExt, const std::string& operation) {
    // Ahora inputDir.string() mostrará la ruta completa, lo que es más claro para el usuario.
    std::cout << "\nIntroduce el/los nombre(s) de archivo(s) en '" << inputDir.string() << "' (separados por espacios, sin extension):\n> ";
    std::string line;
    std::getline(std::cin, line);

    std::cout << "Introduce la contrasena:\n> ";
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

    XOREncoder encoder;

    while (ss >> filename) {
        // La construcción de la ruta ahora es infalible porque parte de una ruta absoluta.
        fs::path inputFile = inputDir / (filename + inputExt);

        if (!fs::exists(inputFile)) {
            std::cerr << "Error: El archivo '" << inputFile.string() << "' no existe. Omitiendo.\n";
            failCount++;
            continue;
        }

        std::string content = readFileContents(inputFile);
        if (content.empty() && fs::file_size(inputFile) > 0) {
             std::cerr << "Error: No se pudo leer el contenido de '" << inputFile.string() << "'. Omitiendo.\n";
             failCount++;
             continue;
        }

        std::string processedContent = encoder.encode(content, password);

        // Generar nombre de archivo con sufijo incremental si ya existe
        fs::path baseOutputFile = outputDir / (filename + outputExt);
        fs::path outputFile = baseOutputFile;
        int suffix = 1;
        while (fs::exists(outputFile)) {
            std::string name = filename + "-" + std::to_string(suffix) + outputExt;
            outputFile = outputDir / name;
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
 * @param filePath La ruta al archivo.
 * @return El contenido del archivo.
 */
std::string readFileContents(const fs::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


/**
 * @brief Escribe un string en un archivo, sobreescribiéndolo si ya existe.
 * @param filePath La ruta al archivo de salida.
 * @param content El contenido a escribir.
 */
void writeFileContents(const fs::path& filePath, const std::string& content) {
    std::ofstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file << content;
    }
}
