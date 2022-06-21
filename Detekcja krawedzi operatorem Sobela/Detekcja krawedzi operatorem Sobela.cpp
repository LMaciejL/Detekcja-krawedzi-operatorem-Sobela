#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int m1[3][3] = { {-1,0,1}, {-2,0,2},{-1,0,1} };
int m2[3][3] = { {0,1,2},{-1,0,1},{-2,-1,0} };
int m3[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };
int m4[3][3] = { {2,1,0},{1,0,-1},{0,-1,-2} };
int m5[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} };
int m6[3][3] = { {0,-1,-2},{1,0,-1},{2,1,0} };
int m7[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };
int m8[3][3] = { {-2,-1,0},{-1,0,1},{0,1,2} };
struct BITMAPFILEHEADER {
    short bfType;
    int bfSize;
    short bfReserved1;
    short bfReserved2;
    int bfOffBits;
};
struct BITMAPINFOHEADER {
    int biSize;
    int biWidth;
    int biHeight;
    short biPlanes;
    short biBitCount;
    int biCompression;
    int biSizeImage;
    int biXpelsPerMeter;
    int biYpelsPerMeter;
    int biCrlUses;
    int biCrlImportant;
};
void readBFH(fstream& file, BITMAPFILEHEADER& bfh) {
    file.read((char*)&bfh.bfType, 2);
    file.read((char*)&bfh.bfSize, 4);
    file.read((char*)&bfh.bfReserved1, 2);
    file.read((char*)&bfh.bfReserved2, 2);
    file.read((char*)&bfh.bfOffBits, 4);
}
void readBIH(fstream& file, BITMAPINFOHEADER& bih) {
    file.read((char*)&bih.biSize, 4);
    file.read((char*)&bih.biWidth, 4);
    file.read((char*)&bih.biHeight, 4);
    file.read((char*)&bih.biPlanes, 2);
    file.read((char*)&bih.biBitCount, 2);
    file.read((char*)&bih.biCompression, 4);
    file.read((char*)&bih.biSizeImage, 4);
    file.read((char*)&bih.biXpelsPerMeter, 4);
    file.read((char*)&bih.biYpelsPerMeter, 4);
    file.read((char*)&bih.biCrlUses, 4);
    file.read((char*)&bih.biCrlImportant, 4);
}
void wczytajNaglowek(fstream& file, int* naglowek, BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih) {
    unsigned char x;
    file.seekg(0, ios_base::beg);
    for (int i = 0; i < bfh.bfOffBits; i += 1) {
        file.read((char*)&x, 1);
        naglowek[i] = x;
    }
}
void showBFH(fstream& file, BITMAPFILEHEADER& bfh) {
    cout << "(2) bfType: " << bfh.bfType << "\n";
    cout << "(4) bfSize: " << bfh.bfSize << "\n";
    cout << "(2) bfReserved1: " << bfh.bfReserved1 << "\n";
    cout << "(2) bfReserved2: " << bfh.bfReserved2 << "\n";
    cout << "(4) bfOffBits: " << bfh.bfOffBits << "\n";
}
void showBIH(fstream& file, BITMAPINFOHEADER& bih) {
    cout << "(4) biSize: " << bih.biSize << "\n";
    cout << "(4) biWidth: " << bih.biWidth << "\n";
    cout << "(4) biHeight: " << bih.biHeight << "\n";
    cout << "(2) biPlanes: " << bih.biPlanes << "\n";
    cout << "(2) biBitCount: " << bih.biBitCount << "\n";
    cout << "(4) biCompression: " << bih.biCompression << "\n";
    cout << "(4) biSizeImage: " << bih.biSizeImage << "\n";
    cout << "(4) biXpelsPerMeter: " << bih.biXpelsPerMeter << "\n";
    cout << "(4) biYpelsPerMeter: " << bih.biYpelsPerMeter << "\n";
    cout << "(4) biCrlUses: " << bih.biCrlUses << "\n";
    cout << "(4) biCrlImportant: " << bih.biCrlImportant << "\n";
}
void readImage(fstream& file, int** image, int width, int height) {
    unsigned char x;
    for (int i = 0; i < height; i += 1) {
        for (int j = 0; j < width; j += 1) {
            file.read((char*)&x, 1);
            image[i][j] = x;
        }
    }
}
void saveImage(int width, int height, BITMAPFILEHEADER& bfh, int* naglowek, int** oi) {
    unsigned char x;
    fstream outputImage;
    string nazwa;
    cout << "nazwa pliku out: ";
    cin >> nazwa;
    outputImage.open(nazwa, ios_base::out | ios::binary);
    outputImage.seekg(0, ios_base::beg);
    for (int i = 0; i < bfh.bfOffBits; i += 1) {
        x = naglowek[i];
        outputImage.write((char*)&x, 1);
        cout << x << " ";
    }
    cout << "naglowek zapisany";
    outputImage.seekg(bfh.bfOffBits, ios_base::beg);
    for (int i = 0; i < height; i += 1) {
        for (int j = 0; j < width; j += 1) {
            x = oi[i][j];
            outputImage.write((char*)&x, 1);
        }
    }
    cout << "zapisano";
}
void mask(int width, int height, int** image, int** outputImage, int m[3][3]) {
    for (int i = 1; i < height - 1; i += 1) {
        for (int j = 3; j < width - 3; j += 1) {
            outputImage[i][j] = outputImage[i][j] + (m[0][0] * image[i - 1][j - 3]) + (m[0][1] * image[i - 1][j]) + (m[0][2] * image[i - 1][j + 3]) + (m[1][0] * image[i][j - 3]) + (m[1][1] * image[i][j]) + (m[1][2] * image[i][j + 3]) + (m[2][0] * image[i + 1][j - 3]) + (m[2][1] * image[i + 1][j]) + (m[2][2] * image[i + 1][j + 3]);
        }
    }

}
void sobelTransform(fstream& file, int** image, int width, int height, int** outputImage) {
    mask(width, height, image, outputImage, m1);
    mask(width, height, image, outputImage, m1);
    mask(width, height, image, outputImage, m2);
    mask(width, height, image, outputImage, m3);
    mask(width, height, image, outputImage, m4);
    mask(width, height, image, outputImage, m5);
    mask(width, height, image, outputImage, m6);
    mask(width, height, image, outputImage, m7);
    mask(width, height, image, outputImage, m8);
    for (int i = 0; i < height; i += 1) {
        for (int j = 0; j < width; j += 1) {
            outputImage[i][j] = outputImage[i][j] / 8;
        }
    }
}
int main()
{
    cout << "Podaj nazwe pliku: ";
    string file_name;
    cin >> file_name;
    fstream file;
    file.open(file_name, ios::in | ios::binary);
    if (!file) {
        cout << "brak pliku\n";
        return 0;
    }
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    readBFH(file, bfh);
    showBFH(file, bfh);
    readBIH(file, bih);
    showBIH(file, bih);
    int offBits = bfh.bfOffBits;
    file.seekg(offBits, ios_base::beg);
    //tablica dynamiczna przechowująca obraz
    int width = bih.biWidth * 3;
    int height = bih.biHeight;
    if (width % 4 == 1) width += 3;
    if (width % 4 == 2) width += 2;
    if (width % 4 == 3) width += 1;
    cout << width << " ";
    cout << height << " \n";
    int** image = new int* [height];
    for (int i = 0; i < height; i += 1) {
        image[i] = new int[width];
    }
    //tablica dynamiczna przechowująca obraz wyjsciowy
    int** outputImage = new int* [height];
    for (int i = 0; i < height; i += 1) {
        outputImage[i] = new int[width];
    }
    //tablica dynamiczna przechowująca nagłówek
    int rozmiar = bfh.bfOffBits;
    int* naglowek = new int[rozmiar];
    wczytajNaglowek(file, naglowek, bfh, bih);
    readImage(file, image, width, height);
    for (int i = 0; i < height; i += 1) {
        for (int j = 0; j < width; j += 1) {
            outputImage[i][j] = image[i][j];
        }
    }
    sobelTransform(file, image, width, height, outputImage);
    saveImage(width, height, bfh, naglowek, outputImage);
    for (int i = 0; i < height; i += 1) {
        delete[] outputImage[i];
    }
    for (int i = 0; i < height; i += 1) {
        delete[] image[i];
    }
    file.close();
}
