#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>

class BmpReader
{
private:
    std::vector<std::vector<unsigned char>> data;

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

public:
    bool openBMP(const std::string &fileName)
    {
        std::ifstream file;

        file.open(fileName, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }

        // читает заголовок файла (BITMAPFILEHEADER) из файла и сохраняет его в переменную
        file.read(reinterpret_cast<char *>(&fileHeader), sizeof(BITMAPFILEHEADER));
        // читает информационный заголовок(BITMAPINFOHEADER) из файла и сохраняет его в переменную
        file.read(reinterpret_cast<char *>(&infoHeader), sizeof(BITMAPINFOHEADER));

        // (в шестнадцатеричной системе) соответствует сигнатуре "BM" в начале BMP-файла
        const int bmType = 0x4D42;

        // Количество бит на пиксель равно 24 или 32
        const int bitsPerPixel_24 = 24;
        const int bitsPerPixel_32 = 32;

        if (fileHeader.bfType != bmType || infoHeader.biBitCount != bitsPerPixel_24 && infoHeader.biBitCount != bitsPerPixel_32)
        {
            file.close();
            return false;
        }

        const auto &width = infoHeader.biWidth;
        const auto &height = infoHeader.biHeight;

        // Заранее выделяем нужную память
        data.resize(width);
        for (auto &i : data)
        {
            i.resize(height);
        }
        // определение размера отступа в конце каждой строки
        const int padding = ((infoHeader.biWidth * (infoHeader.biBitCount / 8)) % 4) & 3;

        const auto blackChar = (unsigned char)0xB0; // = ░

        const auto whiteChar = (unsigned char)0xB2; // = ▓

        const bool isBitsPerPixel_32 = infoHeader.biBitCount == bitsPerPixel_32;

        // в bmp файлы массив снизу вверх
        for (int y = height - 1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned char blue, green, red, alpha;

                // 1 байт данных из файла
                file.read(reinterpret_cast<char *>(&blue), 1);
                file.read(reinterpret_cast<char *>(&green), 1);
                file.read(reinterpret_cast<char *>(&red), 1);

                if (isBitsPerPixel_32)
                {
                    file.read(reinterpret_cast<char *>(&alpha), 1);
                }

                if (red == 0 && green == 0 && blue == 0)
                {
                    data[x][y] = whiteChar;
                }
                else
                {
                    data[x][y] = blackChar;
                }
            }
            file.seekg(padding, std::ios::cur);
        }

        file.close();

        return true;
    }

    void displayBMP() const
    {
        for (int y = 0; y < infoHeader.biHeight; y++)
        {
            for (int x = 0; x < infoHeader.biWidth; x++)
            {
                std::cout << data[x][y];
            }
            std::cout << "\n";
        };
    }

    /**
     * ничего не делает, нужно для соответствию тех заданию
     * освобождение ресурсов происходит через деструктор
     */
    void closeBMP()
    {
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: drawBmp.exe <file_path>" << std::endl;
        return 1;
    }

    BmpReader bmpReader;
    if (!bmpReader.openBMP(argv[1]))
    {
        std::cerr << "Failed to open BMP file: " << argv[1] << std::endl;
        return 1;
    }

    bmpReader.displayBMP();
    return 0;
}
