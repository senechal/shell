	Neste trabalho foi implementada uma mini-shell semelhante à encontrada em distrituições Unix-like.
	O trabalho consiste basicamente em 3 bibliotecas principais: "jobs.h", "path.h" e "shell.h". As outras bibliotecas servem de auxílio à operações com strings e listas de strings.
	Foram implementadas algumas de suas funções mais utilizadas, tais como execução simples de comandos em primeiro plano (ex: xeyes, gedit, jobs), execução de mais de um programa (em segundo plano, ex: xeyes&, gedit&). Os comandos fg <número> e bg <número> estão implementados dessa maneira, ou seja, basta digitar bg 1 para levar o processo de ID 1 para background. As rotinas que tratam da manipulação de jobs estão implementadas na biblioteca "jobs.h", entretanto as rotinas que tratam do envio de sinais (SIGINT, SIGSTP e SIGCHLD) e tratam de execução em primeiro e segundo planos estão implementadas na biblioteca "shell.h".
	Os comandos integrados também estão implementados nesta biblioteca ("shell.h"), mais especificamente na rotina "int execute_int_commands (StringList arguments)", onde estão implementados os comandos "cd", "pwd", entre outros.
	Para o problema de redirecionamento de E/S de programas foi utiliza uma função específica, "int verify_redirect (StringList *arguments)". Esta verifica se é necessário realizar redirecionamento de E/S e retorna um inteiro indicando a ação realizada: 0 - não fez nada; 1 - redirecionou stdout (sem append); 2 - redirecionou stdout (com append); 3 - redirecionou stderr; 4 - redirecionou stdin.

Problemas:
	A implementação dos pipes não foi feita.
	Na parte de comandos integrados alguns comandos estão com problema.

Teste:
	Para teste foram utilizados comandos diversos em primeiro e segundo planos e, consequentemente, a verificação de suas saídas. Foi constatado que alguns casos de teste ainda estão com problemas desconhecidos.