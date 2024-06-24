#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define SUCCES 0
#define FAIL -1

//Char = 1 byte Short = 2 byte Int = 4 byte

struct BMP_header {
    char Signature[2]; //'BM'
    int FileSize; //Rozmiar pliku
    int Reserved; // nie uzywane
    int DataOffset; // wskaznik danych 
};

struct BMP_InfoHeader {
    int Size; //Rozmiar InfoHeader'u
    int Width; //Szerokosc
    int Height; //Wysokosc
    short Planes; //Liczba plane'ów 
    short BitCount; //Ilosc bitow na pixel 1 - monochromatyczna 4 - 4 bitowa paleta
    //8 - 8 bitowa paleta 16 - 16 bitowa paleta 24 - 24 bitowa paleta
    int Compression; //Kompresja: 0 - brak 1 - 8bit rle 2- 4bit rle
    int imageSize; //Rozmiar po kompresji
    int XpixelsPerM; //Rozdzielczosc pozioma
    int YpixelsPerM; //Rozdzielczosc pionowa
    int ColorsUsed; //Ilosc uzytych kolorow
    int ColorsImportant; //Liczba waznych kolorow
};

//RGB
struct BMP_RGB{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct BMP_BMP{
    struct BMP_header *header;
    struct BMP_InfoHeader *info;
    unsigned char *data;
};

 
struct BMP_header * loadHeader(char *filename,char *mode,struct BMP_BMP file);
struct BMP_InfoHeader * loadInfoHeader(char *filename,char *mode,struct BMP_BMP file);
unsigned char * loadRasterData(char *filename,char *mode,struct BMP_BMP file);
int loadBMP(char *filename,char *mode,struct BMP_BMP file);
void FreeBMP(struct BMP_BMP file);

struct BMP_header * loadHeader(char *filename,char *mode,struct BMP_BMP file){
    FILE *fp=fopen(filename,mode);
    struct BMP_header * header=(struct BMP_header *) malloc(sizeof(char)*14);
    if(fp==NULL){
        printf("Nie mozna otworzyc wskazanego pliku BMP!\n");
        return NULL;
    }
    fread(header->Signature,sizeof(char),2,fp);
    fread(&header->FileSize,sizeof(int),1,fp);
    fread(&header->Reserved,sizeof(int),1,fp);
    fread(&header->DataOffset,sizeof(int),1,fp);
    
    printf("BMP HEADER DATA:\n");
    printf("File size:%d\n",header->FileSize);
    printf("Reserved:%d\n",header->Reserved);
    printf("DataOffset:%d\n",header->DataOffset);
    
    //Sprawdzenie zgodnosci pliku
    if(header->Signature[0]!='B' || header->Signature[1]!='M'){
        printf("Nie prawidlowy plik BMP!\n");
        return NULL;
    }
    


    fclose(fp);
    return header;
}

struct BMP_InfoHeader * loadInfoHeader(char *filename,char *mode,struct BMP_BMP file){
    FILE *fp=fopen(filename,mode);
    struct BMP_InfoHeader *info=(struct BMP_InfoHeader *) malloc(sizeof(char)*40);
    if(fp==NULL){
        printf("Nie mozna otworzyc wskazanego pliku BMP!\n");
        return NULL;
    }
    fseek(fp,14,SEEK_SET);
    fread(&info->Size,sizeof(int),1,fp);
    fread(&info->Width,sizeof(int),1,fp);
    fread(&info->Height,sizeof(int),1,fp);
    fread(&info->Planes,sizeof(short),1,fp);
    fread(&info->BitCount,sizeof(int),1,fp);
    fread(&info->Compression,sizeof(int),1,fp);
    fread(&info->imageSize,sizeof(int),1,fp);
    fread(&info->XpixelsPerM,sizeof(int),1,fp);
    fread(&info->YpixelsPerM,sizeof(int),1,fp);
    fread(&info->ColorsUsed,sizeof(int),1,fp);
    fread(&info->ColorsUsed,sizeof(int),1,fp);

    printf("BMP INFO HEADER DATA:\n");
    printf("Size:%d\n",info->Size);
    printf("Width:%d\n",info->Width);
    printf("Height:%d\n",info->Height);
    printf("Planes:%d\n",info->Planes);
    printf("BitCount:%d\n",info->BitCount);
    printf("Compresion:%d\n",info->Compression);
    printf("ImageSize:%d\n",info->imageSize);
    printf("XpixelsPerM:%d\n",info->XpixelsPerM);
    printf("YpixelsPerM:%d\n",info->YpixelsPerM);
    printf("ColorsUsed:%d\n",info->ColorsUsed);
    printf("ColorsImportant:%d\n",info->ColorsUsed);

    fclose(fp);
    return info;
}

unsigned char * loadRasterData(char *filename,char *mode,struct BMP_BMP file){
    
    FILE *fp=fopen(filename,mode);
    struct BMP_RGB rgb;
    char *data;
    data=(char *) malloc(file.info->Width*file.info->Height*3*sizeof(char));
    fseek(fp,file.header->DataOffset,SEEK_SET);
    fread(data,sizeof(char),file.info->Width*file.info->Height*3,fp);
    if(!data){
        printf("Nie odczytane!\n");
        return NULL;
    }
    return data;
}

void FreeBMP(struct BMP_BMP file){
    
    if(file.data!=NULL){
        free(file.data);
    }

    if(file.header!=NULL){    
        free(file.header);
    }
        
    if(file.info!=NULL){   
        free(file.data);
    }
}

int loadBMP(char *filename,char *mode,struct BMP_BMP file){
    file.header=loadHeader(filename,mode,file);
    file.info=loadInfoHeader(filename,mode,file);
    file.data=loadRasterData(filename,mode,file);
    return SUCCES;
}
