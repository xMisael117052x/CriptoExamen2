# Manual de Usuario: Aplicación de Cifrado XOR

Esta es una aplicación de consola para encriptar y desencriptar archivos de texto (.txt) utilizando un cifrado XOR simple con una contraseña.

## Ejecución

Para ejecutar la aplicación, navega hasta el directorio `cmake-build-release` y ejecuta el archivo `CriptoExamen2.exe`.

```bash
cd cmake-build-release
./CriptoExamen2.exe
```

## Estructura de Carpetas

El programa utiliza dos carpetas principales para gestionar los archivos:

-   **`FilesDesencriptados/`**: Esta carpeta contiene los archivos de texto plano (`.txt`) que deseas encriptar. Coloca aquí todos los archivos que necesites procesar.
-   **`FilesEncriptados/`**: En esta carpeta se guardarán los archivos encriptados con la extensión `.cif`. También es el lugar desde donde la aplicación leerá los archivos que desees desencriptar.

## ¿Cómo Encriptar Archivos?

1.  Coloca los archivos `.txt` que quieras encriptar en la carpeta `FilesDesencriptados/`.
2.  Ejecuta la aplicación.
3.  Selecciona la opción **1. Encriptar archivo(s) (.txt -> .cif)**.
4.  Cuando se te solicite, introduce el nombre de los archivos que deseas encriptar, **sin la extensión `.txt`**. Puedes introducir uno o varios nombres separados por espacios.
    > Ejemplo: `archivo1 prueba_secreta`
5.  Introduce una contraseña. Esta será necesaria para desencriptar los archivos.
6.  Los archivos encriptados se guardarán en la carpeta `FilesEncriptados/` con la extensión `.cif`.

## ¿Cómo Desencriptar Archivos?

1.  Asegúrate de que los archivos encriptados (`.cif`) estén en la carpeta `FilesEncriptados/`.
2.  Ejecuta la aplicación.
3.  Selecciona la opción **2. Desencriptar archivo(s) (.cif -> .txt)**.
4.  Introduce el nombre de los archivos que deseas desencriptar, **sin la extensión `.cif`**.
5.  Introduce la misma contraseña que usaste para encriptarlos.
6.  Los archivos desencriptados aparecerán en la carpeta `FilesDesencriptados/` con la extensión `.txt`.

## Manejo de Archivos Duplicados

Si intentas encriptar o desencriptar un archivo y ya existe uno con el mismo nombre en la carpeta de destino, la aplicación no lo sobrescribirá. En su lugar, añadirá un sufijo numérico incremental al nuevo archivo.

-   `nombre.txt` -> `nombre.cif`
-   Si `nombre.cif` ya existe, el nuevo archivo será `nombre-1.cif`.
-   Si `nombre-1.cif` también existe, será `nombre-2.cif`, y así sucesivamente.

> Para el archivo de ejemplo la password es `1234`.