#include<stdio.h>
#include<stdlib.h>

void assembler(char archivo[]);
char mayus(char min);
int valida(char linea[]);
int esLetra(char let);
int esHex(char val);
int opcode(char linea[]);
int toHex(char hx);

/*int main(void)
{
  char nombre[50];
  printf("inserta el nombre del archivo:\t");
  scanf("%s", nombre);

  assembler(nombre);
  }*/

void assembler(char archivo[])
{
	
  int error = 0;
  FILE *fp,*fp2;
  char linea[80];
  char linea2[80];
  char lineaArgs[80];

  int primera = 0b00000000;
  int segunda = 0b00000000;
  int val = 0;
  int op;
  int hx1 = 0;
  int hx2 = 0;
  char tkn[6] = " #$,\n";
  char *ptr;
  int numArg = 0;
  int i = 0;
  //Arreglos con el opcode y mode
  int opCode[16] = {0b00000000,0b00000001,0b00000010,0b00000011,0b00000100,0b00000101,0b00000110,0b00000111,0b00001000,0b00001001,0b00000010,0b00001011,0b00001100,0b00001101,0b00001110,0b00001111};
  int mode[4] = {0b11000000,0b10000000,0b01000000,0b00000000};
  fp = fopen (archivo, "r"); 
  fp2 = fopen ("file.hex", "w");

  if(fp == NULL)
    {
      printf("No existe el archivo");
    }

  else
    {
      //Mientras podamos leer lineas e el archivo
      while(fgets(linea, 80, fp) != NULL)
	{ 
	  //Reiniciamos nuestras variables
	  ptr = NULL;
	  numArg = 0;
	  primera = 0b00000000;
	  segunda = 0b00000000;

	  //la cuenta de los rengloes leidos para poder decir en que linea esta el error
	  error += 1;

	  //nos dice que el mode
	  val = valida(linea);

	  //Copias de la linea leida
	  for(i = 0; i<80;i++)
	    {
	      linea2[i] = linea[i];
	      lineaArgs[i]= linea[i];
	    }
      
	  //Devuelve el numero correspondiente a la posicion+1 de el opcode en el arreglo
	  op = opcode(linea2);
    
	  //En caso de error por sintaxis etc..
	  if(val == 0||op == 0)
	    {
	      printf("error en la linea: %d\n", error);
	      exit(0);
	    }

	  //nos saltamos las lineas que son \n
	  else if(linea[0] != '\n')
	    {

	      // printf("%s\n",linea);
	      //dividimos la linea leida en sus comandos por tokens
	      ptr = strtok( linea, tkn );
	  
	      //Mientras encuentre tokens en la linea
	      while( (ptr = strtok( NULL, tkn )) != NULL )
		{
	      
		  //printf("%s\n",ptr);

		  //contamos cuantos argumentos hay
		  numArg += 1;

		  //de que nos den un hex como src
		  if((val==1 || val == 2)&&numArg == 1)
		    {
		      hx1= 0x10 * toHex(mayus(ptr[0]));
		      hx2=toHex(mayus(ptr[1]));
		      segunda = hx1 + hx2;
		    }

		  //Si no nos dan un hex
		  if(val == 3 && numArg == 1)
		    {
		      if(ptr[0]=='A')
			{
			  segunda=0b00000000;
			}
		      if(ptr[0]=='B')
			{
			  segunda=0b10000000;
			}
		    }
	      

		  //////////////////////////////para los distintos tipos de validacion:

		  //Add,Sub,And,Or,Xor
		  if((op>=1 && op<=5) && val != 4)
		    {
		      if(numArg > 3)
			{
			  printf("error con los argumentos en la linea %d", error);
			  exit(0);
			}

		      //Para el segundo argumento 
		      if(numArg == 2)
			{
			  if(ptr[0]=='A')
			    {
			      primera += 0b00000000;
			    }
			  if(ptr[0]=='B')
			    {
			      primera += 0b00100000;
			    }
			}
		      //para el tercer argumento
		      if(numArg == 3)
			{
			  if(ptr[0]=='A')
			    {
			      primera += 0b00000000;
			    }
			  if(ptr[0]=='B')
			    {
			      primera += 0b00010000;
			    }
			}
	     
		    }


		  //Para Not y Dec
		  if((op==6||op == 8) && numArg != 1 && (val == 1 || val == 3))
		    {
		      if(numArg >1)
			{
			  printf("error con los argumentos en la linea %d", error);
			  exit(0);
			}
		    }

		  //Para Mul
		  if(op == 7 && val != 4)
		    {
		      if(numArg >2)
			{
			  printf("error con los argumentos en la linea %d", error);
			  exit(0);
			}
		      if(numArg == 2)
			{
			  if(ptr[0]=='A')
			    {
			      primera += 0b00000000;
			    }
			  if(ptr[0]=='B')
			    {
			      primera += 0b00100000;
			    }
			}
		    }

		  //Para load y store
		  if((op == 9||op == 10)&& val != 4)
		    {
		      if(numArg >2)
			{
			  printf("error con los argumentos en la linea %d", error);
			  exit(0);
			}
		      if(numArg == 2)
			{
			  if(ptr[0]=='A')
			    {
			      primera += 0b00000000;
			    }
			  if(ptr[0]=='B')
			    {
			      primera += 0b00010000;
			    }
			}
		    }

		  //Para bra
		  if(op == 11 && numArg != 1 && val != 4)
		    {
		      if(numArg >1)
			{
			  printf("error con los argumentos en la linea %d", error);
			  exit(0);
			}
		    }
	       
		  //Para bnz y dbnz
		  if((op == 12 || op == 13) && val != 4)
		    {
		      if(numArg >2)
			{
			  printf("error con los argumentos en la linea %d", error);
			  exit(0);
			}
		      if(numArg == 2)
			{
			  if(ptr[0]=='A')
			    {
			      primera += 0b00000000;
			    }
			  if(ptr[0]=='B')
			    {
			      primera += 0b00100000;
			    }
			}
		    }
	       
		  //Para rts, nop, halt
		  if((op == 14 || op == 15 || op == 16) && val == 4)
		    {
		      segunda = 0b00000000;
		      primera = 0b00000000;
		    }
	      
		}
	  
	      primera += mode[val];
	      primera += opCode[op-1];
	 
	      fprintf(fp2,"%X\n", primera);
	      fprintf (fp2,"%X\n", segunda);
	    
	    }
	}
      fclose(fp);
      fclose(fp2);
      printf("archivo creado\n");
    }
}

char mayus(char min)
{
  if(min<='z'&&min>='a')
    {
      return (min - 32);
    }
  else
    {
      return min;
    }
}

int valida(char linea[])
{
	
  int cont = 0;
  int dirMem = 0;// $
  int valIn = 0;// #
  int normal = 0;
  int coma = 0;
  while(linea[cont] != '\0')
    {
      if((linea[cont] == ','||linea[cont] == ' ') && cont < 79)
	{
	  if(linea[cont + 1] == '#')
	    valIn += 1;
	  else if(linea[cont + 1] == '$')
	    dirMem += 1;
	  else if(esLetra(linea[cont + 1]) == 0)
	    return 0;
	  else if(linea[cont +1] == ',')
	    coma += 1;
	  else
	    normal += 1;
	  if(valIn > 1 || dirMem > 1|| coma > 2 ||(valIn != 0 && dirMem != 0))
	    return 0;
	}
      cont += 1;
    }	
  /****************
   * 0- Inv.
   * 1- $
   * 2- #
   * 3- Normal
   * 4- Nada
   ****************/
  if(valIn == 1)
    return 2;
  else if(dirMem == 1)
    return 1;
  else if(normal != 0)
    return 3;
  else
    return 4;
	
}


int esHex(char val)
{
  int cont;
  char hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	
  for(cont = 0; cont < 16; cont += 1)
    {
      if(hex[cont] == val)
	return 1;
    }
	
  return 0;
}

int esLetra(char let)
{
  char validos[28] = {' ','Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M'};
  int contV = 0;
  if(let>='a'&&let<='z')
    {
      let = mayus(let);
    }
  while(contV < 28)
    {
      if(validos[contV] == let )
	return 1;
      contV += 1;
    }
  return 0;
}

int opcode(char linea[])
{
  char s[4] = " ,\n";
  char *token;
  int numPar = 0;
  int op = 0;
  int numOp = 0;
  /* get the first token */
  token = strtok(linea, s);
   
  /* walk through other tokens */
  while( token != NULL ) 
    {
      numPar += 1;
      if(numPar > 4||numOp > 1)
	{
	  printf("Error de sintaxis");
	  exit(0);
	}
      if(strcmp(token,"add")==0)
	{
	  op = 1;
	  numOp += 1;
	  
	}
      else if(strcmp(token,"sub")==0)
	{
	  op = 2;
	  numOp += 1;
	}
      else if(strcmp(token,"or")==0)
	{
	  op = 3;
	  numOp += 1;
	}
      else if(strcmp(token,"and")==0)
	{
	  op = 4;
	  numOp += 1;
	}
      else if(strcmp(token,"xor")==0)
	{
	  op = 5;
	  numOp += 1;
	}
      else if(strcmp(token,"not")==0)
	{
	  op = 6;
	  numOp += 1;
	}
      else if(strcmp(token,"mul")==0)
	{
	  op = 7;
	  numOp += 1;
	}
      else if(strcmp(token,"dec")==0)
	{
	  op = 8;
	  numOp += 1;
	}
      else if(strcmp(token,"load")==0)
	{
	  op = 9;
	  numOp += 1;
	}
      else if(strcmp(token,"store")==0)
	{
	  op = 10;
	  numOp += 1;
	}
      else if(strcmp(token,"bra")==0)
	{
	  op = 11;
	  numOp += 1;
	}
      else if(strcmp(token,"bnz")==0)
	{
	  op = 12;
	  numOp += 1;
	}
      else if(strcmp(token,"dbnz")==0)
	{
	  op = 13;
	  numOp += 1;
	}
      else if(strcmp(token,"rts")==0)
	{
	  op = 14;
	  numOp += 1;
	}
      else if(strcmp(token,"nop")==0)
	{
	  op = 15;
	  numOp += 1;
	}
      else if(strcmp(token,"halt")==0)
	{
	  op = 16;
	  numOp += 1;
	}
      token = strtok(NULL, s);
    }
  //printf("%s Es : \t",linea);
  //printf( " %x\n", op );
  return op;
}

int toHex(char hx)
{
  if(hx == '0')
    return 0x00;
  else if(hx == '1')
    return 0x01;
  else  if(hx == '2')
    return 0x02;
  else  if(hx == '3')
    return 0x03;
  else  if(hx == '4')
    return 0x04;
  else  if(hx == '5')
    return 0x05;
  else  if(hx == '6')
    return 0x06;
  else  if(hx == '7')
    return 0x07;
  else  if(hx == '8')
    return 0x08;
  else  if(hx == '9')
    return 0x09;
  else  if(hx == 'A')
    return 0x0A;
  else  if(hx == 'B')
    return 0x0B;
  else  if(hx == 'C')
    return 0x0C;
  else  if(hx == 'D')
    return 0x0D;
  else  if(hx == 'E')
    return 0x0E;
  else  if(hx == 'F')
    return 0x0F; 
  /*else
    {
    printf("error en el hex");
    exit(0);  
    }*/
}
