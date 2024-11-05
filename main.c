#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Fruta {
    int codigo; char nome[30]; int quantidade; float preco; struct Fruta *proxima;
} Fruta;

void cadastrarFruta(Fruta **inicio); void listarFrutas(Fruta *inicio); Fruta* buscarFruta(Fruta *inicio, int codigo);
void alterarFruta(Fruta *inicio); void excluirFruta(Fruta **inicio); void venderFruta(Fruta *inicio); void registrarVenda(int codigo, char *nome, int quantidade, float preco);

int main() {
    Fruta *inicio = NULL; int opcao;

    do {
        printf ("\n  MERCADINHO COMPRE 2, LEVE 1 \n");
        printf("\n►  MENU: \n"); 
        printf("► 1. Cadastrar fruta \n"); 
        printf("► 2. Listar frutas \n");
        printf("► 3. Buscar fruta \n"); 
        printf("► 4. Alterar fruta \n"); 
        printf("► 5. Excluir fruta \n");
        printf("► 6. Vender fruta \n"); 
        printf("► 7. Sair \n"); printf("► Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarFruta(&inicio); break; 
            case 2: listarFrutas(inicio); break; 
            case 3: {
                int codigo; printf("► Digite o codigo da fruta: "); scanf("%d", &codigo);
                Fruta *fruta = buscarFruta(inicio, codigo); if (fruta) {
                    printf("► Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n",
                           fruta->codigo, fruta->nome, fruta->quantidade, fruta->preco);
                } else { printf("► Fruta nao encontrada.\n"); } break;
            }
            case 4: alterarFruta(inicio); break; 
            case 5: excluirFruta(&inicio); break; 
            case 6: venderFruta(inicio); break; 
            case 7: printf("► Saindo...\n"); break; 
            default: printf("► Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 7);

    while (inicio != NULL) {
        Fruta *temp = inicio; inicio = inicio->proxima; free(temp);
    }

    return 0;
}

void cadastrarFruta(Fruta **inicio) {
    Fruta *nova = (Fruta *)malloc(sizeof(Fruta));
    printf("► Digite o codigo da fruta: "); scanf("%d", &nova->codigo);
    Fruta *atual = *inicio; while (atual != NULL) {
        if (atual->codigo == nova->codigo) {
            printf("► Erro: Codigo ja existe!\n"); free(nova); return;
        } atual = atual->proxima;
    }

    printf("► Digite o nome da fruta: "); scanf("%s", nova->nome);
    printf("► Digite a quantidade em estoque: "); scanf("%d", &nova->quantidade);
    printf("► Digite o preco da fruta: "); scanf("%f", &nova->preco);
    nova->proxima = *inicio; *inicio = nova;

    printf("► Fruta cadastrada com sucesso!\n");
}

void listarFrutas(Fruta *inicio) {
    if (inicio == NULL) { printf("► Nenhuma fruta cadastrada.\n"); return; }
    Fruta *atual = inicio; printf("\n► Lista de Frutas:\n");
    while (atual != NULL) {
        printf("► Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n",
               atual->codigo, atual->nome, atual->quantidade, atual->preco);
        atual = atual->proxima;
    }
}

Fruta* buscarFruta(Fruta *inicio, int codigo) {
    Fruta *atual = inicio; while (atual != NULL) {
        if (atual->codigo == codigo) { return atual; }
        atual = atual->proxima;
    } return NULL;
}

void alterarFruta(Fruta *inicio) {
    int codigo; printf("► Digite o codigo da fruta a ser alterada: "); scanf("%d", &codigo);
    Fruta *fruta = buscarFruta(inicio, codigo); if (fruta == NULL) {
        printf("► Erro: Fruta nao encontrada.\n"); return;
    }
    printf("► Digite o novo nome da fruta: "); scanf("%s", fruta->nome);
    printf("► Digite a nova quantidade: "); scanf("%d", &fruta->quantidade);
    printf("► Digite o novo preco: "); scanf("%f", &fruta->preco);
    printf("Fruta alterada com sucesso!\n");
}

void excluirFruta(Fruta **inicio) {
    int codigo; printf("► Digite o codigo da fruta a ser excluida: "); scanf("%d", &codigo);
    Fruta *atual = *inicio, *anterior = NULL; while (atual != NULL && atual->codigo != codigo) {
        anterior = atual; atual = atual->proxima;
    }

    if (atual == NULL) { printf("► Erro: Fruta nao encontrada.\n"); return; }
    if (atual->quantidade > 0) { printf("► Erro: Nao e possivel excluir frutas com estoque.\n"); return; }

    if (anterior == NULL) { *inicio = atual->proxima; } else { anterior->proxima = atual->proxima; }

    free(atual); printf("► Fruta excluida com sucesso!\n");
}

void venderFruta(Fruta *inicio) {
    int codigo, quantidade; printf("► Digite o codigo da fruta: "); scanf("%d", &codigo);
    printf("► Digite a quantidade a ser vendida: "); scanf("%d", &quantidade);

    Fruta *fruta = buscarFruta(inicio, codigo); if (fruta == NULL) {
        printf("► Erro: Fruta nao encontrada.\n"); return;
    }

    if (fruta->quantidade < quantidade) { printf("► Erro: Quantidade insuficiente no estoque.\n"); return; }

    fruta->quantidade -= quantidade; registrarVenda(fruta->codigo, fruta->nome, quantidade, fruta->preco);
    printf("► Venda realizada com sucesso!\n");
}

void registrarVenda(int codigo, char *nome, int quantidade, float preco) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (arquivo == NULL) { printf("► Erro ao abrir o arquivo de vendas.\n"); return; }
    fprintf(arquivo, "► Codigo: %d, Nome: %s, Quantidade: %d, Preco unitario: %.2f, Total: %.2f\n",
            codigo, nome, quantidade, preco, quantidade * preco);
    fclose(arquivo);
}
