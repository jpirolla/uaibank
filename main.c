#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

int id_global = 1;
int num_usuario = 0;

typedef struct Usuario {
    int ID;
    char Nome[101];
    int Idade;
    float Saldo;
} Usuario;

int encontrarUsuarioPorID(Usuario usuarios[], int idDesejado, int tamanhoLista) {
    if(idDesejado > 0)
    {
        for (int i = 0; i < tamanhoLista; i++) {
            if (usuarios[i].ID == idDesejado) {
                return i;
        }
    }
    }

    return -1; // Retorna -1 se o usuário não for encontrado
}

int nome_valido(const char *name) {
    while (*name) {
        if (isdigit(*name)) {
            return 0;
        }
        name++;
    }
    return 1;
}

int ad_usuario(Usuario **p) {
    int idade;
    char nome[101], input[300];
    float saldo;

    fgets(input, sizeof(input), stdin);
    if (sscanf(input, " %100[^,], %d, %f", nome, &idade, &saldo) == 3) {

        if (!nome_valido(nome)) {
            printf("Nome inválido: não deve conter dígitos\n");
            return 0;
        }

        if (saldo < 0.0) {
            printf("Saldo inválido: o saldo deve ser positivo ou 0\n");
            return 0;
        }

        if (idade > 125 || idade < 0){
            printf("Idade inválida\n");
            return 0;

        }

        *p = realloc(*p, (num_usuario + 1) * sizeof(Usuario));
        if (*p == NULL) {
            perror("Erro ao alocar memória");
            exit(1);
        }

        Usuario *novo_usuario = &(*p)[num_usuario];
        novo_usuario->ID = id_global++;
        strcpy(novo_usuario->Nome, nome);
        novo_usuario->Idade = idade;
        novo_usuario->Saldo = saldo;

        num_usuario++;

        return novo_usuario->ID;

    } else {
        printf("Erro ao ler os dados. Certifique-se de usar o formato correto: <string>,<int>,<float>\n");
        return 0;
    }
}

void salvarUsuarios(Usuario *usuarios, int tamanhoLista) {
    FILE *arquivo;
    arquivo = fopen("usuarios.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < tamanhoLista; i++) {
        fprintf(arquivo, "%d, %s, %d, %.2f\n", usuarios[i].ID, usuarios[i].Nome, usuarios[i].Idade, usuarios[i].Saldo);
    }

    fclose(arquivo);
}

void abrirArquivo(Usuario **p) {
    FILE *arquivo;
    arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Usuario temp;
    char linha[200];
    int result;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        result = sscanf(linha, "%d, %100[^,], %d, %f", &temp.ID, temp.Nome, &temp.Idade, &temp.Saldo);
        if (result == 4) {
            *p = realloc(*p, (num_usuario + 1) * sizeof(Usuario));
            if (*p == NULL) {
                printf("Erro de alocação de memória.\n");
                fclose(arquivo);
                return;
            }
            (*p)[num_usuario] = temp;
            num_usuario++;
            
        } else {
            printf("Erro ao ler a linha do arquivo: %s\n", linha);
        }
    }
    id_global = num_usuario+1;
   

    fclose(arquivo);
}

int main() {
    setlocale(LC_ALL,"Portuguese");
    setlocale(LC_NUMERIC, "C");

    Usuario *usuarios = NULL;
    abrirArquivo(&usuarios);

    int num, add_user, qnt_usuario, i, id_busca, id_origem, id_destino;
    char input[100];
    float quantia;
    
    printf("Escolha dentre as opções abaixa:\n\n1 - Inserção de um novo usuário\n2 - Inserção de vários usuários\n3 - Busca de um usuário por ID\n4 - Tranferência entre usuários\n5 - Remoção de um usuário por ID\n\nInsira qualquer outro número para sair\n");
    scanf("%d", &num);
    getchar();

    while (num <= 5 && num > 0) {
        switch (num) {
            case 1:
                printf("Digite os dados do usuario no formato: <string>,<int>,<float>\n");
                add_user = ad_usuario(&usuarios);
                if (add_user != 0)
                    printf("\nUsuário inserido com id %d.\n", add_user);
                printf("\n---------Operação finalizada---------\n");
                break;
            case 2:
                printf("Informe quantos usuários serão inseridos: ");
                scanf("%d", &qnt_usuario);
                getchar();
                
                for (i = 0; i < qnt_usuario; i++) {
                    printf("Digite os dados do %d° usuario no formato: <string>,<int>,<float>\n", i + 1);
                    add_user = ad_usuario(&usuarios);
                    if (add_user != 0){
                        printf("Usuário inserido com id %d.\n\n", add_user);
                    }
                }
                break;
            case 3:
                printf("Informe o ID para busca: ");
                scanf("%d", &id_busca);

                int usuarioEncontrado = encontrarUsuarioPorID(usuarios, id_busca, num_usuario);

                if (usuarioEncontrado != -1) {
                    printf("\n---------Resultado da busca---------\n");
                    printf("\nUsuário %d tem saldo de R$%.2f.\n", usuarios[usuarioEncontrado].ID, usuarios[usuarioEncontrado].Saldo);
                } else {
                    printf("\nERRO!! Usuário %d não encontrado.\n", id_busca);
                }
                printf("\n---------Operação finalizada---------\n");
                break;
            case 4:
                printf("Informe o id de origem, o id de destino e a quantia da tranferência (<int>,<int>,<float>):\n");
                fgets(input, sizeof(input), stdin);
                if (sscanf(input, " %d, %d, %f", &id_origem, &id_destino, &quantia) == 3) {
                    if (encontrarUsuarioPorID(usuarios, id_origem, num_usuario) != -1 && encontrarUsuarioPorID(usuarios, id_destino, num_usuario) != -1) {

                        int usuarioOrigem = encontrarUsuarioPorID(usuarios, id_origem, num_usuario);
                        int usuarioDestino = encontrarUsuarioPorID(usuarios, id_destino, num_usuario);

                        if (usuarios[usuarioOrigem].Saldo >= quantia && quantia > 0.0) {
                            usuarios[usuarioOrigem].Saldo -= quantia;
                            usuarios[usuarioDestino].Saldo += quantia;
                            printf("Transferência realizada com sucesso.\n");
                        } else {
                            printf("Saldo inválido.\n");
                        }
                    } else {
                        printf("ID inválido.\n");
                    }
                } else {
                    printf("Erro ao ler os dados. Certifique-se de usar o formato correto: <int>,<int>,<float>\n");
                }
                printf("\n---------Operação finalizada---------\n");
                break;
            case 5:
                printf("Informe o ID do usuário que deseja deletar:\n");
                scanf("%d", &id_busca);

                int usuarioIndex = encontrarUsuarioPorID(usuarios, id_busca, num_usuario);
                if (usuarioIndex != -1) {
                    usuarios[usuarioIndex].ID = -1;
                    strcpy(usuarios[usuarioIndex].Nome, "Null");
                    usuarios[usuarioIndex].Idade = 0;
                    usuarios[usuarioIndex].Saldo = 0.00;
                    printf("Usuário %d deletado com sucesso.\n", id_busca);
                } else {
                    printf("Erro: Usuário %d não encontrado.\n", id_busca);
                }
                printf("\n---------Operação finalizada---------\n");
                break;
            default:
                break;
        }
        printf("Escolha dentre as opções abaixa:\n\n1 - Inserção de um novo usuário\n2 - Inserção de vários usuários\n3 - Busca de um usuário por ID\n4 - Tranferência entre usuários\n5 - Remoção de um usuário por ID\n\nInsira qualquer outro número para sair\n");
        scanf("%d", &num);
        getchar();
    }

    salvarUsuarios(usuarios, num_usuario);

    free(usuarios);

    return 0;
}