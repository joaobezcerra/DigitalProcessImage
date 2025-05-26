#include <stdio.h>
#include <stdlib.h>

#pragma pack(1) // Garante que a estrutura não tenha preenchimento de bytes extras

// Estrutura para o cabeçalho do arquivo BMP
typedef struct {
    unsigned char id[2]; // Identificador do BMP ('BM')
    int size;            // Tamanho total do arquivo
    short reserved1, reserved2;
    int offset;          // Posição inicial dos dados da imagem
} BMPHeader;

// Estrutura para o cabeçalho DIB da imagem
typedef struct {
    int headerSize;  // Tamanho do cabeçalho DIB
    int width, height; // Largura e altura da imagem
    short planes;
    short bpp;       // Bits por pixel
    int compression;
    int imageSize;
    int xPPM, yPPM;
    int colorsUsed;
    int importantColors;
} DIBHeader;

// Função para abrir o arquivo BMP e validar se ele é um BMP válido
FILE* abrirArquivoBMP(const char *filename) {
    FILE *file = fopen(filename, "rb"); // Abre o arquivo em modo de leitura binária
    if (!file) {
        printf("Erro ao abrir o arquivo BMP!\n");
        exit(1);
    }
    return file;
}

char* newName(const char *fileName, const char *addName) {
    char *aux = (char*)malloc(256 * sizeof(char)); // Aloca memória dinamicamente
    if (!aux) {
        printf("Erro ao alocar memória para o nome do arquivo.\n");
        return NULL;
    }

    int i = 0;
    while (fileName[i] != '.' && fileName[i] != '\0') {
        aux[i] = fileName[i];
        i++;
    }

    // Adiciona o sufixo desejado e a extensão .bmp
    snprintf(aux + i, 256 - i, "_%s.bmp", addName);
    printf("aux: %s\n", aux);

    return aux;
}

// Função para ler os cabeçalhos BMP e DIB
void lerCabecalhos(FILE *file, BMPHeader *bmpHeader, DIBHeader *dibHeader) {
    fread(bmpHeader, sizeof(BMPHeader), 1, file);
    fread(dibHeader, sizeof(DIBHeader), 1, file);

    // Verifica se o arquivo é realmente um BMP
    if (bmpHeader->id[0] != 'B' || bmpHeader->id[1] != 'M') {
        printf("O arquivo não é um BMP válido!\n");
        fclose(file);
        exit(1);
    }
}

// Função para exibir informações básicas da imagem
void exibirInformacoes(DIBHeader dibHeader) {
    printf("Largura: %d px\n", dibHeader.width);
    printf("Altura: %d px\n", dibHeader.height);
    printf("Bits por pixel: %d\n", dibHeader.bpp);
}

// Função para copiar a imagem BMP para um novo arquivo
void copiarImagem(FILE *file, const char *novoNome, int fileSize) {
    FILE *newFile = fopen(novoNome, "wb"); // Abre um novo arquivo para escrita binária
    if (!newFile) {
        printf("Erro ao criar a cópia!\n");
        fclose(file);
        exit(1);
    }

    fseek(file, 0, SEEK_SET); // Volta ao início do arquivo
    unsigned char *buffer = (unsigned char *)malloc(fileSize); // Aloca memória para armazenar a imagem
    fread(buffer, fileSize, 1, file); // Lê todo o arquivo BMP para a memória
    fwrite(buffer, fileSize, 1, newFile); // Escreve a cópia no novo arquivo

    printf("Cópia criada como '%s'\n", novoNome);

    free(buffer); // Libera memória alocada
    fclose(newFile); // Fecha o novo arquivo
}

void converterCinza(FILE *file, const char *nomeImagem, int fileSize) {
    const char *addName = "Cinza";
    char *newFileName = newName(nomeImagem, addName);

    FILE *newFile = fopen(newFileName, "wb");
    if (!newFile) {
        printf("Erro ao criar a imagem em tons de cinza!\n");
        return;
    }

    fseek(file, 0, SEEK_SET);     // Seta o ponteiro no inicio do arquivo
    unsigned char *buffer = (unsigned char *)malloc(fileSize);   // Aloca na memória espaço para a imagem
    fread(buffer, fileSize, 1, file);      // Lê todo o conteúdo do arquivo BMP para a memória

    int offset = ((BMPHeader*)buffer)->offset; // Posição onde os pixels começam
    int width = ((DIBHeader*)(buffer + sizeof(BMPHeader)))->width; // Largura da imagem
    int height = ((DIBHeader*)(buffer + sizeof(BMPHeader)))->height; // Altura da imagem
    short bpp = ((DIBHeader*)(buffer + sizeof(BMPHeader)))->bpp; // Bits por pixel

    printf("Valor de bpp: %d\n", bpp);  // Exibe o valor de bpp para depuração

    // Verifica se o BMP está usando 32 bits por pixel (bpp = 32)
    if (bpp != 32 && bpp != 24) {
        printf("A imagem não é 24bpp nem 32bpp, não é possível aplicar a conversão!\n");
        fclose(newFile);
        free(buffer);
        return;
    }

    int rowSize = (width * (bpp / 8) + 3) & (~3); // Tamanho de cada linha considerando o padding
    unsigned char r, g, b, gray;    //var's separando os canais rgb

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixelIndex = offset + (y * rowSize) + (x * (bpp / 8));

            r = buffer[pixelIndex + 2]; // Componente vermelho
            g = buffer[pixelIndex + 1]; // Componente verde
            b = buffer[pixelIndex];     // Componente azul

            // Fórmula de conversão para tons de cinza (luminosidade)
            gray = (unsigned char)(r * 0.299 + g * 0.587 + b * 0.114);

            // Define o novo valor de todos os componentes (r, g, b)
            buffer[pixelIndex] = buffer[pixelIndex + 1] = buffer[pixelIndex + 2] = gray;
        }
    }

    // Escreve o buffer alterado no novo arquivo
    fwrite(buffer, fileSize, 1, newFile);
    printf("Imagem em tons de cinza criada como '%s'\n", newFileName);

    free(buffer);
    free(newFileName); 
    fclose(newFile);
}

void negativarImagem(FILE *file, const char *nomeImagem, int fileSize){
    const char *addName = "Negativa";
    char *newFileName = newName(nomeImagem, addName);

    FILE *newFile = fopen(newFileName, "wb");
    if (!newFile) {
        printf("Erro ao criar a imagem em tons de negativa!\n");
        printf("%s", newFileName);
        return;
    }

    fseek(file, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(fileSize);   // Aloca na memória espaço para a imagem
    fread(buffer, fileSize, 1, file);      // Lê todo o conteúdo do arquivo BMP para a memória

    int offset = ((BMPHeader*)buffer)->offset; // Posição onde os pixels começam
    int width = ((DIBHeader*)(buffer + sizeof(BMPHeader)))->width; // Largura da imagem
    int height = ((DIBHeader*)(buffer + sizeof(BMPHeader)))->height; // Altura da imagem
    short bpp = ((DIBHeader*)(buffer + sizeof(BMPHeader)))->bpp; // Bits por pixel

    int rowSize = (width * (bpp / 8) + 3) & (~3); // Tamanho de cada linha considerando o padding
    unsigned char r, g, b;    //var's separando os canais rgb
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixelIndex = offset + (y * rowSize) + (x * (bpp / 8));

            r = buffer[pixelIndex + 2]; // Componente vermelho
            g = buffer[pixelIndex + 1]; // Componente verde
            b = buffer[pixelIndex];     // Componente azul

            // Define o novo valor de todos os componentes (r, g, b)
            buffer[pixelIndex + 2] = 255 - r; //Inverso do canal R
            buffer[pixelIndex + 1] = 255 - g; //Inverso do canal G
            buffer[pixelIndex] = 255 - b; //Inverso do canal B

        }
    }

    // Escreve o buffer alterado no novo arquivo
    fwrite(buffer, fileSize, 1, newFile);
    printf("Imagem negativa criado como '%s'\n", newFileName);

    free(buffer);
    free(newFileName);  
    fclose(newFile);
}



int main() {
    const char *nomeArquivo = "imagem.bmp";
    FILE *file = abrirArquivoBMP(nomeArquivo); // Abre o arquivo BMP

    BMPHeader bmpHeader;
    DIBHeader dibHeader;
    
    lerCabecalhos(file, &bmpHeader, &dibHeader); // Lê os cabeçalhos
    exibirInformacoes(dibHeader); // Exibe informações básicas da imagem
    
    copiarImagem(file, "copia.bmp", bmpHeader.size); // Cria uma cópia da imagem
    converterCinza(file, nomeArquivo, bmpHeader.size);
    negativarImagem(file,nomeArquivo, bmpHeader.size);
    fclose(file); // Fecha o arquivo original
    return 0;
}
