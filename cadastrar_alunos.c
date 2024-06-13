//  Este projeto consiste em um sistema de cadastro de alunos onde é possível adicionar, listar, buscar e remover alunos. 
//  As informações dos alunos são armazenadas em um arquivo para que os dados persistam entre as execuções do programa.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO_ALUNOS "alunos.dat"

typedef struct {
    int id;
    char nome[50];
    int idade;
    char curso[50];
} Aluno;

void menu();
void adicionarAluno();
void listarAlunos();
void buscarAluno();
void removerAluno();
void salvarAlunosEmTxt(const char *cadastro_de_alunos);
void lerArquivoTxt(const char *alunos);
int obterNovoID();

// A função principal é responsável por exibir o menu, capturar a escolha do usuário e chamar as funções apropriadas com base nessa escolha.

int main() {

    setlocale(LC_ALL, "Portuguese_Brazil");

    int opcao;

    // Garantir que o arquivo existe ao iniciar o programa

    FILE *pont_arq;
    pont_arq = fopen(ARQUIVO_ALUNOS, "ab");
    if (pont_arq == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return 1;
    }
    fclose(pont_arq);
    
    do {
        menu();
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1:
                adicionarAluno();
                break;
            case 2:
                listarAlunos();
                break;
            case 3:
                buscarAluno();
                break;
            case 4:
                removerAluno();
                break;
            case 5:
                salvarAlunosEmTxt("cadastro_de_alunos.txt");
                printf("Alunos salvos em: cadastro de alunos\n");
                break;
            case 6:
                lerArquivoTxt("alunos.txt");
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 7);

    return 0;
}

// A função menu() é responsável por exibir um menu de opções para o usuário no console.

void menu() {
    printf("\n--- Menu ---\n");
    printf("1. Adicionar novo aluno.\n");
    printf("2. Listar todos os novos alunos.\n");
    printf("3. Buscar aluno.\n");
    printf("4. Remover aluno.\n");
    printf("5. Salvar cadastro de novos alunos.\n");
    printf("6. Acessar alunos já cadastrados.\n");
    printf("7. Sair.\n");
}

// A função "adicionarAluno()" tem o propósito de adicionar um novo registro de um aluno em um arquivo.

void adicionarAluno() {
    Aluno aluno;
    FILE *file = fopen(ARQUIVO_ALUNOS, "ab");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    aluno.id = obterNovoID();
    printf("Nome: ");
    fgets(aluno.nome, sizeof(aluno.nome), stdin);
    aluno.nome[strcspn(aluno.nome, "\n")] = '\0'; // Remove o newline
    printf("Idade: ");
    scanf("%d", &aluno.idade);
    getchar(); // Limpa o buffer do teclado
    printf("Curso: ");
    fgets(aluno.curso, sizeof(aluno.curso), stdin);
    aluno.curso[strcspn(aluno.curso, "\n")] = '\0'; // Remove o newline

    fwrite(&aluno, sizeof(Aluno), 1, file);
    fclose(file);

    printf("Aluno adicionado com sucesso!\n");
}

void listarAlunos() {
    Aluno aluno;
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("\n--- Lista de Alunos ---\n");

    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        printf("ID: %d\n", aluno.id);
        printf("Nome: %s\n", aluno.nome);
        printf("Idade: %d\n", aluno.idade);
        printf("Curso: %s\n", aluno.curso);
        printf("----------------------\n");
    }

    fclose(file);
}

// A função "buscarAluno()" tem o objetivo de procurar um aluno específico em um arquivo e exibir suas informações se ele for encontrado.

void buscarAluno() {
    int id;
    Aluno aluno;
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Digite o ID do aluno: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer do teclado

    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id == id) {
            printf("\nAluno encontrado:\n");
            printf("ID: %d\n", aluno.id);
            printf("Nome: %s\n", aluno.nome);
            printf("Idade: %d\n", aluno.idade);
            printf("Curso: %s\n", aluno.curso);
            fclose(file);
            return;
        }
    }

    printf("Aluno não encontrado!\n");
    fclose(file);
}

// A função "removerAluno()" tem a finalidade de remover um aluno específico de um arquivo que contém registros de alunos.

void removerAluno() {
    int id;
    Aluno aluno;
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    FILE *tempFile = fopen("temp.dat", "wb");

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Digite o ID do aluno a ser removido: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer do teclado

    int encontrado = 0;

    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id == id) {
            encontrado = 1;
        } else {
            fwrite(&aluno, sizeof(Aluno), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(ARQUIVO_ALUNOS);
    rename("temp.dat", ARQUIVO_ALUNOS);

    if (encontrado) {
        printf("Aluno removido com sucesso!\n");
    } else {
        printf("Aluno não encontrado!\n");
    }
}

// A função "salvarAlunoEmTxt() tem a finalidade de salvar a lista de alunos cadastrados em um arquivo txt."

void salvarAlunosEmTxt(const char *cadastro_de_alunos) {
    Aluno aluno;
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    FILE *txtFile = fopen(cadastro_de_alunos, "w");

    if (file == NULL || txtFile == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        fprintf(txtFile, "ID: %d\nNome: %s\nIdade: %d\nCurso: %s\n\n", aluno.id, aluno.nome, aluno.idade, aluno.curso);
    }

    fclose(file);
    fclose(txtFile);
}

// A função "lerArquivoTxt()" tem o objetivo de ler um arquivo txt e exibir seu conteúdo no console.

void lerArquivoTxt(const char *alunos) {
    FILE *file = fopen(alunos, "r");
    char linha[256];

    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("\n------ Alunos já cadastrados ------\n");
    printf("\n");

    while (fgets(linha, sizeof(linha), file)) {
        printf("%s", linha);
    }

    printf("\n");
    printf("--------------------------------\n");
    fclose(file);
}

// A função "obterNovoID()" tem como objetivo gerar um novo identificador único (ID) para um aluno 
// ao verificar o maior ID já existente no arquivo de alunos e incrementá-lo em 1.

int obterNovoID() {
    Aluno aluno;
    FILE *file = fopen(ARQUIVO_ALUNOS, "rb");
    int id = 1;

    if (file == NULL) {
        return id;
    }

    while (fread(&aluno, sizeof(Aluno), 1, file)) {
        if (aluno.id >= id) {
            id = aluno.id + 1;
        }
    }

    fclose(file);
    return id;
}
