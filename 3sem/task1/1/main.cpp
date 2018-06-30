#include <iostream>
#include <vector>

/**
 * Находит все вхождения шаблона в текст
 *
 * @param patt шаблон
 * @param str текст
 * @return индексы вхождений шаблона в текст
 */
std::vector<size_t> findEntries(const std::string & patt, const std::string & str) {
    std::vector<size_t> pi(patt.length());
    pi[0] = 0;

    // Считаем префикс-функцию для шаблона
    for (int i = 1; i < patt.length(); i++) {
        size_t j = pi[i - 1];
        while (j > 0 && patt[i] != patt[j]) {
            j = pi[j - 1];
        }
        if (patt[i] == patt[j]) {
            pi[i] = j + 1;
        } else {
            pi[i] = 0;
        }
    }

    std::vector<size_t> entries;
    size_t prev_pi = 0;
    // Считаем префикс-функцию для каждого символа в тексте. Если она равна длине шаблона,
    // то мы нашли вхождение. Т.к. префикс функция не больше длины шаблона, нам не нужно
    // запоминать значения ПФ на тексте.
    for (size_t i = 0; i < str.size(); i++) {
        size_t j = prev_pi;
        while (j > 0 && str[i] != patt[j]) {
            j = pi[j - 1];
        }

        if (str[i] == patt[j]) {
            j++;
        } else {
            j = 0;
        }

        if (j == patt.length()) {
            entries.push_back(1 + i - patt.length());
        }
        prev_pi = j;
    }
    return entries;
}


int main() {
    std::string patt, str;
    std::cin >> patt;
    std::cin >> str;

    std::vector<size_t> entries = findEntries(patt, str);
    for (size_t i : entries) {
        std::cout << i << " ";
    }
    return 0;
}