## BIO

const double L = 10.0;      // Длина трубопровода
const int Nx = 100;         // Число узлов сетки
const double dx = L / Nx;   // Шаг сетки
const double dt = 0.001;    // Шаг по времени
const int Nt = 10000;       // Число временных шагов
const double D_s = 0.01;    // Коэффициент диффузии субстрата
const double D_x = 0.005;   // Коэффициент диффузии бактерий
const double mu_max = 0.1;  // Максимальная скорость роста бактерий
const double K_s = 0.5;     // Константа полунасыщения
const double Y_xs = 0.5;    // Коэффициент выхода биомассы на субстрат
const double k_d = 0.01;    // Константа гибели бактерий
const double k_a = 0.02;    // Константа адгезии
const double k_det = 0.005; // Константа отрыва биопленки
const double eta = 0.8;     // Коэффициент эффективности потребления
const double u = 0.1;       // Постоянная скорость потока

### out

Табличка с чиселками

## 3AC

real TC = 20; -> Из этого получаем данные из справочника

real EDC = -0.67; 
real ERMS = 0.6;                    /*[V] Amplitude of AC potential */
real freq = 0.1;                    /*[Hz] Frequency of AC potential */
real timeFinal = 3 / freq; 			    /*[s] Final time */

### out

Запуск задачи по 1 точке + объеденение результатов

Средние значения
+ опционально таблица для визуализациии


## 10CO2

real TC = 20;                       /*[degC] Operating temperature */
real PCO2 = 1;                      /*[bar] Partial pressure of CO2 */
real rhoSteel = 7850;               /*[kg/m^3] Density of steel */
real MwSteel = 55.845e-3;           /*[kg/mol] Molecular weight of steel */

/*electrolyte*/
/*CO2 + H2O = H2CO3 and H2CO3 = H+ + HCO3-*/

/*electrode reaction
Fe = Fe2+ + 2e and 1/2H2 = H+ + e*/

++ Среда (сейчас H2O)

### out

Запуск задачи по 1 точке + объеденение результатов

Corrosion rate
+ optional: табличка