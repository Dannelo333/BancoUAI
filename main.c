#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct cadastro {
    int id;
    char nome[100];
    char snome[100];
    int idade;
    double saldo;
};

FILE *arquivo1;
FILE *arquivo2;

struct cadastro *Usuarios;
int totalUsuarios = 0;

void carregararquivo() {
    arquivo1 = fopen("banco_de_dados.txt", "r");
    if (arquivo1 == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    int id, idade;
    double saldo;
    char nome[100], snome[100];

    totalUsuarios = 0;
    Usuarios = realloc(Usuarios, sizeof(struct cadastro));

    while (fscanf(arquivo1, "%d %s %s %d %lf", &id, nome, snome, &idade, &saldo) == 5) {
        Usuarios = realloc(Usuarios, (totalUsuarios + 1) * sizeof(struct cadastro));
        if (Usuarios == NULL) {
            printf("Erro ao realocar memória\n");
            fclose(arquivo1);
            return;
        }
        Usuarios[totalUsuarios].id = id;
        strcpy(Usuarios[totalUsuarios].nome, nome);
        strcpy(Usuarios[totalUsuarios].snome, snome);
        Usuarios[totalUsuarios].idade = idade;
        Usuarios[totalUsuarios].saldo = saldo;

        totalUsuarios++;
    }

    fclose(arquivo1);
}

int obterUltimoID() {
    int ultimoID = 0;

    for (int i = 0; i < totalUsuarios; i++) {
        if (Usuarios[i].id > ultimoID) {
            ultimoID = Usuarios[i].id;
        }
    }

    arquivo1 = fopen("banco_de_dados.txt", "r");
    if (arquivo1 != NULL) {
        int ID, idade;
        char nome[100], snome[100];
        double saldo;
        while (fscanf(arquivo1, "%d %s %s %d %lf", &ID, nome, snome, &idade, &saldo) == 5) {
            if (ID > ultimoID) {
                ultimoID = ID;
            }
        }
        fclose(arquivo1);
    }

    return ultimoID;
}


void adcUsuario() {
    double n;
	carregararquivo();
	int ultimoID = obterUltimoID();
	Usuarios = realloc(Usuarios, (totalUsuarios + 1) * sizeof(struct cadastro));

	if (Usuarios == NULL) {
    	printf("Erro ao realocar memória\n");
    	return;
	}

	printf("\nDigite o primeiro nome:\n");
	setbuf(stdin, NULL);
	fgets(Usuarios[totalUsuarios].nome, 100, stdin);
	Usuarios[totalUsuarios].nome[strcspn(Usuarios[totalUsuarios].nome, "\n")] = '\0';

	printf("Digite o sobrenome:\n");
	setbuf(stdin, NULL);
	fgets(Usuarios[totalUsuarios].snome, 100, stdin);
	Usuarios[totalUsuarios].snome[strcspn(Usuarios[totalUsuarios].snome, "\n")] = '\0';


	printf("Digite a idade:\n");
	char idadeStr[10];
	fgets(idadeStr, 10, stdin);
	sscanf(idadeStr, "%d", &Usuarios[totalUsuarios].idade);

    char saldoStr[20];

    printf("Digite o saldo:\n");
	scanf("%lf", &n);
	if(n >= 0){
   	Usuarios[totalUsuarios].saldo = n;
    	printf("\n>Cadastro de usuário completado com sucesso!\n");
	}
	else{
    	printf("\n>Cadastro incompleto, saldo não pode ser negativo\n");
    	return;
	}

	Usuarios[totalUsuarios].id = ultimoID + 1;

	arquivo1 = fopen("banco_de_dados.txt", "a");
	if (arquivo1 == NULL) {
    	printf("Erro ao abrir o arquivo\n");
    	return;
	}

	fprintf(arquivo1, "%d %s %s %d %.2lf\n", Usuarios[totalUsuarios].id, Usuarios[totalUsuarios].nome, Usuarios[totalUsuarios].snome, Usuarios[totalUsuarios].idade, Usuarios[totalUsuarios].saldo);

	fclose(arquivo1);

	totalUsuarios++;
}


void adcUsuarios() {
    int quant, i;
    printf("Quantos usuários deseja adicionar: ");
    scanf("%d", &quant);

    for (i = 0; i < quant; i++) {
        adcUsuario();
    }
}

void buscarID() {
    int ID, i;
    char opcao;
    carregararquivo();
    obterUltimoID();

    do {
        printf("Digite qual ID de usuário deseja buscar: ");
        scanf("%d", &ID);

        for (i = 0; i < totalUsuarios; i++) {
            if (Usuarios[i].id == ID) {
                printf(">O usuário de ID = %d apresenta esses dados:\n", ID);
                printf(">%s %s %d %.2lf\n\n\n", Usuarios[i].nome, Usuarios[i].snome, Usuarios[i].idade, Usuarios[i].saldo);
                return;
            }
        }

        printf("\n\n>Esse usuário não existe em nossa base de dados.\n");

        printf("\n\nDeseja buscar outro ID ?\nResponde com 's' para continuar e 'n' para sair\n");
        scanf(" %c", &opcao);
    } while (opcao == 's' || opcao == 'S');
}

void transferencia() {
    carregararquivo();
    int IDorigem, IDalvo, i, j;
    double ValorTrans;
    char resposta;

    do {
        printf("Digite o ID de origem da transferência: ");
        scanf("%d", &IDorigem);

        for (i = 0; i < totalUsuarios; i++) {
            if (Usuarios[i].id == IDorigem) {
                printf("\nUsuário de ID %d encontrado\n", IDorigem);
                break;
            }
        }

        if (i == totalUsuarios) {
            printf("Usuário não pertence à nossa base de dados. Deseja buscar outro ID origem? (s/n)\n");
            scanf(" %c", &resposta);

            if (resposta == 'n' || resposta == 'N') {
                printf(">Transferência cancelada por não inserir um ID.\n\n\n");
                return;
            }

        } else {
            resposta = 'n';
        }
    } while (resposta == 's' || resposta == 'S');

    do {
        printf("Digite o ID alvo da transferência: ");
        scanf("%d", &IDalvo);

        for (j = 0; j < totalUsuarios; j++) {
            if (Usuarios[j].id == IDalvo) {
                printf("\nUsuário de ID %d encontrado\n", IDalvo);
                break;
            }
        }

        if (j == totalUsuarios) {
            printf("Usuário não pertence a nossa base de dados. Deseja buscar outro ID alvo? (s/n)\n");
            scanf(" %c", &resposta);

            if (resposta == 'n' || resposta == 'N') {
                printf(">Transferência cancelada por não inserir um ID.\n\n\n");
                return;
            }

        } else {
            resposta = 'n';
        }
    } while (resposta == 's' || resposta == 'S');

    do {
        printf("Digite o valor da transferência: ");
        scanf("%lf", &ValorTrans);

        if (ValorTrans > Usuarios[i].saldo) {
            printf("Saldo insuficiente para realizar a transferência. Deseja inserir outro valor? (s/n)\n");
            scanf(" %c", &resposta);

            if (resposta == 'n' || resposta == 'N') {
                printf(">Transferência cancelada devido à falta de saldo.\n\n\n");
                return;
            }

        } else {
            resposta = 'n';
        }
    } while (resposta == 's' || resposta == 'S');

    arquivo2 = fopen("Transacoes.txt", "a");
    if (arquivo2 == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    fprintf(arquivo2, "%d transferiu %.2lf para %d\n", IDorigem, ValorTrans, IDalvo);

    fclose(arquivo2);

    for (int k = 0; k < totalUsuarios; k++) {
        if (Usuarios[k].id == IDorigem) {
            Usuarios[k].saldo -= ValorTrans;
        }
        if (Usuarios[k].id == IDalvo) {
            Usuarios[k].saldo += ValorTrans;
        }
    }

    arquivo1 = fopen("banco_de_dados.txt", "w");
    if (arquivo1 == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    for (int k = 0; k < totalUsuarios; k++) {
        fprintf(arquivo1, "%d %s %s %d %.2f\n", Usuarios[k].id, Usuarios[k].nome,Usuarios[k].snome, Usuarios[k].idade, Usuarios[k].saldo);
    }

    fclose(arquivo1);

    printf(">Transferência realizada com sucesso!!!\n");
    printf(">O usuário de ID %d transferiu %.2lf para o usuário de ID %d\n", IDorigem, ValorTrans, IDalvo);
}

void removerUsuario() {
    int ID, i = 0;
    carregararquivo();
    struct cadastro usuario;
    FILE *arqtemporario;

    printf("Digite qual ID deseja remover: ");
    scanf("%d", &ID);

    arquivo1 = fopen("banco_de_dados.txt", "r");
    if (arquivo1 == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    arqtemporario = fopen("temp.txt", "w");
    if (arqtemporario == NULL) {
        printf("Erro ao abrir o arquivo temporário\n");
        fclose(arquivo1);
        return;
    }

    int usuarioRemovido = 0;
    while (fscanf(arquivo1, "%d %s %s %d %lf", &usuario.id, usuario.nome, usuario.snome, &usuario.idade, &usuario.saldo) == 5) {
        if (usuario.id != ID) {
            fprintf(arqtemporario, "%d %s %s %d %.2lf\n", usuario.id, usuario.nome, usuario.snome, usuario.idade, usuario.saldo);
        } else {
            usuarioRemovido = 1;
        }
    }
    fclose(arquivo1);
    fclose(arqtemporario);

    remove("banco_de_dados.txt");
    rename("temp.txt", "banco_de_dados.txt");

    if (usuarioRemovido) {
        printf(">Usuário de ID %d removido com sucesso\n", ID);
    } else {
        printf(">Usuário de ID %d não encontrado\n", ID);
    }
}

int main(void) {

    setlocale(LC_ALL,"");

    Usuarios = malloc(sizeof(struct cadastro));

    int opc, quant;

    printf("\n\n::Bem vindo ao nosso banco digital::\n\n");

    do {
        do {
            printf("\nEscolha uma das opções:\n1 - Se deseja adicionar um usuário\n2 - Se deseja adicionar vários usuários\n3 - Se deseja buscar um usuário por ID\n4 - Se deseja fazer transferência entre usuários\n5 - Se deseja remover um usuário por ID\n6 - Se deseja sair do programa\n");
            scanf("%d", &opc);
        } while (opc < 1 || opc > 6);

        switch (opc) {
            case 1: adcUsuario(); break;
            case 2: adcUsuarios(); break;
            case 3: buscarID(); break;
            case 4: transferencia(); break;
            case 5: removerUsuario(); break;
            case 6:
                printf("Ok, esperamos que tenha gostado da experiência, até a próxima");
            default: break;
        }
    } while (opc != 6);

    free(Usuarios);
    return 0;
}
