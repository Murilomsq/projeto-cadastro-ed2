#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>

void encrypt(char *array, int array_size)
{
    int i, j = 0;
    char secret[8] = {22, 53, 44, 71, 66, 177, 253, 122};
    for (i = 0; i < array_size; i++, j++)
    {
        //Recicla a "secret" quando a string tem mais de 8 digitos
        if (j < 8)
        {
            array[i] ^= secret[j];
            continue;
        }
        j -= 8;
    }
}

bool StringHasSpace(char *stringToCheck)
{

    for (int i = 0; i < strlen(stringToCheck) + 1; i++)
    {
        if (isspace(stringToCheck[i]) != 0)
        {
            return true;
        }
    }
    return false;
}

int DeleteUser(char *fName, int line)
{

    FILE *fPtr;
    FILE *fTemp;

    char jumpLine[2] = "\n";
    char buffer[50];
    char newline[50];
    int count;

    fPtr = fopen(fName, "r");
    fTemp = fopen("replace.txt", "w");

    count = 0;
    //Lê todas as linhas do arquivo de texto
    while ((fgets(buffer, 50, fPtr)) != NULL)
    {

        count++;

        //Imprime todas as linhas no novo arquivo, menos a do usuário a ser deletado
        if (count != line && count != line + 1 && count != line + 2)
        {
            fputs(buffer, fTemp);
        }
    }

    //fecha os arquivos e substitui o arquivo temporario pelo original
    fclose(fPtr);
    fclose(fTemp);

    remove(fName);
    rename("replace.txt", fName);

    return 0;
}

int GetNumberOfUsers(char *fName)
{

    FILE *fp;
    int numOfUsers = 0;
    int nextLineToGet = 1, actualLine = 1;
    char buffer[50];

    fp = fopen(fName, "r");
    while (fgets(buffer, 50, fp) != NULL)
    {
        //Soma um usuario a cada 3 linhas
        if ((actualLine + 3) % 3 == 1)
        {
            numOfUsers++;
        }
        actualLine++;
    }
    fclose(fp);
    return numOfUsers;
}

int SortingTextFile(char *fname)
{
    int numOfUsers = GetNumberOfUsers(fname);

    char userNameList[numOfUsers][30];
    char userPassList[numOfUsers][30];
    int userArrayAtPos1 = 0, userArrayAtPos2 = 0;
    char buffer[50];

    FILE *fPtr;
    FILE *fTemp;

    if ((fPtr = fopen(fname, "r")) == NULL)
    {
        printf("No file found!");
        return -1;
    }

    //Trazendo os usuarios do arquivo de texto para os vetores userNameList e userPassList
    //---------------------------------------------------------------------------------------------------------------------
    int nextLineToGet = 1, actualLine = 1;
    while (fgets(buffer, 512, fPtr) != NULL)
    {
        if ((actualLine + 3) % 3 == 1)
        {
            strcpy(userNameList[userArrayAtPos1], buffer);
            userArrayAtPos1++;
            nextLineToGet += 3;
        }

        else if ((actualLine + 3) % 3 == 2)
        {
            strcpy(userPassList[userArrayAtPos2], buffer);
            userArrayAtPos2++;
        }
        actualLine++;
    }

    //Ordenando os vetores de acordo com seus respectivos nomes de usuarios
    //----------------------------------------------------------------------------------------------------------------------
    char tmp[30];
    int j, i;
    for (i = 1; i < numOfUsers; i++)
    {
        j = i;
        while (j > 0 && strcmp(userNameList[j - 1], userNameList[j]) > 0)
        {
            //Mudando o usuario de lugar
            strcpy(tmp, userNameList[j]);
            strcpy(userNameList[j], userNameList[j - 1]);
            strcpy(userNameList[j - 1], tmp);

            //Mudando a senha de lugar
            strcpy(tmp, userPassList[j]);
            strcpy(userPassList[j], userPassList[j - 1]);
            strcpy(userPassList[j - 1], tmp);
            j--;
        }
    }
    fclose(fPtr);

    //Confeccionando o novo aquivo txt agora com a ordem certa
    //----------------------------------------------------------------------------------------------------------------------

    fTemp = fopen("replace.txt", "w");

    for (int k = 0; k < numOfUsers; k++)
    {
        fputs(userNameList[k], fTemp);
        fputs(userPassList[k], fTemp);
        fputs("\n", fTemp);
    }

    fclose(fTemp);

    remove(fname);
    rename("replace.txt", fname);

    return 0;
}

void ReplaceLineWith(int line, char *fName, char *textToReplace)
{

    FILE *fPtr;
    FILE *fTemp;

    char jumpLine[2] = "\n";
    char buffer[50];
    char newline[50];
    int count;

    //Criando uma cópia do texto pra adicionar um "jumpline" sem alterar a string original
    char *newLineCopy = (char *)malloc(sizeof(char) * strlen(textToReplace) + 1);
    strcpy(newLineCopy, textToReplace);
    strcat(newLineCopy, jumpLine);
    strcpy(newline, newLineCopy);

    fPtr = fopen(fName, "r");
    fTemp = fopen("replace.txt", "w");

    count = 0;
    while ((fgets(buffer, 50, fPtr)) != NULL)
    {

        count++;

        //se a linha atual for a linha que tem que substituir
        if (count == line)
            fputs(newLineCopy, fTemp);
        else
            fputs(buffer, fTemp);
    }
    fclose(fPtr);
    fclose(fTemp);

    free(newLineCopy);
    remove(fName);
    rename("replace.txt", fName);
}

int CheckForUserPassword(char *fname, char *str, char *consoleMessage, int *line)
{

    char jumpLine[2] = "\n";
    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];
    char password[50];
    int userFound = 0;
    strcat(str, jumpLine);

    if ((fp = fopen(fname, "r")) == NULL)
    {
        return -1;
    }

    //Pesquisa o usuario
    while (fgets(temp, 512, fp) != NULL)
    {

        if (strcmp(str, temp) == 0)
        {
            system("cls");
            *line = line_num;
            printf("Usuario encontrado\nSenha: ");

            scanf("%s", password);
            userFound = 1;
            break;
        }
        line_num++;
    }
    if (userFound == 0)
    {
        system("cls");
        printf("User not found!");
        return 1;
    }

    fclose(fp);
    fp = fopen(fname, "r");


    //Dá fgets até chegar na linha da senha do usuario
    for (int i = 0; i < line_num + 1; i++)
    {
        fgets(temp, 512, fp);
    }

    //Criptografa a senha 
    encrypt(password, strlen(password));
    strcat(password, jumpLine);

    fclose(fp);

    //Checa se a senha criptografada é a correta
    if ((strcmp(password, temp)) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int Search_in_File(char *fname, char *str)
{
    char jumpLine[2] = "\n";
    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];

    if ((fp = fopen(fname, "r")) == NULL)
    {
        printf("No file found!");
        return -1;
    }

    //Cria uma copia para que o jumpline nao mude a string original parea checagem
    char *strCopy = (char *)malloc(sizeof(char) * strlen(str) + 1);
    strcpy(strCopy, str);

    strcat(strCopy, jumpLine);

    //Procura a String no arquivo
    while (fgets(temp, 512, fp) != NULL)
    {
        if ((strcmp(temp, strCopy)) == NULL)
        {
            find_result++;
        }
        line_num++;
    }

    
    if (find_result == 0)
    {
        fclose(fp);
        return -1;
    }
    free(strCopy);
    fclose(fp);
    return 0;
}

int PrintInANewLine(char *name, char *str)
{
    FILE *fp;
    fp = fopen(name, "a+");
    fputs(str, fp);
    fclose(fp);
}

int main()
{
    system("cls");
    char consoleMessage[40] = "";
    char state[20] = "logging";
    int lookingAtLine = 0;
    while (1)
    {

        int option = 0;
        char userName[100];
        char jumpLine[2] = "\n";
        char toEncrypt[200];
        printf("%s\n\n", consoleMessage);

        if (strstr(state, "logging"))
        {

            printf("Deseja fazer login ou cadastrar-se?\nLogin (1)\nCadastro (2)\n");
            do
            {
                scanf("%d", &option);

                if (option != 1 && option != 2)
                {
                    system("cls");
                    printf("Opcao invalida\n\n");
                    printf("Deseja fazer login ou cadastrar-se?\nLogin (1)\nCadastro (2)\n");
                }

                while (getchar() != '\n')
                {
                    system("cls");
                    printf("Opcao invalida\n\n");
                    printf("Deseja fazer login ou cadastrar-se?\nLogin (1)\nCadastro (2)\n");
                }
            } while (option != 1 && option != 2);

            switch (option)
            {
            case 1:

                system("cls");
                printf("Digite o nome de usuario: ");
                scanf("%s", userName);
                if (CheckForUserPassword("Usuarios", userName, consoleMessage, &lookingAtLine) == 0)
                {
                    strcpy(consoleMessage, "Logado com sucesso");
                    strcpy(state, "logged");
                    system("cls");
                }
                else
                {
                    strcpy(consoleMessage, "Usuario ou senha incorretos");
                    system("cls");
                }

                break;

            case 2:
                system("cls");
                printf("Cadastro\n");
                printf("Digite o nome de usuario: ");
                scanf(" %[^\n]", userName);

                if (StringHasSpace(userName))
                {
                    strcpy(consoleMessage, "Usuario nao pode ter espaco");
                    system("cls");
                    continue;
                }
                if (Search_in_File("Usuarios", userName) == 0)
                {
                    strcpy(consoleMessage, "Usuario ja existe");

                    system("cls");
                    continue;
                }
                printf("Digite sua senha: ");
                scanf(" %[^\n]", toEncrypt);
                if (StringHasSpace(toEncrypt))
                {
                    strcpy(consoleMessage, "Senha nao pode ter espaco");
                    system("cls");
                    continue;
                }

                encrypt(toEncrypt, strlen(toEncrypt));

                strcat(toEncrypt, jumpLine);
                strcat(userName, jumpLine);
                PrintInANewLine("Usuarios", userName);
                PrintInANewLine("Usuarios", toEncrypt);
                PrintInANewLine("Usuarios", jumpLine);

                SortingTextFile("Usuarios");
                strcpy(consoleMessage, "Cadastro concluido!");
                system("cls");
                break;
            }
        }
        else if (strstr(state, "logged"))
        {
            printf("Usuario: %s\n", userName);
            printf("Mudar senha (1)\nMudar nome de usuario (2)\nDeslogar (3)\nDeletar usuario (4)\n");
            do
            {
                scanf("%d", &option);

                if (option != 1 && option != 2 && option != 3)
                {
                    system("cls");
                    printf("Opcao invalida\n\n");
                    printf("Mudar senha (1)\nMudar nome de usuario (2)\nDeslogar (3)\nDeletar usuario (4)\n");
                }

                while (getchar() != '\n')
                {
                    system("cls");
                    printf("Opcao invalida\n\n");
                    printf("Mudar senha (1)\nMudar nome de usuario (2)\nDeslogar (3)\nDeletar usuario (4)\n");
                }
            } while (option != 1 && option != 2 && option != 3 && option != 4);

            switch (option)
            {
            case 1:
                system("cls");
                printf("Qual sera sua nova senha?\n");
                char newPass[50];
                scanf("%s", newPass);
                encrypt(newPass, strlen(newPass));
                ReplaceLineWith(lookingAtLine + 1, "Usuarios", newPass);
                strcpy(consoleMessage, "Senha alterada com sucesso!");
                system("cls");
                break;
            case 2:
                system("cls");
                printf("Qual sera sua novo nome de usuario?\n");
                char newUser[50];
                scanf("%s", newUser);
                if (Search_in_File("Usuarios", newUser) == 0)
                {
                    strcpy(consoleMessage, "Usuario ja existe");

                    system("cls");
                    continue;
                }

                ReplaceLineWith(lookingAtLine, "Usuarios", newUser);
                strcpy(userName, newUser);
                strcat(userName, jumpLine);
                strcpy(consoleMessage, "Nome de usuario alterado com sucesso");
                system("cls");
                break;
            case 3:
                lookingAtLine = 0;
                strcpy(state, "logging");
                strcpy(consoleMessage, "Deslogado com sucesso");
                system("cls");
                break;
            case 4:
                DeleteUser("Usuarios", lookingAtLine);
                lookingAtLine = 0;
                strcpy(state, "logging");
                strcpy(consoleMessage, "Usuario deletado com sucesso");
                system("cls");
                break;
            }
        }
    }

    return 0;
}
