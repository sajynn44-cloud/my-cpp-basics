#include <iostream>
using namespace std;
int main() {
    int distance = 100; // Начальное значение
    int scanCount = 1;
  cout << "--- РОБОТ НАЧАЛ ДВИЖЕНИЕ ---" << endl;
    // Цикл работает, пока дистанция больше 15 см
    while (distance > 15) {
        cout << "\nЗамер #" << scanCount << endl;
        cout << "Введи дистанцию с датчика (см): ";
        cin >> distance;
        if (distance > 15) {
            cout << "Путь свободен, едем дальше..." << endl;
        } else {
            cout << "СТОП! Препятствие обнаружено на " << distance << " см!" << endl;
        }
        scanCount++; // Увеличиваем номер замера
    }
    cout << "\n--- РОБОТ ОСТАНОВИЛСЯ ---" << endl;
    return 0;
}
