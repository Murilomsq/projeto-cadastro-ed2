# Projeto de cadastro com criptografia ED2

CRUD simples onde cadastros são manipulados e armazenados em um arquivo txt, a senha é criptografada (operador XOR) pra ser armazenada, dessa forma
se acontece algum vazamento de banco de dados, existe uma barreira (mesmo que fraca, nesse caso) pra quem tem acesso ao arquivo, pois ele não armazena a senha
da forma que o usuario fornece.

-----------------------------------------------------------------------------------------------------------------------------------------------------
## Funcionalidades:
- Logar e Cadastrar
- Alterar Senha do Usuário
- Alterar Nome do Usuário
-  Deletar Cadastro
    
-----------------------------------------------------------------------------------------------------------------------------------------------------
## Tecnologias:

- Linguagem C
- Arquivos .txt em C
- Hash feito com operador binário XOR
- Senhas armazenadas criptografadas
- Ordenação do arquivo txt com Insertion sort
- Busca sequencial nos arquivos txt


-----------------------------------------------------------------------------------------------------------------------------------------------------
## Buildando o projeto:

### Windows 
- No terminal no diretório do arquivo ED2.c pela primeira vez, execute:

 ```
 gcc ED2.c -o ED2 & ED2
 ```

- Após compilado pela primeira vez é só executar o arquivo ED2.exe. Ou, no terminal no diretório do arquivo, executar:
 ```
 ED2
 ```

### Linux 
- Não é possivel compilar pois uma biblioteca "conio.h", feita para MS-DOS, foi utilizada.

-----------------------------------------------------------------------------------------------------------------------------------------------------
## Observações:
A maioria dos inputs estão tratados, ou seja, o programa dirá quando algum input não vier da forma que lhe é esperado;
O único input não tratado é o de numero de caracteres, então tente sempre utilizar nome e senha com menos de 20 caracteres

Algumas senhas numericas quando criptografadas se traduzem nos caracteres \n bagunçando assim, a ordem do arquivo de texto. 
Se o programa não estiver funcionando corretamente, essa é possívelmente a causa.
Caso isso aconteça delete o arquivo de texto e cadastre novos usuários pra resetar o arquivo txt.

### Para evitar esse tipo de bug sugerimos:
   - Não utilizar algarismos maiores que 7 pra criar a senha
   - Misturar algarismos com letras
