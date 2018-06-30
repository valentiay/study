#include <iostream>
#include <vector>

/**
 * Строит лексикографически минимальную z-функцию. Значение z-функции на первом символе должно быть
 * равно нулю.
 *
 * Z-блок - это непустая подстрока [ i, i + z[i] ).
 *
 * @param z вектор, к котором хранится z-функция.
 * @return лексикографически минимальная строка, построенная по z-функции.
 */
std::string buildStrByZ(const std::vector<size_t> & z) {
    // Хранит строку-ответ. Первый символ этой строки "a", иначе
    // строка не будет лексикографически минимальной
    std::string result = "a";
    // Хранит индексы символов, которые не могут быть дописаны после z-блока
    // (Иначе значение z-функции для этого блока увеличится)
    std::vector<size_t> used_indices;
    // true, если последний записанный символ был частью z-блока
    bool append_after_block = true;

    // Пробегаемся по z-функции
    size_t str_index = 1;
    while (str_index < z.size()) {
        // Если значение z-функции не ноль, копируем префикс в z-блок,
        // иначе дописываем в строку первую допустимую букву
        if (z[str_index] != 0) {
            used_indices.clear();
            size_t prefix_index = 0;
            size_t remaining_block_len = z[str_index];

            while (remaining_block_len > 0) {
                // Если попался z-блок с бОльшей правой границей, чем тот, который мы копируем,
                // начинаем копировать этот z-блок
                if (z[str_index] > remaining_block_len) {
                    remaining_block_len = z[str_index];
                    used_indices.push_back(z[str_index]);
                    prefix_index = 0;
                }

                // Если какой-то z-блок заканчивается на той же позиции, что и тот, который мы
                // копируем, записываем индекс символа, следующего за концом префикса, который
                // соответствует этому z-блоку
                if (z[str_index] == remaining_block_len) {
                    used_indices.push_back(z[str_index]);
                }

                result += result[prefix_index];

                prefix_index++;
                str_index++;
                remaining_block_len--;
            }

            append_after_block = true;
        } else {
            // Если мы записываем символ после z-блока, дописываем строку первым символом,
            // индекса которого нет в used_indices, иначе записываем "b", т.к. "a" сделает значение
            // z-функции единицей
            if (append_after_block) {
                std::vector<bool> used_chars(26, false);
                for (size_t n : used_indices) {
                    used_chars[result[n] - 'a'] = true;
                }

                char c = 'b';
                while (used_chars[c - 'a']) {
                    c++;
                }
                result += c;

                append_after_block = false;
            } else {
                result += 'b';
            }

            str_index++;
        }
    }

    return result;
}

int main() {
    freopen("input.txt", "r", stdin);

    // Чтение z-функции
    size_t x;
    std::vector<size_t> z;
    while(std::cin >> x) {
        z.push_back(x);
    }

    // Поиск строки
    z[0] = 0;
    std::cout << buildStrByZ(z);

    return 0;
}