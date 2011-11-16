#include "huffman.h"

using namespace std;

Arquivo::Arquivo(char* nomeArquivo) : tamanhoVetorAscii(256) {
    frequenciaCaracteres = new int[tamanhoVetorAscii];
    int i;
    for (i = 0; i < tamanhoVetorAscii; i++)
        frequenciaCaracteres[i] = 0;
    arquivoOrigem = fopen(nomeArquivo, "r");
    /*Lê tamanho arquivo*/
    fseek(arquivoOrigem, 0, SEEK_END);
    tamanhoArquivoOrigem = ftell(arquivoOrigem);
    rewind(arquivoOrigem);
    cout << endl << endl << endl;
    cout << "tamanho do arquivo: " << tamanhoArquivoOrigem << endl;
}

const int Arquivo::getTamanhoVetorAscii() const {
    return tamanhoVetorAscii;
}

int* Arquivo::getFrequenciaCaracteres() const {
    return frequenciaCaracteres;
}

int* Arquivo::contaCaracteres() {
    //int i;
    int caracterArquivo;
    caracterArquivo = getc(arquivoOrigem);
    while (caracterArquivo != EOF) {
        frequenciaCaracteres[caracterArquivo]++;
        //putchar(caracterArquivo);
        //cout << caracterArquivo << endl;
        caracterArquivo = getc(arquivoOrigem);
    }
    /*Teste para mostrar a tabela de frequencia*/
    //for (i = 0; i < tamanhoVetorAscii; i++)
        //cout << frequenciaCaracteres[i] << "[" << i << "]" << endl;
    fclose(arquivoOrigem);
}

Filtragem::Filtragem() : esq(NULL), dir(NULL) {

}

int Filtragem::getCaracterAscii() const {
    return caracterAscii;
}

int Filtragem::getFrequenciaCaracterAscii() const {
    return frequenciaCaracterAscii;
}

/*Sobrecarga do operador << para a classe Filtragem*/
std::ostream & operator <<(std::ostream &os, const Filtragem & p) {
    os << "caractere: " << p.getCaracterAscii() << ", frequencia: " <<
            p.getFrequenciaCaracterAscii() << ", folha: " << p.isLeaf() <<
            ", bincode: " << p.getCodigobinario();
    return os;
}

bool Filtragem::operator<(const Filtragem &A) const {
    if (frequenciaCaracterAscii < A.frequenciaCaracterAscii || caracterAscii < A.caracterAscii)
        return true;
    return false;
}

bool Filtragem::operator==(const Filtragem &A) const {
    if (frequenciaCaracterAscii == A.frequenciaCaracterAscii)
        return true;
    return false;
}

void Estatistica::filtraFrequencia(int tamanhoVetor,
        int* vetorFrequenciaCaracteres) {
    int i;
    i = tamanhoVetor;
    //cout << "ENTRA NA PILHA ASSIM:" << endl;
    while (--i > 0) {
        if (vetorFrequenciaCaracteres[i] > 0) {
            Filtragem* contagem = new Filtragem();
            contagem->dir = NULL;
            contagem->esq = NULL;
            contagem->caracterAscii = i;
            contagem->frequenciaCaracterAscii = vetorFrequenciaCaracteres[i];
            contagem->leaf = true;
            //cout << (*contagem) << endl;
            frequenciaAscii.push(contagem);
        }
    }/*
    cout <<"FICA NA PILHA ASSIM:" << endl;
    while (!frequenciaAscii.empty()) {
        Filtragem* frequencia = frequenciaAscii.top();
        cout << (*frequencia) << endl; // Print highest priority string
        frequenciaAscii.pop(); // Remmove highest priority string
    }
*/
}

Huffman::Huffman() {
    codigoBinario = "";
}

void Huffman::encodeHuffman(filaprioridade fila) {
    arvoreCodificada = fila;
    cout << "HUFFMAN:::FICA NA PILHA ASSIM:" << endl;
    while (arvoreCodificada.size() > 1) {
        Filtragem* no = new Filtragem();
        no->esq = arvoreCodificada.top();
        no->esq->codigobinario = "0";
        arvoreCodificada.pop();
        no->dir = arvoreCodificada.top();
        no->dir->codigobinario = "1";
        arvoreCodificada.pop();
        no->leaf = false;
        no->caracterAscii = 257;
        no->frequenciaCaracterAscii = (no->dir->frequenciaCaracterAscii +
                no->esq->frequenciaCaracterAscii);

        arvoreCodificada.push(no);
    }
    root = new Filtragem;
    root = arvoreCodificada.top();
    root->leaf = false;
    root->caracterAscii = 258;
    root->frequenciaCaracterAscii = (root->dir->frequenciaCaracterAscii +
            root->esq->frequenciaCaracterAscii);
    arvoreCodificada.pop();
    cout << (*root) << endl << endl << endl;

    /*cout << "FICA NA PILHA ASSIM:" << endl;
    while (!arvoreCodificada.empty()) {
        Filtragem* frequencia = arvoreCodificada.top();
        cout << (*frequencia) << endl; // Print highest priority string
        arvoreCodificada.pop(); // Remmove highest priority string
    }*/
}

void Huffman::criaCodigo(Filtragem* root, string bincode) {
    if (root != NULL) {

        bincode += root->getCodigobinario();
        criaCodigo(root->getDir(), bincode);
        //cout<<"DIR - bincode:::::"<<bincode<<endl;
        criaCodigo(root->getEsq(), bincode);
        //cout<<"ESQ - bincode:::::"<<bincode<<endl;
        if (root->isLeaf()) {
            root->setCodigobinario(bincode);
            //cout << *(root) << endl;
            tabelaCodigoBinario.push(root);
            //cout << *tabelaCodigoBinario.top() << endl;
        }
    }
}

void Huffman::imprimeTeste() {
    cout << endl << endl << endl;
    cout << "IMPRIMINDO CODE:" << endl;
    while (!tabelaCodigoBinario.empty()) {
        cout << *tabelaCodigoBinario.top() << endl; // Print highest priority string
        tabelaCodigoBinario.pop(); // Remmove highest priority string
    }
}

bool verificaArquivo(char* nomeArquivo) {
    FILE* arquivo;
    if ((arquivo = fopen(nomeArquivo, "r")) == NULL) {
        cout << "Arquivo não encontrado!" << endl;
        //fclose(arquivo);
        return 1;
    }
    fclose(arquivo);
    return 0;
}
