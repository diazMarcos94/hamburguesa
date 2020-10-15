<!DOCTYPE html>
<html lang="es">
<head>
    <h1>informe tp</h1>
</head>
<body>
<h2>Objetivo</h2>
<p>A partir de un archivo .txt en el cual contendrá la receta para una hamburguesa el objetivo
de este trabajo es realizar un .C en el cual se leerá dicho archivo, utilizara threads
simulando 3 equipos los cuales tendrán como objetivo realizar la hamburguesa
controlando las acciones que están realizando, controlando la utilización del
equipamiento compartido (salero, plancha, horno) ya que lo puede utilizar un equipo a la
vez. Para finalizar las acciones de los equipos serán enviadas a un archivo .txt el cual
contendrá toda la información correspondiente.</p>
<h2>Procedimiento</h2>
<p>Defino los mutex y los semáforos que van a compartir los hilos y una variable de tipo
entera compartida que me ayudara a saber si algún equipo gano, Creo la estructura de los
semáforos y dentro de ella coloco todos los semáforos que utilizaran los hilos, Creo una
estructura paso que posee una acción de la receta y un "array" con los ingredientes, Creo
los parámetros de los hilos, en la función imprimir Acción Imprimirá cada acción realizada
por los equipos, creo las funciones:
Cortar: en la cual creo el nombre de la acción de la función “cortar”, el puntero para
pasarle la referencia de memoria (data) del struct pasado por parámetro (la cual es un
puntero), llamo a la función imprimir le paso el struct y la acción de la función, Uso sleep
para simular que que pasa tiempo, Doy la señal a la siguiente acción (cortar me habilita
mezclar)</p>

<p>Mezclar: ídem a cortar pero me habilita a salar la mezcla

Salar: Bloqueo el semáforo salero único para que otro equipo no pueda usarlo, Creo el
nombre de la acción de la función, Creo el puntero para pasarle la referencia de memoria
(data) del struct pasado por parámetro (la cual es un puntero), Llamo a la función imprimir
le paso el struct y la acción de la función, Uso sleep para simular que pasa tiempo,
Desbloqueo el semáforo salero único para que otro equipo pueda usarlo, Doy la señal a la
siguiente acción (salar me habilita a armar los medallones)

Armar medallones: ídem mezclar pero me habilita a cocinar la hamburguesa

Cocinar Hamburguesa: ídem a salar pero me habilita a hornear el pan

Hornear pan: ídem cocinar Hamburguesa, habilitando a cortar la lechuga y el tomate
Cortar extras: ídem armar medallones pero me habilita a armar la hamburguesa
Armar hamburguesa: Creo el nombre de la acción de la función, Creo el puntero para
pasarle la referencia de memoria (data) del struct pasado por parámetro (la cual es un
puntero), Llamo a la función imprimir le paso el struct y la acción de la función, Me subirá
al archivo cual es el equipo ganador, quien salió segundo y tercero, Uso sleep para simular
que que pasa tiempo

Ejecutar receta: Defino los semáforos, Identifico con un puntero los hilos que apuntaran
a cada función, Abro el archivo que contiene la receta, Defino los delimitadores que
separaran las palabra, creo una variable la cual Guardara cada substring de la receta,
Recorro la cantidad de líneas del archivo hasta el final, Leo la línea, Con el strtok leo el
string y si encuentro un delimitador se queda con el string anterior, Como el primer string
siempre es la acción, guardo en sub la acción de cada línea, y utilizo la estructura pasos en
donde la voy a almacenar, Recorro el resto de la línea , al finalizar cierro el archivo receta.
Inicializo los semáforos, creo los hilos a todos les paso el struct creado (el mismo a todos
los hilos) ya que todos comparten los semáforos, realizo el join de todos los hilos
validando que el hilo se allá creado bien finalizando con la destrucción de los semáforos y
los mutex.

MAIN: Dentro del Main inicializo los semáforos compartidos, creo las variables los hilos
de los equipos, inicializo los hilos de los equipos y realizo el join de todos los hilos y
destruyo los semáforos compartidos</p>

<h2>Dificultades Obtenidas</h2>
<p>Obtuve dos complicaciones con este trabajo, el primero fue con el lenguaje ya que lo
desconocía, y la principal dificultad fue en la lectura del .txt y en enviar el resultado a un
archivo el cual se debería crear, la solución la obtuve mediante asesoramiento de los
profesores, videos y páginas web</p>

</body>
</html>
