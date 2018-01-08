# Proyecto memoria de titulación

Para entender el contexto de este programa revisar en doc/ el pdf memoria.pdf

# Guía de uso de los programas

Para la utilización de los scripts de python se necesita:
Python 2.7.14

Librerias: numpy, scipy, matplotlib

Para el programa del ajuste de los datos de la dirección del viento:

Libreria: libocamlgsl-ocaml-dev

# Uso de programas de búsqueda de parámetros de ajuste de funciones de densidad de probabilidad a muestra de datos

# Parseando los datos
En la carpeta data/ están los datos fuente de las velocidades del viento y los archivos parseados correspondientes obtenidos con los scripts en python que se encuentran en la carpeta programs/wind_direction/parse_scripts y programs/wind_velocity/parse_scripts/.
No es necesario ejecutar estos scripts por que ya están listo los archivos para ser utilizados pero de todas formas están funcionales para probar.
Lo que hacen los scripts en parse_scripts/ es sólo formatear los archivos de datos de muestra para facilitar su uso por parte de los algoritmos.

# Obtener resultados de los experimentos
Dentro de la carpeta pso_alg/ en el archivo main.c se pueden ajustar los valores de la cantidad de experimentos que se quieren realizar y los archivos a utilizar. Para obtener los resultado basta con ejecutar una vez el algoritmo PSO para determinado conjunto de datos. Las repeticiones son sólo para cambiar la semilla de origen de los números aleatorios para así poder validar los resultados.
Para utilzar el programa basta con usar el comando make y luego ejecutar el programa compilado ./pso . De manera predeterminada se imprime por pantalla los resultados. Para guardar los resultados basta con direccionar el flujo a un archivo de salida: ./pso > resultados.txt

# Visualizando los resultados

Para visualizar los resultados están los scripts de gráficos plot_scripts/ y boxplots_scripts/ . Para visualizar los resultados hay que copiar los valores obtenidos en la ejecución del pso y escribirlos en las variables que correspondan dentro del script relacionado. Por ejemplo, para ver la función de Weibull ajustada hay que copiar los parámetros K y C obtenidos del programa (el ejecutable llamado pso) para la velocidad del viento y setearlos como variables en el script plot_weibull.py. Luego sólo queda ejecutar el script (una vez instaladas las librerías solicitadas).

# Modificación de parámetros
En los archivos main.c, pso.c y pso.h, en ambos programas, se pueden modificar los parámetros del algoritmo y los archivos de datos a utilizar para obtener los valores requeridos.
