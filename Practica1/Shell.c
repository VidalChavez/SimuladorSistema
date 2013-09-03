#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(void)
{
	char cmd[64],arg1[64],arg2[64];
	int n;
	system("clear");
		do{	
		
		printf("El Chell -> $");
		scanf("%s", cmd);
		if(strcmp("mdump", cmd) == 0) 
		{
			scanf("%s", arg1);
			scanf("%s", arg2);
			printf("hola tu metiste: %s %s %s\n", cmd, arg1, arg2);
		}
		else if(strcmp("assembler", cmd) == 0)
		{
			scanf("%s", arg1);
			//printf("el argumento fue= %s", arg1);
			assembler(arg1);	
		}
		else if(strcmp("run", cmd) == 0)
		{
			scanf("%d", &n);
		
		}
		else if(strcmp("go", cmd) == 0)
		{
			
		}
		else if(strcmp("next", cmd) == 0)
		{

		}
		else if(strcmp("loader", cmd) == 0)
		{

		}
		else if(strcmp("rdmp", cmd) == 0)
		{

		}
		else if(strcmp("input", cmd) == 0)
		{
			scanf("%s", arg1);
			scanf("%s", arg2);
		}
		else if(strcmp("?", cmd) == 0)
		{
			printf("Los comandos validos para \"El Chell\" son:\n");
			printf("loader <file.hex>\n");
			printf("assembler <file.txt>\n");
			printf("run <n>\n");
			printf("go\n");
			printf("next\n");
			printf("mdump <low> <high>\n");
			printf("rdump\n");
			printf("input <reg> <val>\n");
			printf("? <-- El comando que estas utilizando\n");
			printf("quit\n");

		}
		else if(strcmp("quit",cmd) == 0)
		{
			printf("Adios!! :)\n");
		}
		else if(strcmp("clear", cmd) == 0)
		{
			system("clear");
		}
		else
		{
			printf("Opcion no valida, porfafor intente de nuevo\n");
		}
	}while(strcmp("quit", cmd) != 0);

}
