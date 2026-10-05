#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
// Размеры экрана консоли 
const int WIDTH = 60;
const int HEIGHT = 22;
// Углы вращение куба 
float A = 0.0f, B = 0.0f, C = 0.0f;
// Математические функции вращения точки (X, Y, Z) в 3D-пространстве
float calculateX(float i, float j, float k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}
float calculateY(float i, float j, float k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}
float calculateZ(float i, float j, float k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}
int main() {
    // Очищаем консоль перед стартом
    std::cout << "\x1b[2J";
    while (true) {
        // Буфер экрана (текстовые символы) и Z-буфер (глубина)
        std::vector<char> buffer(WIDTH * HEIGHT, ' ');
        std::vector<float> zBuffer(WIDTH * HEIGHT, 0.0f);
        float cubeWidth = 10.0f;       // Размер грани куба
        float distanceFromCam = 60.0f; // Расстояние до камеры 
        float K1 = 30.0f;              // Коэффициент масштаба перспективы
        // Перебор точек поверхности куба 
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += 0.5f) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += 0.5f) {
                // Лямбда-функция для прорисовки одной точки
                auto renderSurface = [&](float x, float y, float z, char ch) {
                    float rx = calculateX(x, y, z);
                    float ry = calculateY(x, y, z);
                    float rz = calculateZ(x, y, z) + distanceFromCam;
                    float ooz = 1.0f / rz; // Обратная глубина (1/Z)
                    // Проекция 3D-координат на 2D-экрана
                    int xp = static_cast<int>(WIDTH / 2 + K1 * ooz * rx * 2.0f);
                    int yp = static_cast<int>(HEIGHT / 2 + K1 * ooz * ry);
                    int idx = xp + yp * WIDTH;
                    if (idx >= 0 && idx < WIDTH * HEIGHT) {
                        // Z-буферизация: рисуем только если точка ближе к камере
                        if (ooz > zBuffer[idx]) {
                            zBuffer[idx] = ooz;
                            buffer[idx] = ch;
                        }
                    }
                };
                // Отрисовка 6 граней куба разными текстурными символами 
                renderSurface(cubeX, cubeY, -cubeWidth, '@');
                renderSurface(cubeWidth, cubeY, cubeX, '$');
                renderSurface(-cubeWidth, cubeY, -cubeX, '~' );
                renderSurface(-cubeX, cubeY, cubeWidth, '#');
                renderSurface(cubeX, -cubeWidth, -cubeY, ';');
                renderSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }
        // Возвращаем курсор в левый верхний угол без перерисовки экрана
        std::cout << "\x1b[H";
        // Вывод буфера кадра в консоль
        for (int k = 0; k < WIDTH * HEIGHT; k++) {
            std::cout << (k % WIDTH ? buffer[k] : '\n');
        }
        // Изменение углов для анимаций вращения
        A += 0.05f;
        B += 0.05f;
        C += 0.01f;
        // Фиксация частоты кадров (~30 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }   
    return 0;
}
