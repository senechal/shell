	Neste trabalho foi implementada uma mini-shell semelhante � encontrada em distritui��es Unix-like.
	O trabalho consiste basicamente em 3 bibliotecas principais: "jobs.h", "path.h" e "shell.h". As outras bibliotecas servem de aux�lio � opera��es com strings e listas de strings.
	Foram implementadas algumas de suas fun��es mais utilizadas, tais como execu��o simples de comandos em primeiro plano (ex: xeyes, gedit, jobs), execu��o de mais de um programa (em segundo plano, ex: xeyes&, gedit&). Os comandos fg <n�mero> e bg <n�mero> est�o implementados dessa maneira, ou seja, basta digitar bg 1 para levar o processo de ID 1 para background. As rotinas que tratam da manipula��o de jobs est�o implementadas na biblioteca "jobs.h", entretanto as rotinas que tratam do envio de sinais (SIGINT, SIGSTP e SIGCHLD) e tratam de execu��o em primeiro e segundo planos est�o implementadas na biblioteca "shell.h".
	Os comandos integrados tamb�m est�o implementados nesta biblioteca ("shell.h"), mais especificamente na rotina "int execute_int_commands (StringList arguments)", onde est�o implementados os comandos "cd", "pwd", entre outros.
	Para o problema de redirecionamento de E/S de programas foi utiliza uma fun��o espec�fica, "int verify_redirect (StringList *arguments)". Esta verifica se � necess�rio realizar redirecionamento de E/S e retorna um inteiro indicando a a��o realizada: 0 - n�o fez nada; 1 - redirecionou stdout (sem append); 2 - redirecionou stdout (com append); 3 - redirecionou stderr; 4 - redirecionou stdin.

Problemas:
	A implementa��o dos pipes n�o foi feita.
	Na parte de comandos integrados alguns comandos est�o com problema.

Teste:
	Para teste foram utilizados comandos diversos em primeiro e segundo planos e, consequentemente, a verifica��o de suas sa�das. Foi constatado que alguns casos de teste ainda est�o com problemas desconhecidos.