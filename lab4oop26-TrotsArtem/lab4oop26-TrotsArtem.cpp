#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <ctime>   
#include <string>

// ============================================================================
// ЗАВДАННЯ 1.5. КЛАС VectorLong
// ============================================================================
class VectorLong {
private:
    long* data;
    unsigned int size;
    int codeError;
    static int objectCount;

public:
    VectorLong() : size(1), codeError(0) {
        data = new long{ 0 };
        objectCount++;
    }
    VectorLong(unsigned int s) : size(s), codeError(0) {
        data = new long[size] {0};
        objectCount++;
    }
    VectorLong(unsigned int s, long initVal) : size(s), codeError(0) {
        data = new long[size];
        for (unsigned int i = 0; i < size; i++) data[i] = initVal;
        objectCount++;
    }
    VectorLong(const VectorLong& other) : size(other.size), codeError(other.codeError) {
        data = new long[size];
        for (unsigned int i = 0; i < size; i++) data[i] = other.data[i];
        objectCount++;
    }
    ~VectorLong() {
        delete[] data;
        objectCount--;
    }

    static int getCount() { return objectCount; }
    unsigned int getSize() const { return size; }
    int getError() const { return codeError; }
    void clearError() { codeError = 0; }

    VectorLong& operator=(const VectorLong& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            codeError = other.codeError;
            data = new long[size];
            for (unsigned int i = 0; i < size; i++) data[i] = other.data[i];
        }
        return *this;
    }

    // Унарні оператори
    VectorLong& operator++() {
        for (unsigned int i = 0; i < size; i++) data[i]++;
        return *this;
    }
    VectorLong operator++(int) {
        VectorLong temp(*this);
        ++(*this);
        return temp;
    }
    VectorLong& operator--() {
        for (unsigned int i = 0; i < size; i++) data[i]--;
        return *this;
    }
    VectorLong operator--(int) {
        VectorLong temp(*this);
        --(*this);
        return temp;
    }
    bool operator!() const { return size != 0; }
    VectorLong operator~() const {
        VectorLong temp(size);
        for (unsigned int i = 0; i < size; i++) temp.data[i] = ~data[i];
        return temp;
    }
    VectorLong operator-() const {
        VectorLong temp(size);
        for (unsigned int i = 0; i < size; i++) temp.data[i] = -data[i];
        return temp;
    }

    // Присвоєння з операцією
    VectorLong& operator+=(const VectorLong& b) {
        unsigned int minSize = (size < b.size) ? size : b.size;
        for (unsigned int i = 0; i < minSize; i++) data[i] += b.data[i];
        return *this;
    }
    VectorLong& operator-=(const VectorLong& b) {
        unsigned int minSize = (size < b.size) ? size : b.size;
        for (unsigned int i = 0; i < minSize; i++) data[i] -= b.data[i];
        return *this;
    }
    VectorLong& operator*=(int b) {
        for (unsigned int i = 0; i < size; i++) data[i] *= b;
        return *this;
    }
    VectorLong& operator/=(int b) {
        if (b == 0) { codeError = 2; return *this; }
        for (unsigned int i = 0; i < size; i++) data[i] /= b;
        return *this;
    }
    VectorLong& operator%=(int b) {
        if (b == 0) { codeError = 2; return *this; }
        for (unsigned int i = 0; i < size; i++) data[i] %= b;
        return *this;
    }
    VectorLong& operator|=(const VectorLong& b) {
        unsigned int minSize = (size < b.size) ? size : b.size;
        for (unsigned int i = 0; i < minSize; i++) data[i] |= b.data[i];
        return *this;
    }
    VectorLong& operator^=(const VectorLong& b) {
        unsigned int minSize = (size < b.size) ? size : b.size;
        for (unsigned int i = 0; i < minSize; i++) data[i] ^= b.data[i];
        return *this;
    }
    VectorLong& operator&=(const VectorLong& b) {
        unsigned int minSize = (size < b.size) ? size : b.size;
        for (unsigned int i = 0; i < minSize; i++) data[i] &= b.data[i];
        return *this;
    }

    // Бінарні оператори
    VectorLong operator+(const VectorLong& b) const { VectorLong t(*this); return t += b; }
    VectorLong operator-(const VectorLong& b) const { VectorLong t(*this); return t -= b; }
    VectorLong operator*(int b) const { VectorLong t(*this); return t *= b; }
    VectorLong operator/(int b) const { VectorLong t(*this); return t /= b; }
    VectorLong operator%(int b) const { VectorLong t(*this); return t %= b; }
    VectorLong operator|(const VectorLong& b) const { VectorLong t(*this); return t |= b; }
    VectorLong operator^(const VectorLong& b) const { VectorLong t(*this); return t ^= b; }
    VectorLong operator&(const VectorLong& b) const { VectorLong t(*this); return t &= b; }

    long& operator[](unsigned int index) {
        if (index >= size) {
            codeError = 1;
            return data[size - 1];
        }
        return data[index];
    }
    long operator()(unsigned int index) const {
        if (index >= size) return 0;
        return data[index];
    }

    bool operator==(const VectorLong& b) const {
        if (size != b.size) return false;
        for (unsigned int i = 0; i < size; i++) if (data[i] != b.data[i]) return false;
        return true;
    }
    bool operator!=(const VectorLong& b) const { return !(*this == b); }

    friend std::ostream& operator<<(std::ostream& os, const VectorLong& v) {
        for (unsigned int i = 0; i < v.size; i++) os << v.data[i] << " ";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, VectorLong& v) {
        for (unsigned int i = 0; i < v.size; i++) is >> v.data[i];
        return is;
    }

    void randomize(long minVal = 1, long maxVal = 50) {
        for (unsigned int i = 0; i < size; i++) {
            data[i] = minVal + rand() % (maxVal - minVal + 1);
        }
    }
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        file >> size;
        delete[] data;
        data = new long[size];
        for (unsigned int i = 0; i < size; i++) file >> data[i];
        file.close();
        return true;
    }
};
int VectorLong::objectCount = 0;


// ============================================================================
// ЗАВДАННЯ 2.5. АСОЦІАТИВНИЙ КЛАС Електронна пошта та ПІБ
// ============================================================================
struct Association {
    std::string email;
    std::string pib;
};

class EmailPibMap {
private:
    Association* records;
    int capacity;
    int count;
    int CodeError;

public:
    EmailPibMap(int cap = 20) : capacity(cap), count(0), CodeError(0) {
        records = new Association[capacity];
    }
    ~EmailPibMap() { delete[] records; }

    int getCodeError() const { return CodeError; }
    void clearError() { CodeError = 0; }

    void add(const std::string& email, const std::string& pib) {
        if (count >= capacity) return;
        records[count++] = { email, pib };
    }

    std::string& operator[](const std::string& keyEmail) {
        for (int i = 0; i < count; i++) {
            if (records[i].email == keyEmail) return records[i].pib;
        }
        CodeError = 404;
        static std::string empty = "NOT_FOUND";
        return empty;
    }

    std::string operator()(const std::string& keyPib) {
        for (int i = 0; i < count; i++) {
            if (records[i].pib == keyPib) return records[i].email;
        }
        CodeError = 404;
        return "NOT_FOUND";
    }

    friend std::ostream& operator<<(std::ostream& os, const EmailPibMap& map) {
        for (int i = 0; i < map.count; i++) {
            os << map.records[i].email << " -> " << map.records[i].pib << "\n";
        }
        return os;
    }
    friend std::istream& operator>>(std::istream& is, EmailPibMap& map) {
        std::string em, pb;
        if (is >> em) {
            is.ignore();
            std::getline(is, pb);
            map.add(em, pb);
        }
        return is;
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        int numRecords;
        file >> numRecords;
        for (int i = 0; i < numRecords; i++) {
            std::string em, pb;
            file >> em;
            file.ignore();
            std::getline(file, pb);
            if (!em.empty() && !pb.empty()) add(em, pb);
        }
        file.close();
        return true;
    }
};


// ============================================================================
// ЗАВДАННЯ 3.5. КЛАС Matrix3D
// ============================================================================
class Vector3D {
public:
    double x, y, z;
    int State;
    static int count;
    Vector3D() : x(0), y(0), z(0), State(0) { count++; }
    Vector3D(double val) : x(val), y(val), z(val), State(0) { count++; }
    Vector3D(const Vector3D& o) : x(o.x), y(o.y), z(o.z), State(o.State) { count++; }
    ~Vector3D() { count--; }

    Vector3D& operator+=(const Vector3D& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vector3D& operator-=(const Vector3D& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vector3D& operator*=(double val) { x *= val; y *= val; z *= val; return *this; }
    Vector3D operator+(const Vector3D& o) const { Vector3D t(*this); return t += o; }
    Vector3D operator-(const Vector3D& o) const { Vector3D t(*this); return t -= o; }
    Vector3D operator*(double val) const { Vector3D t(*this); return t *= val; }

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Vector3D& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }
};
int Vector3D::count = 0;

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
    Matrix3D(int size, double initVal) : n(size), codeError(0) {
        PointArray = new Vector3D[n];
        for (int i = 0; i < n; i++) PointArray[i] = Vector3D(initVal);
        num_matrix++;
    }
    Matrix3D(const Matrix3D& other) : n(other.n), codeError(other.codeError) {
        PointArray = new Vector3D[n];
        for (int i = 0; i < n; i++) PointArray[i] = other.PointArray[i];
        num_matrix++;
    }
    ~Matrix3D() {
        delete[] PointArray;
        num_matrix--;
    }

    static int getMatrixCount() { return num_matrix; }

    Matrix3D& operator=(const Matrix3D& other) {
        if (this != &other) {
            delete[] PointArray;
            n = other.n;
            codeError = other.codeError;
            PointArray = new Vector3D[n];
            for (int i = 0; i < n; i++) PointArray[i] = other.PointArray[i];
        }
        return *this;
    }

    Matrix3D operator+(const Matrix3D& b) const {
        Matrix3D res(n);
        for (int i = 0; i < n; i++) res.PointArray[i] = PointArray[i] + b.PointArray[i];
        return res;
    }
    Matrix3D operator-(const Matrix3D& b) const {
        Matrix3D res(n);
        for (int i = 0; i < n; i++) res.PointArray[i] = PointArray[i] - b.PointArray[i];
        return res;
    }
    Matrix3D operator*(double val) const {
        Matrix3D res(n);
        for (int i = 0; i < n; i++) res.PointArray[i] = PointArray[i] * val;
        return res;
    }

    Vector3D& operator[](int index) {
        if (index >= n || index < 0) { codeError = 1; return PointArray[n - 1]; }
        return PointArray[index];
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix3D& m) {
        for (int i = 0; i < m.n; i++) os << m.PointArray[i] << "\n";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Matrix3D& m) {
        for (int i = 0; i < m.n; i++) is >> m.PointArray[i];
        return is;
    }

    void fillRandom() {
        for (int i = 0; i < n; i++) {
            PointArray[i].x = 1.0 + (rand() % 900) / 100.0;
            PointArray[i].y = 1.0 + (rand() % 900) / 100.0;
            PointArray[i].z = 1.0 + (rand() % 900) / 100.0;
        }
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        file >> n;
        delete[] PointArray;
        PointArray = new Vector3D[n];
        for (int i = 0; i < n; i++) file >> PointArray[i].x >> PointArray[i].y >> PointArray[i].z;
        file.close();
        return true;
    }
};
int Matrix3D::num_matrix = 0;


// Помічна функція для створення демонстраційних файлів на диску
void createDummyFiles() {
    std::ofstream f1("vector_data.txt");
    f1 << "5\n10 20 30 40 50\n";
    f1.close();

    std::ofstream f2("matrix_data.txt");
    f2 << "3\n1.1 1.2 1.3\n2.1 2.2 2.3\n3.1 3.2 3.3\n";
    f2.close();

    std::ofstream f3("map_data.txt");
    f3 << "2\nteacher@chnu.edu.ua\nЛазорик В.В.\ndekanat@chnu.edu.ua\nПетров П.П.\n";
    f3.close();
}

// ============================================================================
// ГОЛОВНЕ КОНСОЛЬНЕ МЕНЮ ДЛЯ РУЧНОГО ТЕСТУВАННЯ
// ============================================================================
int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    createDummyFiles(); // Створюємо файли автогенерацією, щоб користувачу було звідки читати

    int mainChoice = 0;
    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "                 ГОЛОВНЕ МЕНЮ                     \n";
        std::cout << "==================================================\n";
        std::cout << "1. Тестувати Завдання 1.5 (Вектори VectorLong)\n";
        std::cout << "2. Тестувати Завдання 2.5 (Асоціативний EmailPibMap)\n";
        std::cout << "3. Тестувати Завдання 3.5 (Матриці Matrix3D)\n";
        std::cout << "0. Вийти з програми\n";
        std::cout << "Ваш вибір: ";
        std::cin >> mainChoice;

        if (mainChoice == 0) break;

        switch (mainChoice) {
        case 1: {
            int inputType;
            std::cout << "\n--- ОБЕРІТЬ СПОСІБ ВВЕДЕННЯ ДЛЯ ВЕКТОРІВ ---\n";
            std::cout << "1. Вручну з клавіатури\n";
            std::cout << "2. Завантажити з файлу (vector_data.txt)\n";
            std::cout << "3. Використати датчик випадкових чисел\n";
            std::cout << "Вибір: ";
            std::cin >> inputType;

            VectorLong A(5), B(5), C(5);

            if (inputType == 1) {
                std::cout << "Введіть 5 елементів для вектора A: "; std::cin >> A;
                std::cout << "Введіть 5 елементів для вектора B: "; std::cin >> B;
                std::cout << "Введіть 5 елементів для вектора C: "; std::cin >> C;
            }
            else if (inputType == 2) {
                if (A.loadFromFile("vector_data.txt") && B.loadFromFile("vector_data.txt") && C.loadFromFile("vector_data.txt")) {
                    std::cout << "Дані успішно зчитано з файлу!\n";
                }
                else {
                    std::cout << "Помилка читання файлу! Застосовано нулі.\n";
                }
            }
            else {
                A.randomize(1, 10);
                B.randomize(2, 6);
                C.randomize(1, 3);
            }

            std::cout << "\nПоточні вектори:\n";
            std::cout << "A: " << A << "\nB: " << B << "\nC: " << C << "\n";

            // Обчислення виразу з понад 5 операцій
            VectorLong ResultV = (A + B) * 2 - (-C) + (A & B) - ++C;
            std::cout << "Результат векторного виразу: " << ResultV << "\n";
            std::cout << "Загальна кількість активних VectorLong: " << VectorLong::getCount() << "\n";
            break;
        }
        case 2: {
            EmailPibMap contacts(10);
            int inputType;
            std::cout << "\n--- ОБЕРІТЬ СПОСІБ ВВЕДЕННЯ ДЛЯ АСОЦІАТИВНОГО МАСИВУ ---\n";
            std::cout << "1. Вручну з клавіатури (1 запис)\n";
            std::cout << "2. Зчитати з файлу (map_data.txt)\n";
            std::cout << "Вибір: ";
            std::cin >> inputType;

            if (inputType == 1) {
                std::cout << "Введіть Email та ПІБ (через Enter):\n";
                std::cin >> contacts;
            }
            else {
                contacts.loadFromFile("map_data.txt");
                std::cout << "Записи зчитано з файлу.\n";
            }

            std::cout << "\nПоточна база контактів:\n" << contacts;

            // Ручний інтерактивний пошук
            int subChoice;
            std::cout << "\n1. Шукати ПІБ за допомогою Email (оператор [])\n";
            std::cout << "2. Шукати Email за допомогою ПІБ (оператор ())\n";
            std::cout << "Вибір: ";
            std::cin >> subChoice;

            std::cin.ignore();
            contacts.clearError();
            if (subChoice == 1) {
                std::string email;
                std::cout << "Введіть Email: ";
                std::cin >> email;
                std::string pib = contacts[email];
                std::cout << "Результат пошуку: " << pib << " (Код помилки: " << contacts.getCodeError() << ")\n";
            }
            else {
                std::string pib;
                std::cout << "Введіть ПІБ: ";
                std::getline(std::cin, pib);
                std::string email = contacts(pib);
                std::cout << "Результат пошуку: " << email << " (Код помилки: " << contacts.getCodeError() << ")\n";
            }
            break;
        }
        case 3: {
            int inputType;
            std::cout << "\n--- ОБЕРІТЬ СПОСІБ ВВЕДЕННЯ ДЛЯ МАТРИЦЬ ---\n";
            std::cout << "1. Вручну з клавіатури (розмір 2x3)\n";
            std::cout << "2. Зчитати з файлу (matrix_data.txt)\n";
            std::cout << "3. Заповнити випадковими числами\n";
            std::cout << "Вибір: ";
            std::cin >> inputType;

            Matrix3D M1(2), M2(2), M3(2);

            if (inputType == 1) {
                std::cout << "Введіть елементи (по 3 дробові числа для кожного рядка) для M1:\n"; std::cin >> M1;
                std::cout << "Введіть елементи для M2:\n"; std::cin >> M2;
                std::cout << "Введіть елементи для M3:\n"; std::cin >> M3;
            }
            else if (inputType == 2) {
                M1.loadFromFile("matrix_data.txt");
                M2.loadFromFile("matrix_data.txt");
                M3.loadFromFile("matrix_data.txt");
                std::cout << "Матриці зчитано з диска.\n";
            }
            else {
                M1.fillRandom();
                M2.fillRandom();
                M3.fillRandom();
            }

            std::cout << "\nМатриця M1:\n" << M1;
            std::cout << "Матриця M2:\n" << M2;

            // Обчислення виразу на 5 операцій
            Matrix3D ResultM = (M1 + M2) * 2.5 - M1 + M3 - M2;
            std::cout << "Результат матричного виразу:\n" << ResultM;
            std::cout << "Загальна кількість активних Matrix3D: " << Matrix3D::getMatrixCount() << "\n";
            break;
        }
        default:
            std::cout << "Неправильний пункт меню. Спробуйте ще раз.\n";
        }
    }

    std::cout << "\nПрограму завершено. Успішного захисту лабораторної роботи!\n";
    return 0;
}
