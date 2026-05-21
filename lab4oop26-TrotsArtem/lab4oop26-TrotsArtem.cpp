#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <clocale>

// ============================================================================
// ЗАВДАННЯ 1. Клас VectorLong (Варіант 1.5)
// ============================================================================
class VectorLong {
private:
    long* data;
    size_t size;
    int codeError;
    static int object_count;

public:
    VectorLong() : size(1), codeError(0) {
        data = new long[1] {0};
        object_count++;
    }

    VectorLong(size_t s) : size(s), codeError(0) {
        data = new long[size] {0};
        object_count++;
    }

    VectorLong(const VectorLong& other) : size(other.size), codeError(other.codeError) {
        data = new long[size];
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        object_count++;
    }

    ~VectorLong() {
        delete[] data;
        object_count--;
    }

    static int getObjectCount() { return object_count; }
    int getCodeError() const { return codeError; }

    long& operator[](size_t index) {
        if (index >= size) { codeError = 1; return data[size - 1]; }
        return data[index];
    }

    void operator()() {
        std::cout << "[VectorLong Стан]: розмір = " << size << ", codeError = " << codeError << std::endl;
    }

    VectorLong operator~() const {
        VectorLong temp(*this);
        for (size_t i = 0; i < size; ++i) temp.data[i] = ~data[i];
        return temp;
    }

    VectorLong& operator+=(const VectorLong& other) {
        size_t min_s = std::min(size, other.size);
        for (size_t i = 0; i < min_s; ++i) data[i] += other.data[i];
        return *this;
    }

    VectorLong& operator-=(const VectorLong& other) {
        size_t min_s = std::min(size, other.size);
        for (size_t i = 0; i < min_s; ++i) data[i] -= other.data[i];
        return *this;
    }

    VectorLong& operator*=(long num) {
        for (size_t i = 0; i < size; ++i) data[i] *= num;
        return *this;
    }

    VectorLong& operator/=(long num) {
        if (num == 0) { codeError = 2; return *this; }
        for (size_t i = 0; i < size; ++i) data[i] /= num;
        return *this;
    }

    VectorLong operator+(const VectorLong& other) const { VectorLong temp(*this); return temp += other; }
    VectorLong operator-(const VectorLong& other) const { VectorLong temp(*this); return temp -= other; }
    VectorLong operator*(long num) const { VectorLong temp(*this); return temp *= num; }
    VectorLong operator/(long num) const { VectorLong temp(*this); return temp /= num; }

    friend std::ostream& operator<<(std::ostream& os, const VectorLong& v) {
        for (size_t i = 0; i < v.size; ++i) os << v.data[i] << " ";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, VectorLong& v) {
        for (size_t i = 0; i < v.size; ++i) is >> v.data[i];
        return is;
    }

    void fillRandom(size_t s) {
        delete[] data; size = s; data = new long[size];
        for (size_t i = 0; i < size; ++i) data[i] = std::rand() % 50 + 1;
    }

    void fillFromKeyboard(size_t s) {
        delete[] data; size = s; data = new long[size];
        std::cout << "Введіть " << size << " чисел через пробіл: ";
        for (size_t i = 0; i < size; ++i) std::cin >> data[i];
    }

    bool fillFromFile(size_t s, const std::string& filename) {
        delete[] data; size = s; data = new long[size];
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        for (size_t i = 0; i < size && !file.eof(); ++i) file >> data[i];
        file.close();
        return true;
    }
};
int VectorLong::object_count = 0;


// ============================================================================
// ЗАВДАННЯ 2. Асоціативний клас (Електронна пошта -> ПІБ) (Варіант 2.5)
// ============================================================================
struct AccountPair {
    std::string email;
    std::string fullName;
};

class EmailMap {
private:
    std::vector<AccountPair> storage;
    int codeError;
    std::string fallback;

public:
    EmailMap() : codeError(0), fallback("") {}

    int getCodeError() const { return codeError; }
    void clearError() { codeError = 0; }

    void add(const std::string& email, const std::string& name) {
        for (auto& pair : storage) {
            if (pair.email == email) { pair.fullName = name; return; }
        }
        storage.push_back({ email, name });
    }

    std::string& operator[](const std::string& email) {
        for (auto& pair : storage) {
            if (pair.email == email) return pair.fullName;
        }
        codeError = 404;
        fallback = "";
        return fallback;
    }

    std::string operator()(const std::string& email) {
        for (const auto& pair : storage) {
            if (pair.email == email) return pair.fullName;
        }
        codeError = 404;
        return "NOT_FOUND";
    }

    friend std::ostream& operator<<(std::ostream& os, const EmailMap& map) {
        if (map.storage.empty()) os << "  [База порожня]\n";
        for (const auto& pair : map.storage) {
            os << "  " << pair.email << " --> " << pair.fullName << "\n";
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, EmailMap& map) {
        std::string em, name;
        std::cout << "  Введіть email: "; is >> em;
        is.ignore();
        std::cout << "  Введіть ПІБ: "; std::getline(is, name);
        map.add(em, name);
        return is;
    }

    bool fillFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string em, name;
        while (file >> em) {
            file.ignore();
            std::getline(file, name);
            if (!em.empty() && !name.empty()) add(em, name);
        }
        file.close();
        return true;
    }
};


// ============================================================================
// ЗАВДАННЯ 3. Матриці на основі Vector3D (Варіант 3.5)
// ============================================================================
class Vector3D {
public:
    int x, y, z;
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(int val) : x(val), y(val), z(val) {}
    Vector3D(int x, int y, int z) : x(x), y(y), z(z) {}

    Vector3D& operator+=(const Vector3D& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vector3D& operator-=(const Vector3D& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vector3D& operator*=(int val) { x *= val; y *= val; z *= val; return *this; }

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        os << "(" << v.x << "," << v.y << "," << v.z << ")";
        return os;
    }
};

class Matrix3D {
protected:
    Vector3D* PointArray;
    int n;
    int codeError;
    static int num_matrix;

public:
    Matrix3D() : PointArray(nullptr), n(0), codeError(0) { num_matrix++; }
    Matrix3D(int size) : n(size), codeError(0) {
        PointArray = new Vector3D[n];
        num_matrix++;
    }
    Matrix3D(const Matrix3D& other) : n(other.n), codeError(other.codeError) {
        PointArray = new Vector3D[n];
        for (int i = 0; i < n; ++i) PointArray[i] = other.PointArray[i];
        num_matrix++;
    }
    ~Matrix3D() { delete[] PointArray; num_matrix--; }

    static int getNumMatrix() { return num_matrix; }

    Matrix3D operator-() const {
        Matrix3D temp(*this);
        for (int i = 0; i < n; ++i) { temp.PointArray[i].x *= -1; temp.PointArray[i].y *= -1; temp.PointArray[i].z *= -1; }
        return temp;
    }

    Matrix3D& operator=(const Matrix3D& other) {
        if (this == &other) return *this;
        delete[] PointArray; n = other.n; codeError = other.codeError;
        PointArray = new Vector3D[n];
        for (int i = 0; i < n; ++i) PointArray[i] = other.PointArray[i];
        return *this;
    }

    Matrix3D operator+(const Matrix3D& other) const {
        Matrix3D temp(*this); int min_n = std::min(n, other.n);
        for (int i = 0; i < min_n; ++i) temp.PointArray[i] += other.PointArray[i];
        return temp;
    }

    Matrix3D operator-(const Matrix3D& other) const {
        Matrix3D temp(*this); int min_n = std::min(n, other.n);
        for (int i = 0; i < min_n; ++i) temp.PointArray[i] -= other.PointArray[i];
        return temp;
    }

    Matrix3D operator*(int scalar) const {
        Matrix3D temp(*this);
        for (int i = 0; i < n; ++i) temp.PointArray[i] *= scalar;
        return temp;
    }

    Matrix3D operator*(double scalar) const {
        Matrix3D temp(*this);
        for (int i = 0; i < n; ++i) {
            temp.PointArray[i].x = static_cast<int>(temp.PointArray[i].x * scalar);
            temp.PointArray[i].y = static_cast<int>(temp.PointArray[i].y * scalar);
            temp.PointArray[i].z = static_cast<int>(temp.PointArray[i].z * scalar);
        }
        return temp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix3D& m) {
        for (int i = 0; i < m.n; ++i) os << m.PointArray[i] << " ";
        return os;
    }

    void fillRandom(int size) {
        delete[] PointArray; n = size; PointArray = new Vector3D[n];
        for (int i = 0; i < n; ++i) PointArray[i] = Vector3D(std::rand() % 10, std::rand() % 10, std::rand() % 10);
    }

    void fillFromKeyboard(int size) {
        delete[] PointArray; n = size; PointArray = new Vector3D[n];
        std::cout << "Введіть " << size << " тривимірних точок (формат: x y z):\n";
        for (int i = 0; i < n; ++i) {
            std::cout << "  Точка [" << i << "]: ";
            std::cin >> PointArray[i].x >> PointArray[i].y >> PointArray[i].z;
        }
    }

    bool fillFromFile(int size, const std::string& filename) {
        delete[] PointArray; n = size; PointArray = new Vector3D[n];
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        for (int i = 0; i < n && !file.eof(); ++i) {
            file >> PointArray[i].x >> PointArray[i].y >> PointArray[i].z;
        }
        file.close();
        return true;
    }
};
int Matrix3D::num_matrix = 0;


// Helper функція вибору типу заповнення даних
int chooseInputMethod() {
    int choice;
    std::cout << "Оберіть метод введення даних:\n";
    std::cout << "  1 - З клавіатури (Вручну)\n";
    std::cout << "  2 - З файлу (Потрібні готові файли)\n";
    std::cout << "  3 - Датчик випадкових чисел (Автогенерація)\n";
    std::cout << "Ваш вибір: ";
    std::cin >> choice;
    return choice;
}

// ============================================================================
// ГОЛОВНА КОНСОЛЬНА ПРОГРАМА З МЕНЮ
// ============================================================================
int main() {
    std::setlocale(LC_ALL, "uk_UA.UTF-8");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Авто-генерація заготовок файлів (про всяк випадок, якщо оберуть файл)
    {
        std::ofstream f1("vector_data.txt"); f1 << "5 10 15 20 25"; f1.close();
        std::ofstream f2("emails.txt"); f2 << "shevchenko@knu.ua\nШевченко Тарас Григорович\nfranko@knu.ua\nФранко Іван Якович\n"; f2.close();
        std::ofstream f3("matrix_data.txt"); f3 << "1 2 3\n4 5 6\n7 8 9\n"; f3.close();
    }

    int taskChoice = 0;
    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "          ГОЛОВНЕ МЕНЮ ЛАБОРАТОРНОЇ РОБОТИ        \n";
        std::cout << "==================================================\n";
        std::cout << "1. Завдання 1: Робота з Векторами (VectorLong)\n";
        std::cout << "2. Завдання 2: Асоціативний Словник (EmailMap)\n";
        std::cout << "3. Завдання 3: Робота з Матрицями (Matrix3D)\n";
        std::cout << "0. Вихід з програми\n";
        std::cout << "Оберіть номер завдання: ";
        std::cin >> taskChoice;

        if (taskChoice == 0) break;

        switch (taskChoice) {
        case 1: {
            std::cout << "\n--- [ЗАВДАННЯ 1: Векторні вирази з 5+ операцій] ---\n";
            int method = chooseInputMethod();
            VectorLong v1, v2, v3, v4, v5;
            size_t size = 5;

            if (method == 1) {
                std::cout << "Введення для v1: "; v1.fillFromKeyboard(size);
                std::cout << "Введення для v2: "; v2.fillFromKeyboard(size);
                std::cout << "Введення для v3: "; v3.fillFromKeyboard(size);
                std::cout << "Введення для v4: "; v4.fillFromKeyboard(size);
                std::cout << "Введення для v5: "; v5.fillFromKeyboard(size);
            }
            else if (method == 2) {
                if (!v1.fillFromFile(size, "vector_data.txt")) {
                    std::cout << "Помилка відкриття файлу! Авто-перемикання на рандом.\n";
                    v1.fillRandom(size); v2.fillRandom(size); v3.fillRandom(size); v4.fillRandom(size); v5.fillRandom(size);
                }
                else {
                    v2.fillFromFile(size, "vector_data.txt"); v3.fillFromFile(size, "vector_data.txt");
                    v4.fillFromFile(size, "vector_data.txt"); v5.fillFromFile(size, "vector_data.txt");
                }
            }
            else {
                v1.fillRandom(size); v2.fillRandom(size); v3.fillRandom(size); v4.fillRandom(size); v5.fillRandom(size);
            }

            std::cout << "\nЗчитані вектори:\n";
            std::cout << "v1: " << v1 << "\nv2: " << v2 << "\nv3: " << v3 << "\n";

            // Векторний вираз (6 операцій: +, *, -, /, ~, +)
            VectorLong v_res = v1 + v2 * 2 - v3 / 2 + (~v4) + v5;
            std::cout << "\nРезультат виразу (v1 + v2 * 2 - v3 / 2 + (~v4) + v5):\n" << v_res << "\n";
            v_res(); // Виклик оператора ()
            std::cout << "Активних об'єктів у пам'яті: " << VectorLong::getObjectCount() << "\n";
            break;
        }
        case 2: {
            std::cout << "\n--- [ЗАВДАННЯ 2: Асоціативний масив Email -> ПІБ] ---\n";
            EmailMap db;
            int method = chooseInputMethod();

            if (method == 1) {
                int count;
                std::cout << "Скільки записів ви хочете ввести? "; std::cin >> count;
                for (int i = 0; i < count; ++i) {
                    std::cin >> db; // Використовує дружній оператор >>
                }
            }
            else if (method == 2) {
                db.fillFromFile("emails.txt");
                std::cout << "Дані успішно імпортовано з файлу 'emails.txt'\n";
            }
            else {
                db.add("random.user1@knu.ua", "Іванов Іван Іванович");
                db.add("test.student@knu.ua", "Петров Петро Петрович");
                std::cout << "Базу автоматично заповнено тестовими рандомними даними.\n";
            }

            std::cout << "\nПоточний стан бази:\n" << db;

            // Пошук
            std::string searchEmail;
            std::cout << "\nВведіть email для пошуку в базі: ";
            std::cin >> searchEmail;

            std::cout << "Пошук через оператор [ ]: " << db[searchEmail] << "\n";
            if (db.getCodeError() == 404) {
                std::cout << "  (!) Помилка: Ключ відсутній. CodeError = 404\n";
                db.clearError();
            }

            std::cout << "Альтернативний пошук через ( ): " << db(searchEmail) << "\n";
            break;
        }
        case 3: {
            std::cout << "\n--- [ЗАВДАННЯ 3: Матричні вирази з 5+ операцій] ---\n";
            int method = chooseInputMethod();
            Matrix3D m1, m2, m3;
            int size = 3;

            if (method == 1) {
                std::cout << "Введення матриці m1:\n"; m1.fillFromKeyboard(size);
                std::cout << "Введення матриці m2:\n"; m2.fillFromKeyboard(size);
                std::cout << "Введення матриці m3:\n"; m3.fillFromKeyboard(size);
            }
            else if (method == 2) {
                m1.fillFromFile(size, "matrix_data.txt");
                m2.fillFromFile(size, "matrix_data.txt");
                m3.fillFromFile(size, "matrix_data.txt");
            }
            else {
                m1.fillRandom(size); m2.fillRandom(size); m3.fillRandom(size);
            }

            std::cout << "\nЗчитані матриці точок:\n";
            std::cout << "m1: " << m1 << "\nm2: " << m2 << "\nm3: " << m3 << "\n";

            // Матричний вираз (5 операцій: *, +, -, унарний мінус, * на double)
            Matrix3D m_res = m1 * 2 + m2 - (-m3) * 1.5;
            std::cout << "\nРезультат виразу (m1 * 2 + m2 - (-m3) * 1.5):\n" << m_res << "\n";
            std::cout << "Всього створено об'єктів матриць: " << Matrix3D::getNumMatrix() << "\n";
            break;
        }
        default:
            std::cout << "Неправильний вибір! Спробуйте ще раз.\n";
        }
    }

    std::cout << "\nПрограму успішно завершено. Гарного дня!\n";
    return 0;
}