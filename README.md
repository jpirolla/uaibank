# Sistema de Gestão de Usuários do Uaibank


O projeto foi desenvolvido na disciplina de Introdução à Ciência da Computação I, cujo objetivo era criar um sistema de gestão dos usuários do Uaibank, em C, utilizando alocação dinâmica de memória para armazenar as informações dos usuários e passá-los para um arquivo externo. O sistema permite as seguintes operações:

### Inserção de um novo usuário:
```
1
<nome>, <idade>, <saldo atual>
```

### Inserção de vários usuários:
```
2
<quantidade de usuários>
<nome>, <idade>, <saldo atual> (para cada usuário)
```

### Busca de um usuário por ID:
```
3
<id>
```

### Transferências entre usuários:
```
4
<id de origem> <id de destino> <quantia>
```
### Remoção de um usuário por ID:
```
5
<id>
```
## Considerações
- Cada usuário é formado por ID, nome, idade e saldo atual.
- A interface do sistema é baseada em linha de comando, permitindo interações conforme descrito acima.
- Existem melhorias referente à criação de variáveis criadas no escopo global. As modificações serão realizadas quando possível. 





