#include <iostream>
#include <cstring>
#include <vector>

const size_t alphabetSize = 256;

/**
 * Возвращает количество различных подстрок строки.
 *
 * @param s строка.
 * @return количество различных подстрок.
 */
size_t distinctSubstrings(std::string s) {
    s.push_back('\0');
    // Вектор, хранящий перестановки циклических строк
    std::vector<size_t> permutations(s.length());

    // Сортируем подстроки из одного элемента подсчетом
    std::vector<size_t> count(alphabetSize, 0);
    for (char i : s) {
        count[i]++;
    }
    // Cчитаем начала групп
    for (size_t i = 1; i < alphabetSize; i++) {
        count[i] += count[i - 1];
    }
    // Расставляем строки в нужном порядке
    for (size_t i = 0; i < s.length(); i++) {
        permutations[--count[s[i]]] = i;
    }

    // Считаем количество классов эквивалентности для строк из двух символов.

    // Вектор, хранящий класс номер класса эквивалентности для циклической строки на i-й операции.
    // Порядок номеров соответсвует порядку строк
    std::vector<std::vector<size_t>> classes(1, std::vector<size_t>(s.length()));
    classes[0][permutations[0]] = 0;
    size_t classes_num = 1;
    for (size_t i = 1; i < s.length(); i++) {
        if (s[permutations[i]] != s[permutations[i - 1]]) ++classes_num;
        classes[0][permutations[i]] = classes_num - 1;
    }

    std::vector<long> new_permutations(s.length());
    // Повторяем алгоритм, пока длина подстрок ( 2^(j + 1) ) не превышает длины строки
    int j;
    for (j = 0; (1 << j) < s.length(); j++) {
        // Увеличиваем размер подстрок, при необходимости циклически переносим начало подстроки
        for (size_t i = 0; i < s.length(); i++) {
            new_permutations[i] = permutations[i] - (1 << j);
            if (new_permutations[i] < 0) {
                new_permutations[i] += s.length();
            }
        }
        // Зная пару (classes[i], classes[i + 2^j]), мы можем расположить строки в лексикографическом порядке.
        // Для этого используем LSD. Т.к. младший разряд отсортирован, сортируем по старшему
        count.assign(classes_num, 0);
        for (size_t i = 0; i < s.length(); i++) {
            count[classes[j][new_permutations[i]]]++;
        }
        for (size_t i = 1; i < classes_num; i++) {
            count[i] += count[i - 1];
        }
        for (long i = s.length() - 1; i >= 0; i--) {
            permutations[--count[classes[j][new_permutations[i]]]] = new_permutations[i];
        }

        // Считаем новые классы эквивалентности
        classes.emplace_back(s.length());
        classes[j + 1][permutations[0]] = 0;
        classes_num = 1;
        for (size_t i = 1; i < s.length(); i++) {
            size_t mid1 = (permutations[i] + (1 << j)) % s.length();
            size_t mid2 = (permutations[i - 1] + (1 << j)) % s.length();
            if (classes[j][permutations[i]] != classes[j][permutations[i - 1]] || classes[j][mid1] != classes[j][mid2]) {
                classes_num++;
            }
            classes[j + 1][permutations[i]] = classes_num - 1;
        }
    }

    std::vector<size_t> lcp(s.length() - 2, 0);
    for (size_t i = 1; i < s.length() - 1; i++) {
        size_t x = permutations[i];
        size_t y = permutations[i + 1];
        for (int k = j - 1; k >= 0; --k)
            if (classes[k][x] == classes[k][y]) {
                lcp[i - 1] += 1 << k;
                x += 1 << k;
                y += 1 << k;
            }
    }

    // Считаем количество различных подстрок
    size_t ans = 0;
    for (int i = 1; i < s.length(); i++) {
        ans += (s.length() - 1) - permutations[i];
    }
    for (int i = 0; i < s.length() - 2; i++) {
        ans -= lcp[i];
    }
    return ans;
}

int main() {
    std::string str;
    std::cin >> str;
    std::cout << distinctSubstrings(str);
    return 0;
}