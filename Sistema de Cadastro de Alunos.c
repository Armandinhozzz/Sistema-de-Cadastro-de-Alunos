#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50

// Estrutura do aluno
typedef struct {
    char nome[TAM_NOME];
    float nota1;
    float nota2;
} Aluno;

// Função para exibir o menu
void exibirMenu() {
    printf("\n=== MENU ===\n");
    printf("1. Cadastrar aluno\n");
    printf("2. Listar alunos cadastrados\n");
    printf("3. Remover aluno por nome\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// Função para salvar um aluno no arquivo
void salvarAluno(Aluno aluno) {
    FILE *arquivo = fopen("alunos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(arquivo, "%s,%.2f,%.2f\n", aluno.nome, aluno.nota1, aluno.nota2);
    fclose(arquivo);
    printf("Aluno salvo com sucesso!\n");
}

// Função para listar os alunos
void listarAlunos() {
    FILE *arquivo = fopen("alunos.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum aluno cadastrado ainda.\n");
        return;
    }

    char linha[100];
    printf("\n=== Lista de Alunos ===\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        Aluno aluno;
        sscanf(linha, "%[^,],%f,%f", aluno.nome, &aluno.nota1, &aluno.nota2);
        float media = (aluno.nota1 + aluno.nota2) / 2;

        printf("%s - Média: %.2f - ", aluno.nome, media);
        if (media >= 7.0)
            printf("Aprovado\n");
        else if (media >= 5.0)
            printf("Recuperação\n");
        else
            printf("Reprovado\n");
    }

    fclose(arquivo);
}

// Função para remover um aluno por nome
void removerAlunoPorNome(const char *nomeAlvo) {
    FILE *arquivo = fopen("alunos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    char linha[100];
    int removido = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        char nome[50];
        float n1, n2;

        sscanf(linha, "%[^,],%f,%f", nome, &n1, &n2);

        // Escreve no novo arquivo apenas se o nome for diferente
        if (strcmp(nome, nomeAlvo) != 0) {
            fputs(linha, temp);
        } else {
            removido = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o original pelo novo
    remove("alunos.txt");
    rename("temp.txt", "alunos.txt");

    if (removido)
        printf("Aluno removido com sucesso!\n");
    else
        printf("Aluno não encontrado.\n");
}

// Função principal
int main() {
    int opcao;
    Aluno aluno;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("\nNome do aluno: ");
                fgets(aluno.nome, TAM_NOME, stdin);
                aluno.nome[strcspn(aluno.nome, "\n")] = '\0';

                printf("Nota 1: ");
                scanf("%f", &aluno.nota1);
                printf("Nota 2: ");
                scanf("%f", &aluno.nota2);

                salvarAluno(aluno);
                break;

            case 2:
                listarAlunos();
                break;

            case 3: {
                char nomeRemover[TAM_NOME];
                printf("Nome do aluno a remover: ");
                fgets(nomeRemover, TAM_NOME, stdin);
                nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

                removerAlunoPorNome(nomeRemover);
                break;
            }

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
