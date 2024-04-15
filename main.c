#include <stdio.h>
#include <stdlib.h>
#include <conio.c>
#include <conio.h>
#include <ctype.h>
#include <time.h>

#define MAXNUMCOL        5
#define RANGO           15
#define POSX            35
#define POSY             4
#define BOLOSMAXIMOS    76
#define BOLOCUPADO       1
#define LIBRE            0
#define VALINI           1
#define POSICIONY        6
#define DISTANCIATABLAS 20
#define ACTIVO           3
#define TABLASMAXIMAS    4
#define VALORV           1
#define VALORF           0
#define VICTORIA         5
#define LOAD             3


#define CT     BLUE
#define CF     LIGHTGRAY
#define TCT    LIGHTGRAY
#define TCF    RED

#define ESC        27
#define ENTER      13

typedef struct{
   int valor;
   int status; // Seleccionada o no.
}casilla;

typedef struct{
   casilla B[MAXNUMCOL],
           I[MAXNUMCOL],
           N[MAXNUMCOL],
           G[MAXNUMCOL],
           O[MAXNUMCOL];
}carton;

typedef struct{
   int cantcartones;
   carton *cartones;
}JUG;


int main()
{
   JUG *jugadores;

   srand(time(NULL));
   int cantidadjugadores;
   int bombo, bombos[BOLOSMAXIMOS],teclado,tiempo,ganar=0,lleno=0;
   bombo = obtenerbolo(bombos);

   printf("%cCu%cntos jugadores ser%cn?",168,160,160);
   scanf("%d", &cantidadjugadores);
   int jugadoresllenos[cantidadjugadores];
   int ganadores[cantidadjugadores];

   for (int ind = 0; ind < cantidadjugadores; ind++)
      ganadores[ind] = 0;
   jugadores = (JUG *)malloc(cantidadjugadores * sizeof(JUG));

   do
   {
      tiempo = time(NULL);
      // Bucle para preguntar la cantidad de cartones por jugador.
      for (int ind = 0; ind < cantidadjugadores; ind++)
      {
         do
         {
            printf("Cuantos cartones para el jugador #%d: ", ind + 1);
            scanf("%d", &(jugadores + ind)->cantcartones);
            if ((jugadores + ind)->cantcartones > TABLASMAXIMAS)
               printf("NO MAS DE 4 CARTONES\n");
         } while ((jugadores + ind)->cantcartones > TABLASMAXIMAS);
         (jugadores + ind)->cartones = (carton *)malloc((jugadores + ind)->cantcartones * sizeof(carton));
      }

      for (int ind = 0; ind < cantidadjugadores; ind++)
         jugadoresllenos[ind] = VALORV;
      rellenarcartones(jugadores, cantidadjugadores);
      system("cls");
      imprimircartones(jugadores, cantidadjugadores, jugadoresllenos);
      // Asignación de bolos libres
      for (int ind = 0; ind < BOLOSMAXIMOS; ind++)
         bombos[ind] = LIBRE;
      ganar = 0;
      lleno = 0;

    do
      {
         _setcursortype(0);
         do
         {
            fflush(stdin);
            teclado = tolower(getch());
         } while (teclado != ENTER && teclado != ESC);
         if (teclado == ESC)
         {
            system("cls");
            estaditicas(ganadores, cantidadjugadores);
            system("pause");
            system("cls");
         }
         if (teclado == ENTER)
         {
            bombo = obtenerbolo(bombos);
            imprimirbolo(bombo);
            for (int ind = 0; ind < cantidadjugadores; ind++)
               validarjugador((jugadores + ind), bombo);
            imprimircartones(jugadores, cantidadjugadores, jugadoresllenos);
            for (int ind = 0; ind < cantidadjugadores; ind++)
            {
               if (validarvictoria((jugadores + ind), ind))
               {
                  ganar = 1;
                  ganadores[ind]++;
               }
            }
         }
      } while (!ganar);

      ganar = 0;
      gotoxy(POSX, POSY);
      printf("%cQuieren jugar el bing%cn? (S)si o (N)no", 168,162);
      do
      {
         fflush(stdin);
         teclado = tolower(getch());
      } while (teclado != 's' && teclado != 'n');
      if (teclado == 's')
      {
         gotoxy(POSX, POSY);
         printf("                                                                    ");
         for (int ind = 0; ind < cantidadjugadores; ind++)
         {
            gotoxy(POSX, POSY);
            printf("El jugador #%d %cdesea jugar bingon? (S)si o (N)no", ind + 1, 168 );
            do
            {
               fflush(stdin);
               teclado = tolower(getch());
            } while (teclado != 's' && teclado != 'n');

            if (teclado == 's')
            {
               jugadoresllenos[ind] = 1;
               lleno = 1;
            }
            else if (teclado == 'n')
               jugadoresllenos[ind] = 0;
         }
      }
      gotoxy(POSX, POSY);
      system("cls");

      if (lleno)
      {
         do
         {
            _setcursortype(0);
            do
            {
               fflush(stdin);
               teclado = tolower(getch());
            } while (teclado != ENTER && teclado != ESC);
            if (teclado == ESC)
            {
               system("cls");
               estaditicas(ganadores, cantidadjugadores);
               system("pause");
               system("cls");
            }
            if (teclado == ENTER)
            {
               bombo = obtenerbolo(bombos);
               imprimirbolo(bombos);
               for (int ind = 0; ind < cantidadjugadores; ind++)
                  if (jugadoresllenos[ind])
                     validarjugador((jugadores + ind), bombo);
               imprimircartones(jugadores, cantidadjugadores, jugadoresllenos);
               for (int ind = 0; ind < cantidadjugadores; ind++)
               {
                  if (validarvictoriaBINGON((jugadores + ind), ind))
                  {
                     ganar = 1;
                     ganadores[ind]++;
                     // break;
                  }
               }
            }
         } while (!ganar);
      }
      gotoxy(POSX, POSY);
      printf("%cQuiere jugar una vez m%cs? (S)si o (N)no", 168,160);
      do
      {
         fflush(stdin);
         teclado = tolower(getch());
      } while (teclado != 's' && teclado != 'n');
      if (teclado == 's')
         for (int ind = 0; ind < cantidadjugadores; ind++)
            free((jugadores + ind)->cartones);
      system("cls");
   } while (teclado != 'n');
   free(jugadores);
   gotoxy(5, 2);
   setcolor(WHITE, BLACK);
   printf("El juego ha terminado\n");
   printf("\n");
   return 0;
}
/*
   Función: imprimircartones
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int cantidadjugadores, cantidad de jugadores para el juego.
               int full, arreglo para alidar e imprimir.
   Objetivo: imprimir los cartones de cada jugador.
   Retorno: Ninguno.
*/
void imprimircartones(JUG *jugadores, int cantidadjugadores, int full[])
{
   int posx, posy = POSICIONY, temporal;
   for (int posicion = 0; posicion < cantidadjugadores; posicion++)
   {
      if (full[posicion])
      {
         for (int ind = 0; ind < (jugadores + posicion)->cantcartones; ind++)
         {
            posx = DISTANCIATABLAS * (posicion + 1);
            temporal = posx;
            gotoxy(posx, posy);
            setcolor(RED, BLACK);
            printf("      %d-%d", posicion + 1, ind + 1);
            posy++;
            gotoxy(posx, posy);
            setcolor(BLACK, YELLOW);
            printf(" B  I  N  G  O ");
            posy++;
            for (int temp = 0; temp < MAXNUMCOL; temp++)
            {
               gotoxy(posx, posy);
               if ((jugadores + posicion)->cartones[ind].B[temp].status != BOLOCUPADO)
                  setcolor(RED, WHITE);
               else if ((jugadores + posicion)->cartones[ind].B[temp].status == BOLOCUPADO)
                  setcolor(RED, YELLOW);
               printf("%3d", (jugadores + posicion)->cartones[ind].B[temp].valor);

               posx += LOAD;
               gotoxy(posx, posy);

               if ((jugadores + posicion)->cartones[ind].I[temp].status != BOLOCUPADO)
                  setcolor(RED, WHITE);
               else if ((jugadores + posicion)->cartones[ind].I[temp].status == BOLOCUPADO)
                  setcolor(RED, YELLOW);
               printf("%3d", (jugadores + posicion)->cartones[ind].I[temp].valor);

               posx += LOAD;
               gotoxy(posx, posy);

               if ((jugadores + posicion)->cartones[ind].N[temp].status != BOLOCUPADO)
                  setcolor(RED, WHITE);
               else if ((jugadores + posicion)->cartones[ind].N[temp].status == BOLOCUPADO)
                  setcolor(RED, YELLOW);
               if ((jugadores + posicion)->cartones[ind].N[temp].valor == VALINI)
                  setcolor(RED, RED);

               printf("%3d", (jugadores + posicion)->cartones[ind].N[temp].valor);

               posx += LOAD;
               gotoxy(posx, posy);

               if ((jugadores + posicion)->cartones[ind].G[temp].status != BOLOCUPADO)
                  setcolor(RED, WHITE);
               else if ((jugadores + posicion)->cartones[ind].G[temp].status == BOLOCUPADO)
                  setcolor(RED, YELLOW);
               printf("%3d", (jugadores + posicion)->cartones[ind].G[temp].valor);

               posx += LOAD;
               gotoxy(posx, posy);

               if ((jugadores + posicion)->cartones[ind].O[temp].status != BOLOCUPADO)
                  setcolor(RED, WHITE);
               else if ((jugadores + posicion)->cartones[ind].O[temp].status == BOLOCUPADO)
                  setcolor(RED, YELLOW);
               printf("%3d", (jugadores + posicion)->cartones[ind].O[temp].valor);

               posx = temporal;
               posy++;
            }
            colordefault();
         }
         posy = POSICIONY;
      }
   }
}
/*
   Función: rellenarcartones
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int tablasmaximas, cantidad de tablas maximas.
   Objetivo: Rellenar con valores al azar los cartones de cada jugador.
   Retorno: Ninguno.
*/
int rellenarcartones( JUG *jugadores, int tablasmaximas)
{
   int valor;
   for (int posicion = 0; posicion < tablasmaximas; posicion++)
      for(int ind = 0; ind < (jugadores+posicion)->cantcartones; ind++)
         for (int temp = 0; temp < MAXNUMCOL; temp++)
         {
            do
            {
               valor = randrange(1,RANGO)+ 0 * RANGO;
            }while (!validarbolo((jugadores+posicion),valor,posicion));
            (jugadores+posicion)->cartones[ind].B[temp].valor = valor;
            do
            {
               valor = randrange(1,RANGO)+ 1 * RANGO;
            }while (!validarbolo((jugadores+posicion),valor,posicion));
            (jugadores+posicion)->cartones[ind].I[temp].valor = valor;
            do
            {
               valor = randrange(1,RANGO)+ 2 * RANGO;
            }while (!validarbolo((jugadores+posicion),valor,posicion));
            (jugadores+posicion)->cartones[ind].N[temp].valor = valor;
            do
            {
               valor = randrange(1,RANGO)+ 3 * RANGO;
            }while (!validarbolo((jugadores+posicion),valor,posicion));
            (jugadores+posicion)->cartones[ind].G[temp].valor = valor;
            do
            {
               valor = randrange(1,RANGO)+ 4 * RANGO;
            }while (!validarbolo((jugadores+posicion),valor,posicion));
            (jugadores+posicion)->cartones[ind].O[temp].valor = valor;

            (jugadores+posicion)->cartones[ind].N[2].valor = VALINI;
            (jugadores+posicion)->cartones[ind].N[2].valor = BOLOCUPADO;
         }
}
/*
   Función: validarbolo
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int value, valor con el que se validara el bolo.
               int carton, posicion que se le ingresara.
   Objetivo: validar los bombos que saldran al azar.
   Retorno: Ninguno.
*/
int validarbolo(JUG *jugadores, int value , int carton)
{

   for (int ind = 0; ind < MAXNUMCOL; ind++)
   {
      if (jugadores->cartones[carton].B[ind].valor == value)

         return 0;

         if (jugadores->cartones[carton].I[ind].valor == value)

         return 0;

            if (jugadores->cartones[carton].N[ind].valor == value)

         return 0;

               if (jugadores->cartones[carton].G[ind].valor == value)

         return 0;

                  if (jugadores->cartones[carton].O[ind].valor == value)

         return 0;
   }
   return 1;
}
/*
   Función: imprimirbolo
   Argumentos: int bolo, variable para el rango de numeros disponible.

   Objetivo: imprimir los bolos que vayan saliendo entre 0 a 75.
   Retorno: Ninguno.
*/
void imprimirbolo (int bolo)
{
   setcolor(BLUE,GREEN);
   gotoxy(POSX,POSY);
   if (bolo >0 && bolo <= 15)
      printf("BOLO: B-%d ", bolo);
   else if (bolo > 15 && bolo <= 30)
      printf("BOLO: I-%d ", bolo);
   else if (bolo > 30 && bolo <= 45)
      printf("BOLO: N-%d ", bolo);
   else if (bolo > 45 && bolo <= 60)
      printf("BOLO: G-%d ", bolo);
   else
      printf("BOLO: O-%d ", bolo);
}
/*
   Función: obtenerbolo
   Argumentos: int valido, verificador para los bolos ya existentes.

   Objetivo: Que no se muestren bolos repetidos cuando se presione enter.
   Retorno: ind.
*/
int obtenerbolo (int valido[])
{
   int ind;
   do
   {
      ind = randrange(1,BOLOSMAXIMOS);
      if (valido[ind] == LIBRE)
         valido[ind] = BOLOCUPADO;
      else
         ind = 0;
   }while (!ind);
   return ind;
}
/*
   Función: randrange
   Argumentos: int a, valor numero 1.
               int b, valor numero 2.

   Objetivo: que se cree un numero aleatorio.
   Retorno: el numero aleatorio creado.
*/

int randrange(int a, int b)
{
   return rand() % (b - a + 1 ) + a;
}
/*
   Función: setcolor
   Argumentos: int ct, color del texto
               int cf, color del fondo
   Objetivo: declarar los colores de texto y fondo
   Retorno: Ninguno.
*/
void setcolor(int ct, int cf)
{
   textcolor(ct);
   textbackground(cf);
}
/*
   Función: colordefault
   Argumentos: no contiene
   Objetivo: declarar los colores
   Retorno: Ninguno.
*/
void colordefault()
{
   textcolor(LIGHTGRAY);
   textbackground(BLACK);
}
/*
   Función: validarjugador
   Argumentos: JUG *jugadores, estructura que se le haran los cambios
               int bolo, valor que se verificara.
   Objetivo: chequear los bolos en los cartones del jugador.
   Retorno: Ninguno.
*/
void validarjugador(JUG *jugadores, int bolo)
{
   for (int ind = 0; ind < jugadores->cantcartones; ind++)
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if ((jugadores)->cartones[ind].B[temp].valor == bolo)

            (jugadores)->cartones[ind].B[temp].status = BOLOCUPADO;

         if ((jugadores)->cartones[ind].I[temp].valor == bolo)

            (jugadores)->cartones[ind].I[temp].status = BOLOCUPADO;

         if ((jugadores)->cartones[ind].N[temp].valor == bolo)

            (jugadores)->cartones[ind].N[temp].status = BOLOCUPADO;

         if ((jugadores)->cartones[ind].G[temp].valor == bolo)

            (jugadores)->cartones[ind].G[temp].status = BOLOCUPADO;

         if ((jugadores)->cartones[ind].O[temp].valor == bolo)

            (jugadores)->cartones[ind].O[temp].status = BOLOCUPADO;

      }
   return;
}
/*
   Función: validarvictoriaCOL
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int jugadornum, numero del jugador que se evaluara si gano.
   Objetivo: valida la victoria de el jugador elegido de manera horizontal.
   Retorno: 0 si se cumplen las condiciones.
*/
int validarvictoriaCOL(JUG *jugadores, int jugadornum)
{
   int contador = 0;

   for (int ind = 0; ind < jugadores->cantcartones; ind++)
   {
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].B[temp].status == BOLOCUPADO)
         {
            contador++;
            if (jugadores->cartones[ind].I[temp].status == BOLOCUPADO)
            {
               contador++;
               if (jugadores->cartones[ind].N[temp].status == BOLOCUPADO)
               {
                  contador++;
                  if (jugadores->cartones[ind].G[temp].status == BOLOCUPADO)
                  {
                     contador++;
                     if (jugadores->cartones[ind].O[temp].status == BOLOCUPADO)
                     {
                        contador++;
                        if (contador == VICTORIA)
                        {
                           avisoganador(jugadores, jugadornum, ind);
                           return 1;
                        }
                     }
                     else
                        contador = 0;
                  }
                  else
                     contador = 0;
               }
               else
                  contador = 0;
            }
            else
               contador = 0;
         }
         else
            contador = 0;
      }
      contador = 0;
   }

   return 0;
}
/*
   Función: validarvictoriaFIL
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int jugadornum, numero del jugador que se evaluara si gano.
   Objetivo: valida la victoria de el jugador elegido de manera vertical.
   Retorno: 0 si se cumplen las condiciones.
*/
int validarvictoriaFIL(JUG *jugadores, int jugadornum)
{
   int contador = 0;

   for (int ind = 0; ind < jugadores->cantcartones; ind++)
   {
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].B[temp].status == BOLOCUPADO)
            contador++;
         else
            contador = 0;
         if (contador == VICTORIA)
         {
            avisoganador(jugadores, jugadornum, ind);
            return 1;
         }
      }
      contador = 0;
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].I[temp].status == BOLOCUPADO)
            contador++;
         else
            contador = 0;
         if (contador == VICTORIA)
         {
            avisoganador(jugadores, jugadornum, ind);
            return 1;
         }
      }
      contador = 0;
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].N[temp].status == BOLOCUPADO)
            contador++;
         else
            contador = 0;
         if (contador == VICTORIA)
         {
            avisoganador(jugadores, jugadornum, ind);
            return 1;
         }
      }
      contador = 0;
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].G[temp].status == BOLOCUPADO)
            contador++;
         else
            contador = 0;
         if (contador == VICTORIA)
         {
            avisoganador(jugadores, jugadornum, ind);
            return 1;
         }
      }
      contador = 0;
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].O[temp].status == BOLOCUPADO)
            contador++;
         else
            contador = 0;
         if (contador == VICTORIA)
         {
            avisoganador(jugadores, jugadornum, ind);
            return 1;
         }
      }
      contador = 0;
   }
   return 0;
}
/*
   Función: validarvictoriaDIAG
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int jugadornum, numero del jugador que se evaluara si gano.
   Objetivo: valida la victoria de el jugador elegido de manera diagonal, usando la diagonal principal y secundaria de los arreglos.
   Retorno: 0 si se cumplen las condiciones.
*/
int validarvictoriaDIAG(JUG *jugadores, int jugadornum)
{
   int contador = 0;
   for (int ind = 0; ind < jugadores->cantcartones; ind++)
   {
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].B[0].status == BOLOCUPADO)
         {
            contador++;
            if (jugadores->cartones[ind].I[1].status == BOLOCUPADO)
            {
               contador++;
               if (jugadores->cartones[ind].N[2].status == BOLOCUPADO)
               {
                  contador++;
                  if (jugadores->cartones[ind].G[3].status == BOLOCUPADO)
                  {
                     contador++;
                     if (jugadores->cartones[ind].O[4].status == BOLOCUPADO)
                     {
                        contador++;
                        if (contador == VICTORIA)
                        {
                           avisoganador(jugadores, jugadornum, ind);
                           return 1;
                        }
                     }
                     else
                        contador = 0;
                  }
                  else
                     contador = 0;
               }
               else
                  contador = 0;
            }
            else
               contador = 0;
         }
         else
            contador = 0;
      }
      contador = 0;
   }
   contador = 0;

   for (int ind = 0; ind < jugadores->cantcartones; ind++)
   {
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].B[4].status == BOLOCUPADO)
         {
            contador++;
            if (jugadores->cartones[ind].I[3].status == BOLOCUPADO)
            {
               contador++;
               if (jugadores->cartones[ind].N[2].status == BOLOCUPADO)
               {
                  contador++;
                  if (jugadores->cartones[ind].G[1].status == BOLOCUPADO)
                  {
                     contador++;
                     if (jugadores->cartones[ind].O[0].status == BOLOCUPADO)
                     {
                        contador++;
                        if (contador == VICTORIA)
                        {
                           avisoganador(jugadores, jugadornum, ind);
                           return 1;
                        }
                     }
                     else
                        contador = 0;
                  }
                  else
                     contador = 0;
               }
               else
                  contador = 0;
            }
            else
               contador = 0;
         }
         else
            contador = 0;
      }
      contador = 0;
   }
   return 0;
}
/*
   Función: validarvictoriaBINGON
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int jugadornum, numero del jugador que se evaluara si gano.
   Objetivo: valida la victoria del carton completo.
   Retorno: 0 si se cumplen las condiciones.
*/
int validarvictoriaBINGON(JUG *jugadores, int jugadornum)
{
   int contador = 0;

   for (int ind = 0; ind < jugadores->cantcartones; ind++)
   {
      for (int temp = 0; temp < MAXNUMCOL; temp++)
      {
         if (jugadores->cartones[ind].B[temp].status == BOLOCUPADO)
         {
            contador++;
            if (jugadores->cartones[ind].I[temp].status == BOLOCUPADO)
            {
               contador++;
               if (jugadores->cartones[ind].N[temp].status == BOLOCUPADO)
               {
                  contador++;
                  if (jugadores->cartones[ind].G[temp].status == BOLOCUPADO)
                  {
                     contador++;
                     if (jugadores->cartones[ind].O[temp].status == BOLOCUPADO)
                     {
                        contador++;
                        if (contador == VICTORIA * VICTORIA)
                        {
                           avisoganador(jugadores, jugadornum, ind);
                           return 1;
                        }
                     }
                     else
                        contador = 0;
                  }
                  else
                     contador = 0;
               }
               else
                  contador = 0;
            }
            else
               contador = 0;
         }
         else
            contador = 0;
      }
      contador = 0;
   }

   return 0;
}
/*
   Función: estadisticas
   Argumentos: int ganadores, se verifican los ganadores en el arreglo
               int cantidadjugadores, variable que almacena el numero de jugadores agregados

   Objetivo: decir la cantidad de victorias que lleva cada jugador.
   Retorno: ninguno.
*/
void estaditicas(int ganadores[], int cantidadjugadores)
{
   for (int ind = 0; ind < cantidadjugadores; ind++)
      printf("El jugador %d, lleva %d victorias\n", ind + 1, ganadores[ind]);

}
/*
   Función: validarvictoria
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int jugadornum, numero del jugador que se evaluara si gano.

   Objetivo: validar la victoria del jugador de las 3 formas posibles.
   Retorno: 0 si se cumplen las condiciones.
*/
int validarvictoria(JUG *jugadores, int jugadornum)
{

   if (validarvictoriaCOL(jugadores, jugadornum))

      return 1;

   if (validarvictoriaFIL(jugadores, jugadornum))

      return 1;

   if (validarvictoriaDIAG(jugadores, jugadornum))

      return 1;

   return 0;
}
/*
   Función: avisoganador
   Argumentos: JUG *jugadores, estructura que se utilizara para realizar los cambios.
               int jugadornum, numero del jugador que gano.
               int carton, el numero del carton del jugador que gano

   Objetivo: imprimir el aviso de donde gano el jugador.
   Retorno: ninguno.
*/
void avisoganador(JUG *jugadores, int jugadornum, int carton)
{
   setcolor(RED, WHITE);
   gotoxy(POSX, POSY - 1);
   printf("Victoria para el Jugador %d en el carton %d (%d-%d).\n", jugadornum + 1, carton + 1, jugadornum + 1, carton + 1);
   colordefault();
}
