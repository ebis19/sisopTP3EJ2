#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#define ERROR_CANTIDADARGUMENTOS -1
#define ERROR_CANTIDADTHREAD -2
#define HELP -3
#define OK 0

pthread_mutex_t mutex_counter;
pthread_mutex_t mutex_array;
unsigned int *array_sumas;
unsigned int count;
unsigned int numero_thread;
unsigned int array_resultado[2];
void *figonacci();
void *multiplicar_array();
int thread_handler(int n);
int thread_handler_figonacci(int n);
void *sumar_array();
int verificar_argumentos(int argc, char *argv[]);
int get_help(char comando[]);


int main(int argc, char *argv[]){
    int res=verificar_argumentos(argc,argv);
    if(res == HELP)
        return get_help(argv[0]);
    if (res == ERROR_CANTIDADTHREAD){
        printf("Los argumentos ingresados son invalidos \n");
        get_help(argv[0]);
        return res;
    }
    printf("%d",res);
    count=res;
    thread_handler(count);
    return 0;
}

int thread_handler(int n){
    pthread_t th_multiplicacion;
    pthread_t th_suma;
    thread_handler_figonacci(n);
    pthread_create(&th_multiplicacion,NULL,multiplicar_array,NULL);
    pthread_create(&th_suma,NULL,sumar_array,NULL);
    pthread_join(th_suma, NULL);
    pthread_join(th_multiplicacion, NULL);
    printf("El resultado final es : %d \n",array_resultado[1]-array_resultado[0] );
    free(array_sumas);
}

int thread_handler_figonacci(int n){
    size_t i;
    numero_thread=n;
    pthread_mutex_init(&mutex_counter, NULL);
    pthread_t *th =  malloc(n*sizeof(pthread_t));
    array_sumas = malloc(n*sizeof(int));
    for(i = 0;i<n;i++){
        pthread_create(&(th[i]),NULL,figonacci,NULL);
    }
    for(i = 0; i<n;i++ ){
        pthread_join(th[i], NULL);
    }
    free(th);
    pthread_mutex_destroy(&mutex_counter);
    return 0;
}
int get_help(char comando[]){
    printf("--------------------------------------------\n");
    printf("El programa se ejecuta %s  [numero]\n",comando);
    printf("---------------------------------------------\n");
    printf("\n");
    return HELP;
}

int verificar_argumentos(int argc, char *argv[]){
    int num;
    if(argc!=2)
        return ERROR_CANTIDADARGUMENTOS;
    if(argv[1]=="--help")
        return HELP;
    if(argv[1]=="-h")
        return HELP;
    sscanf(argv[1], "%d", &num);
    if(num >0){
        return num;
    }
    return ERROR_CANTIDADARGUMENTOS;
}

void *figonacci(){
    pthread_mutex_lock(&mutex_counter);
    numero_thread--;
    pthread_mutex_unlock(&mutex_counter);
    int n=numero_thread;
    int i,aux,numero_actual = 0;
    int numero_siguiente = 1;
    int suma = 0;
    for(i = 0 ;i <= n;i++){
        aux = numero_siguiente; 
        numero_siguiente += numero_actual;
        numero_actual = aux;
        suma += numero_actual;
    }
    array_sumas[n]=suma;
}

void *sumar_array(){
    unsigned int sum = 0;
    for (size_t i = 0; i < count; i++)
       sum +=array_sumas[i]; 
    array_resultado[0]=sum;
}

void *multiplicar_array(){
    unsigned int mul = 1;
    for (size_t i = 0; i < count; i++)
       mul *=array_sumas[i];
    array_resultado[1]=mul;
}