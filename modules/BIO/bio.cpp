#include <cmath>
#include <iostream>
#include <vector>

#include "papd_config.h"

using namespace std;

// Параметры модели
const double dx = double(L) / double(Nx); // Шаг сетки
const double dt = 0.001;                  // Шаг по времени
const int Nt = 10000;                     // Число временных шагов
const double D_s = 0.01;                  // Коэффициент диффузии субстрата
const double D_x = 0.005;                 // Коэффициент диффузии бактерий
const double mu_max = 0.1;       // Максимальная скорость роста бактерий
const double K_s = 0.5;          // Константа полунасыщения
const double Y_xs = 0.5;         // Коэффициент выхода биомассы на субстрат
const double k_d = 0.01;         // Константа гибели бактерий
const double k_a = 0.02;         // Константа адгезии
const double k_det = 0.005;      // Константа отрыва биопленки
const double eta = 0.8;          // Коэффициент эффективности потребления
const double u = velocity_water; // Постоянная скорость потока

// Функция роста бактерий
double growthRate(double C_s, double C_x_pl, double C_x_ad) {
  return (mu_max * C_s / (K_s + C_s)) * (C_x_pl + eta * C_x_ad);
}

// Главная функция
int main() {
  vector<double> C_s(Nx, 1.0);    // Концентрация субстрата
  vector<double> C_x_pl(Nx, 0.1); // Планктонная биомасса
  vector<double> C_x_ad(Nx, 0.1); // Адгезированная биомасса

  for (int t = 0; t < Nt; t++) {
    vector<double> C_s_new = C_s;
    vector<double> C_x_pl_new = C_x_pl;
    vector<double> C_x_ad_new = C_x_ad;

    for (int i = 1; i < Nx - 1; i++) {
      double R_s = (1 / Y_xs) * growthRate(C_s[i], C_x_pl[i], C_x_ad[i]);
      double R_x = mu_max * C_s[i] / (K_s + C_s[i]) * C_x_pl[i];

      // Адвекция (схема против потока)
      double adv_s = -u * (C_s[i] - C_s[i - 1]) / dx;
      double adv_x_pl = -u * (C_x_pl[i] - C_x_pl[i - 1]) / dx;

      // Уравнение субстрата
      C_s_new[i] = C_s[i] + dt * (D_s * (C_s[i + 1] - 2 * C_s[i] + C_s[i - 1]) /
                                      (dx * dx) +
                                  adv_s - R_s);

      // Уравнение планктонной биомассы
      C_x_pl_new[i] =
          C_x_pl[i] +
          dt * (D_x * (C_x_pl[i + 1] - 2 * C_x_pl[i] + C_x_pl[i - 1]) /
                    (dx * dx) +
                adv_x_pl + R_x - k_d * C_x_pl[i] - k_a * C_x_pl[i] +
                k_det * C_x_ad[i]);

      // Уравнение адгезированной биомассы
      C_x_ad_new[i] = C_x_ad[i] + dt * (k_a * C_x_pl[i] - k_d * C_x_ad[i] -
                                        k_det * C_x_ad[i]);
    }

    // Обновление значений
    C_s = C_s_new;
    C_x_pl = C_x_pl_new;
    C_x_ad = C_x_ad_new;

    // Граничные условия (свободный выход)
    C_s[Nx - 1] = C_s[Nx - 2];
    C_x_pl[Nx - 1] = C_x_pl[Nx - 2];
    C_x_ad[Nx - 1] = C_x_ad[Nx - 2];
  }

  // Вывод результатов
  for (int i = 0; i < Nx; i += 10) {
    cout << "x = " << i * dx << " C_s = " << C_s[i] << " C_x_pl = " << C_x_pl[i]
         << " C_x_ad = " << C_x_ad[i] << endl;
  }
  return 0;
}