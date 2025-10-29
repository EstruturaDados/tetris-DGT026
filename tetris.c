#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILA_MAX 5    // Capacidade da fila de peças futuras
#define PILHA_MAX 3   // Capacidade da pilha de reserva

// Estrutura que representa uma peça
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Fila circular
typedef struct {
    Peca fila[FILA_MAX];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Pilha linear
typedef struct {
    Peca pilha[PILHA_MAX];
    int topo;
} Pilha;

// Gera peça aleatória com ID único
Peca gerarPeca(int id) {
    Peca p;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// Inicializa a fila com FILA_MAX peças
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
    for(int i = 0; i < FILA_MAX; i++) {
        f->fila[f->tras] = gerarPeca(i);
        f->tras = (f->tras + 1) % FILA_MAX;
        f->tamanho++;
    }
}

// Inicializa pilha vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Exibe o estado atual da fila e da pilha
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");

    // Fila
    printf("Fila de peças: ");
    if(f->tamanho == 0) {
        printf("Vazia");
    } else {
        int count = f->frente;
        for(int i = 0; i < f->tamanho; i++) {
            printf("[%c %d] ", f->fila[count].nome, f->fila[count].id);
            count = (count + 1) % FILA_MAX;
        }
    }
    printf("\n");

    // Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if(p->topo == -1) {
        printf("Vazia");
    } else {
        for(int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pilha[i].nome, p->pilha[i].id);
        }
    }
    printf("\n");
}

// Remove peça da frente da fila (jogar)
Peca jogarPeca(Fila *f) {
    Peca p = f->fila[f->frente];
    f->frente = (f->frente + 1) % FILA_MAX;
    f->tamanho--;
    return p;
}

// Adiciona peça ao final da fila
void inserirFila(Fila *f, Peca p) {
    f->fila[f->tras] = p;
    f->tras = (f->tras + 1) % FILA_MAX;
    f->tamanho++;
}

// Adiciona peça à pilha de reserva
int empilhar(Pilha *p, Peca pc) {
    if(p->topo >= PILHA_MAX - 1) return 0; // Pilha cheia
    p->topo++;
    p->pilha[p->topo] = pc;
    return 1;
}

// Remove peça do topo da pilha (usar)
int desempilhar(Pilha *p, Peca *pc) {
    if(p->topo == -1) return 0; // Pilha vazia
    *pc = p->pilha[p->topo];
    p->topo--;
    return 1;
}

// Troca peça da frente da fila com topo da pilha
int trocarTopoFilaPilha(Fila *f, Pilha *p) {
    if(f->tamanho == 0 || p->topo == -1) return 0; // impossível trocar
    Peca temp = f->fila[f->frente];
    f->fila[f->frente] = p->pilha[p->topo];
    p->pilha[p->topo] = temp;
    return 1;
}

// Troca múltipla: 3 primeiros da fila com 3 da pilha
int trocaMultipla(Fila *f, Pilha *p) {
    if(f->tamanho < 3 || p->topo < 2) return 0; // mínimo de peças necessário

    for(int i = 0; i < 3; i++) {
        int indexFila = (f->frente + i) % FILA_MAX;
        Peca temp = f->fila[indexFila];
        f->fila[indexFila] = p->pilha[p->topo - i];
        p->pilha[p->topo - i] = temp;
    }
    return 1;
}

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int proximoID = FILA_MAX; // ID único para novas peças

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        mostrarEstado(&fila, &pilha);
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                if(fila.tamanho == 0) {
                    printf("Fila vazia! Não é possível jogar peça.\n");
                } else {
                    Peca jogada = jogarPeca(&fila);
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    inserirFila(&fila, gerarPeca(proximoID++));
                }
                break;
            }
            case 2: {
                if(fila.tamanho == 0) {
                    printf("Fila vazia! Não é possível reservar peça.\n");
                } else if(pilha.topo >= PILHA_MAX - 1) {
                    printf("Pilha cheia! Não é possível reservar peça.\n");
                } else {
                    Peca reservada = jogarPeca(&fila);
                    empilhar(&pilha, reservada);
                    printf("Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    inserirFila(&fila, gerarPeca(proximoID++));
                }
                break;
            }
            case 3: {
                Peca usada;
                if(desempilhar(&pilha, &usada)) {
                    printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("Pilha vazia! Não há peça para usar.\n");
                }
                break;
            }
            case 4: {
                if(trocarTopoFilaPilha(&fila, &pilha)) {
                    printf("Troca realizada entre a peça da frente da fila e o topo da pilha.\n");
                } else {
                    printf("Não é possível realizar a troca (fila ou pilha vazia).\n");
                }
                break;
            }
            case 5: {
                if(trocaMultipla(&fila, &pilha)) {
                    printf("Troca múltipla realizada entre os 3 primeiros da fila e 3 da pilha.\n");
                } else {
                    printf("Não é possível realizar a troca múltipla (mínimo de 3 peças em cada).\n");
                }
                break;
            }
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}
