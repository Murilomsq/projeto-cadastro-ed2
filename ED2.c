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
    fTemp = fopen("replace.tmp", "w");


    count = 0;
    while ((fgets(buffer, 50, fPtr)) != NULL)
    {
        count++;

        /* If current line is line to replace */
        if (count == line)
            fputs(textToReplace, fTemp);
        else
            fputs(buffer, fTemp);
    }
    fclose(fPtr);
    fclose(fTemp);

    remove(fName);
    rename("replace.tmp", fName);

    printf("\nSuccessfully replaced '%d' line with '%s'.", line, newline);


}

int CheckForUserPassword(char *fname, char *str, char *consoleMessage){

    char jumpLine[2] = "\n";
    FILE *fp;
	int line_num = 1;
	int find_result = 0;
	char temp[512];
	char password[50];
	int userFound = 0;
    strcat(str, jumpLine);

	if((fp = fopen(fname, "r")) == NULL) {
        consoleMessage = "No file found!";
		return -1;
	}

	//Pega linha por linha com o fgets e checa se está igual
	while(fgets(temp, 512, fp) != NULL) {



		if(strcmp(str,temp) == 0) {
            system("cls");
			printf("User found\nPassword: ");

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

	//Creating a copy so the jumpline for checking doesnt change the original string to be checked
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

	if(fp) {
		fclose(fp);
	}
   	return 0;
}

void ChangeLine2Content(char *fName, char *toChange){
    FILE *fp;
    fp = fopen(fName, "r+");
    char temp[50];

        fgets(temp, 50, fp);
        strcpy(temp, toChange);
        printf("%s", temp);

}

int PrintInANewLine(char *name, char *str){

    FILE *fp;
    fp = fopen(name, "a+");
    fputs(str, fp);
    fclose(fp);
}


int main(){

    ReplaceLineWith(2,"Usuarios", "flp");
    char consoleMessage[40] = "";
    char state[20] = "logging";
    while(1){
        int option = 0;
        char userName[100];
        char jumpLine[2] = "\n";
        char toEncrypt[200];
        printf("%s\n\n", consoleMessage);
        if(strstr(state, "logging")){

            printf("Deseja fazer login ou cadastrar-se?\nLogin (1)\nCadastro (2)\n");
            scanf("%d", &option);


            if(option == 1){
                system("cls");
                printf("Digite o nome de usuario: ");
                scanf("%s", userName);
                CheckForUserPassword("Usuarios", userName, consoleMessage);
                strcpy(consoleMessage, "Logado com sucesso");
                strcpy(state, "logged");
                system("cls");

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

                char *encryptedCopy;

                encrypt(toEncrypt, strlen(toEncrypt));

                encryptedCopy = (char*)malloc(sizeof(char)*strlen(toEncrypt) + 1);
                for(int i = 0; i < strlen(toEncrypt) + 1; i++){
                    encryptedCopy[i] = toEncrypt[i];
                }

                strcat(toEncrypt, jumpLine);
                strcat(userName, jumpLine);
                PrintInANewLine("Usuarios", userName);
                PrintInANewLine("Usuarios", toEncrypt);
                PrintInANewLine("Usuarios", jumpLine);


                printf("Sua senha sera armazenada na forma: %s\n", encryptedCopy);
                *consoleMessage = "Cadastro concluido";
            }
        }
        else if(strstr(state, "logged")){
            printf("Mudar senha (1)\nMudar nome de usuario (2)\nDeslogar (3)");
            scanf("%d", &option);
        }




    }







    return 0;
}

