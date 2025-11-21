## LAB 2: Sistemas Operativos 2024
### Implementacion de Semaforos en XV6.

### Autores: 
- Mauro Centurion 
- Felipe Tomas Pagani

## Sobre la implementacion;
- Los semaforos se inicializan en el main.c
- Si se quiere llama sem_up de un semaforo pasado su limite inicial, se devuelve -1 y se procede a aumentar de igual forma.
- Si se llama a sem_close de un semaforo que esta abierto, este se cierra.