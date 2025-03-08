
#include <stdio.h>
#include <string.h>

#define MAX_FILES 50
#define FILENAME_SIZE 256
#define DISK_SIZE 1024
#define DISK_FILE "virtual_disk.dat" //nome do disco virtual


typedef struct DataFile {
    char name[FILENAME_SIZE];
    int file_size;
    int file_start;
} DataFile;


DataFile directory[MAX_FILES]; //o diretorio ira armazenar os arquivos
int file_quant=0;
char disk[DISK_SIZE]; //disco virtual

//---------------------------------------------------------------------------------------------------------


//funçao para carregar os arquivos do disco virtual
void loadDisk() {

    FILE *file = fopen(DISK_FILE, "rb");

    if(file != NULL) {

        fread(&file_quant, sizeof(int), 1, file); //lendo a quantidade de arquivos
        fread(directory, sizeof(DataFile), MAX_FILES, file); //lendo os dados do diretorio e passando para o array directory
        fread(disk, sizeof(char), DISK_SIZE, file);//lendo os dados do disco e passando para o array
        fclose(file);

    } else {

        memset(directory, 0, sizeof(directory));
        memset(disk, 0, sizeof(disk));
        file_quant = 0; 

    }
}


//funçaõ para salvar os arquivos no disco
void saveDisk() {

    FILE *file = fopen(DISK_FILE,"wb");

    if(file != NULL) {

        fwrite(&file_quant, sizeof(int), 1, file); //escreve o num de arquivos
        fwrite(directory, sizeof(DataFile), MAX_FILES, file); //escreve os dados do diretorio
        fwrite(disk, sizeof(char), DISK_SIZE, file); //escreve os dados do disco

        fclose(file);

    } else {
        printf("erro ao salvar o disco virtual.");
    }
}



//função para criar arquivos
void cat(char *filename, char *data) {

    if(file_quant >= MAX_FILES) 
    {
        printf("erro: diretório cheio\n");
        return;
    }

    int data_size = strlen(data_size);

    int start=0;
    if (file_quant == 0) { //define a posiçaõ do arquivo1 em 0 se o disco estiver vazio. se nao estiver, armazena apos o ultimo arquivo
        start = 0;
    } else {
        start = directory[file_quant - 1].file_start + directory[file_quant - 1].file_size;
    }

    if(start + data_size > DISK_SIZE) {
        printf("erro: disco cheio\n");

        return;
    }

//atualizando os dados no array 
    strncpy(directory[file_quant].name, filename, FILENAME_SIZE-1);
    directory[file_quant].name[FILENAME_SIZE-1] = '\0';
    directory[file_quant].file_size = data_size;
    directory[file_quant].file_start = start;

    //atualizando no disco
    strncpy(&disk[start], data, data_size);
    file_quant++;
    saveDisk();
}



void ls() {
    for(int i=0; i<file_quant; i++) {
        printf("nome: %s, tamanho: %d, inicio: %d\n", directory[i].name, directory[i].file_size, directory[i].file_start);
    }
}



void more(const char *filename) {

    for(int i=0; i<file_quant; i++) {

        if (strcmp(directory[i].name, filename)==0) {

            int start = directory[i].file_start;
            int size = directory[i].file_size;
            //percorre o tamanho do arquivo
            for(int j = start; j<start + size; j++) {
                printf("%c", disk[j]);
            }

            printf("\n");

            return;
        }
    }
    printf("erro: arquivo não encontrado\n");
}



//------------------------------------------------------------------------------------------------

int main() {
    loadDisk(); 

    int choice;
    char filename[FILENAME_SIZE];
    char content[DISK_SIZE];

    do {
        printf("\nEscolha uma opção:\n");
        printf("1. Criar um arquivo\n");
        printf("2. Listar arquivos do diretório\n");
        printf("3. Mostrar conteúdo de determinado arquivo\n");
        printf("0. Sair\n");
        scanf("%d", &choice);
        getchar(); // Limpa o buffer de entrada

        switch(choice) {
            case 1:
                printf("Insira o nome do arquivo:\n");
                fgets(filename, FILENAME_SIZE, stdin);
                strtok(filename, "\n"); // Removendo a quebra de linha
                
                printf("Insira o conteúdo para %s:\n", filename);
                fgets(content, DISK_SIZE, stdin);
                
                cat(filename, content);
                break;
            case 2:
                ls();
                break;
            case 3: {
                printf("Insira o nome do arquivo:\n");
                fgets(filename, FILENAME_SIZE, stdin);
                strtok(filename, "\n"); // Removendo a quebra de linha
                more(filename);
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while(choice != 0);

    return 0;
}


