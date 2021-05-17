#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

void encrypt(char *array, int array_size)
{
    int i;
    char secret[8] = { 22, 53, 44, 71, 66, 177, 253, 122 };
    for(i = 0; i < array_size; i++){
        array[i] ^= secret[i];
    }
}

void ReplaceLineWith(int line, char *fName, char *textToReplace){

    FILE * fPtr;
    FILE * fTemp;

    char jumpLine[2] = "\n";
    char buffer[50];
    char newline[50];
    int count;
	
    char *newLineCopy = (char*)malloc(sizeof(char)* strlen(textToReplace) + 1);
    strcpy(newLineCopy, textToReplace);
	
    strcat(newLineCopy, jumpLine);

    strcpy(newline, newLineCopy);

    fPtr  = fopen(fName, "r");
    fTemp = fopen("replace.txt", "w");

    count = 0;
    while ((fgets(buffer, 50, fPtr)) != NULL){

        count++;

        //se a linha atual for a linha que tem que substituir
        if (count == line)
            fputs(newLineCopy, fTemp);
        else
            fputs(buffer, fTemp);
    }
    fclose(fPtr);
    fclose(fTemp);

    remove(fName);
    rename("replace.txt", fName);
    }

int CheckForUserPassword(char *fname, char *str, char *consoleMessage, int *line){

    char jumpLine[2] = "\n";
    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];
    char password[50];
    int userFound = 0;
    strcat(str, jumpLine);

	if((fp = fopen(fname, "r")) == NULL) {
	    return -1;
	}

	//Pega linha por linha com o fgets e checa se sao iguais
	while(fgets(temp, 512, fp) != NULL) {

	    if(strcmp(str,temp) == 0) {
                system("cls");
                *line = line_num;
	        printf("Usuario encontrado\nSenha: ");

	        scanf("%s", password);
                userFound = 1;
	        break;
	    }
       	    line_num ++;
	}
	    if(userFound == 0){
                system("cls");
                printf("User not found!");
                return 1;
	}

	    fclose(fp);
	    fp = fopen(fname, "r");

	    for (int i = 0; i < line_num + 1; i++){
            fgets(temp, 512, fp);
	}

	encrypt(password, strlen(password));
	strcat(password, jumpLine);

        fclose(fp);

        if((strcmp(password, temp)) == 0) {
             return 0;
        } 
        else{
            printf("wrong password\n");
            return 1;
        }
}

int Search_in_File(char *fname, char *str) {

    char jumpLine[2] = "\n";
    FILE *fp;
    int line_num = 1;
    int find_result = 0; 
    char temp[512];

    if((fp = fopen(fname, "r")) == NULL) {
        printf("No file found!");
 	return -1;
    }
	
    //Cria uma copia para que o jumpline nao mude a string original parea checagem
    char *strCopy = (char*)malloc(sizeof(char)* strlen(str) + 1);
    strcpy(strCopy, str);

    strcat(strCopy, jumpLine);

    while(fgets(temp, 512, fp) != NULL) {
        if((strcmp(temp, strCopy)) == NULL) {
	    find_result++;
	}
	line_num++;
	}

	if(find_result == 0) {
       	    fclose(fp);
            return -1;
	}
	fclose(fp);
   	return 0;
}

int PrintInANewLine(char *name, char *str){

    FILE *fp;
    fp = fopen(name, "a+");
    fputs(str, fp);
    fclose(fp);
}

int main(){
    system("cls");
    char consoleMessage[40] = "";
    char state[20] = "logging";
    int lookingAtLine = 0;
    while(1){

        int option = 0;
        char userName[100];
        char jumpLine[2] = "\n";
        char toEncrypt[200];
        printf("%s\n\n", consoleMessage);
	    
        if(strstr(state, "logging")){

            printf("Deseja fazer login ou cadastrar-se?\nLogin (1)\nCadastro (2)\n");
            scanf("%d", &option);
	    if (option != 1 && != 2){
		printf("opcao invalida\n");
		break;
	    	}
		
            if(option == 1){

                system("cls");
                printf("Digite o nome de usuario: ");
                scanf("%s", userName);
                if(CheckForUserPassword("Usuarios", userName, consoleMessage, &lookingAtLine) == 0){
                    strcpy(consoleMessage, "Logado com sucesso");
                    strcpy(state, "logged");
                    system("cls");
                }
                else{
                    strcpy(consoleMessage, "Usuario ou senha incorretos");
                    system("cls");
                }

            }
            else if(option == 2){
                system("cls");
                printf("Cadastro\n");
                printf("Digite o nome de usuario: ");
                scanf("%s", userName);
                if(Search_in_File("Usuarios",userName) == 0){
                    strcpy(consoleMessage, "Usuario ja existe");

                    system("cls");
                    continue;
                }
                printf("Digite sua senha: ");
                scanf("%s", toEncrypt);

                encrypt(toEncrypt, strlen(toEncrypt));

                strcat(toEncrypt, jumpLine);
                strcat(userName, jumpLine);
                PrintInANewLine("Usuarios", userName);
                PrintInANewLine("Usuarios", toEncrypt);
                PrintInANewLine("Usuarios", jumpLine);

                strcpy(consoleMessage, "Cadastro concluido!");
                system("cls");
            }
        }
        else if(strstr(state, "logged")){
            printf("Usuario: %s\n", userName);
            printf("Mudar senha (1)\nMudar nome de usuario (2)\nDeslogar (3)\n");
            scanf("%d", &option);

            if(option == 1){
                system("cls");
                printf("Qual sera sua nova senha?\n");
                char newPass[50];
                scanf("%s", newPass);
                encrypt(newPass, strlen(newPass));
                strcat(newPass, jumpLine);
                ReplaceLineWith(lookingAtLine + 1, "Usuarios", newPass);
                strcpy(consoleMessage, "Senha alterada com sucesso!");
                system("cls");
            } 
            else if(option == 2){
                system("cls");
                printf("Qual sera sua novo nome de usuario?\n");
                char newUser[50];
                scanf("%s", newUser);
                if(Search_in_File("Usuarios", newUser) == 0){
                    strcpy(consoleMessage, "Usuario ja existe");

                    system("cls");
                    continue;
                }
                
                ReplaceLineWith(lookingAtLine, "Usuarios", newUser);
                strcpy(userName, newUser);
                strcat(userName, jumpLine);
                strcpy(consoleMessage, "Nome de usuario alterado com sucesso");
                system("cls");
            }
            else if(option == 3){
                lookingAtLine = 0;
                strcpy(state, "logging");
                strcpy(consoleMessage, "Deslogado com sucesso");
                system("cls");
            }
        }
    }

    return 0;
}
