#include <iostream>
#include <vector>

/**
 * Строит z-функцию по префикс-функции.
 *
 * Z-блок - это непустая подстрока [ i, i + z[i] ).
 *
 * @param pi вектор, хранящий префикс-функцию.
 * @return вектор, хранящий z-функцию, построенную по префикс-функции.
 */
std::vector<size_t> buildZByPi(const std::vector<size_t> & pi) {
    std::vector<size_t> z(pi.size());

    // Каждый суффикс, задаваемый префикс-функцией, записываем в соответствующую позицию
    // как максимальный префикс одновременно z-блока и всей строки.
    for (size_t i = 1; i < pi.size(); i++) {
        if (pi[i] > 0) {
            z[i - pi[i] + 1] = pi[i];
        }
    }

    z[0] = pi.size();
    size_t i = 1;
    while (i < pi.size()) {
        size_t t = i;
        // Если z-функция не равна нулю проходимся по z-блоку, иначе оставляем ее как есть.
        if (z[i] > 0) {
            for (size_t j = 1; j < z[i]; j++) {
                // Если находим значение z-функции бОльшее, чем текущее, выходим из цикла.
                if (z[i + j] > z[j]) {
                    break;
                }

                // Задаем значение z-функции в блоке, как минимум из z-функции
                // в префиксе и в суффиксе префикса
                z[i + j] = std::min(z[j], z[i] - j);
                t = i + j;
            }
        }
        i = t + 1;
    }

    z[0] = 0;
    return z;
}


/**
 * Строит префикс-функцию по z-функции.
 *
 * @param z вектор, хранящий z-функцию.
 * @return вектор, хранящий префикс-функцию, построенную по z-функции.
 */
std::vector<size_t> buildPiByZ(const std::vector<size_t> & z) {
    std::vector<size_t> pi(z.size(), 0);

    // Проходимся по индексам строки.
    for (size_t i = 1; i < z.size(); i++) {
        // Идем назад от конца z-блока, пока не найдем инициализированный элемент, и
        // инициализируем длиной суффикса все позиции, которые проходим.
        for (size_t j = z[i] - 1; j >= 0; j--) {
            if (pi[i + j] > 0) {
                break;
            }  else {
                pi[i + j] = j + 1;
            }
        }
    }
    return pi;
}


/**
 * Строит z-функцию по строке.
 *
 * Z-блок - это непустая подстрока [ i, i + z[i] ).
 *
 * @param str строка.
 * @return вектор size_t, содержащий z-функцию.
 */
std::vector<size_t> buildZByStr(const std::string & str) {
    std::vector<size_t> z(str.length());
    z[0] = 0;

    // Границы z-блока с максимальной правой границей
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 1; i < str.length(); i++) {
        // Выбираем из трех случаев:
        // 1) i > right, j = 0 - пробегаемся от начала строки и ищем первый символ,
        // который отличается от соответсвующего символа в z-блоке;
        // 2) i <= right и right >= z[i - left] + i, j = right - i - пробегаемся по z-блоку
        // от позиции right и ищем первое несовпадение;
        // 3) i <= right и right < z[i - left] + i, j = z[i - left] - в этом случае значение
        // z-функции равно z[i - left].
        size_t j = (size_t)std::max(0, std::min((int)(right - i), (int)z[i - left]));

        while (i + j < str.size() && str[j] == str[i + j]) {
            j++;
        }

        // Перемещаем right, если необходимо
        if (i + j >= right) {
            left = i;
            right = i + j;
        }

        z[i] = j;
    }
    return z;
}


/**
 * Строит префикс-функцию по строке.
 *
 * @param str строка.
 * @return вектор size_t, содержащий префикс-функцию.
 */
std::vector<size_t> buildPiByStr(const std::string & str) {
    // Вектор, хранящий префикс-функцию
    std::vector<size_t> pi(str.size());
    pi[0] = 0;

    // Проходимся по каждому символу
    for (size_t i = 1; i < str.length(); i++) {
        // Рассматриваем префиксы меньшей длины, "спускаясь" по значениям префикс-функции,
        // пока не найдем тот, который был бы суффиксом или не достигнем пустой строки.
        size_t k = pi[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = pi[k - 1];
        }
        if (str[i] == str[k]) {
            pi[i] = k + 1;
        } else {
            pi[i] = k;
        }
    }

    return pi;
}


/**
 * Строит лексикографически минимальную z-функцию. Значение z-функции на первом символе должно быть
 * равно нулю.
 *
 * Z-блок - это непустая подстрока [ i, i + z[i] ).
 *
 * @param z вектор, в котором хранится z-функция.
 * @return лексикографически минимальная строка, построенная по z-функции.
 */
std::string buildStrByZ(const std::vector<size_t> & z) {
    if (z.size() == 0) {
        return "";
    }
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


/**
 * Строит лексикографически минимальную строку по префикс-функции.
 *
 * @param pi вектор, в котором хранится префикс-функция.
 * @return лексикографически минимальная строка, построенная по префикс-функции.
 */
std::string buildStrByPi(const std::vector<size_t> & pi) {
    if (pi.size() == 0) {
        return "";
    }
    // Строка, в которой хранится результат. Первый символ - "a", т.к. иначе строка не будет
    // лексикографически минимальной.
    std::string result = "a";

    // Проходим по каждому символу в строке.
    for (size_t i = 1; i < pi.size(); i++) {
        if (pi[i] != 0) {
            // Если префикс-функция не ноль, дописываем символ,
            // который стоит на нужном месте в префиксе строки.
            result += result[pi[i] - 1];
        } else {
            // Ищем символы, которые мы не можем использовать и берем первый, который можно.
            // i-й элемент true, если i-я буква алфавита была использована в префиксе.
            std::vector<bool> used(26, false);

            // "Спускаемся" к началу строки, как при построении префикс-функции и
            // запоминаем символы, которые нам встретились.
            size_t k = pi[i - 1];
            while (k > 0) {
                used[result[k] - 'a'] = true;
                k = pi[k - 1];
            }

            // Ищем первый символ, который не был встречен при "спуске" к началу строки
            char c = 'b';
            while (used[c - 'a']){
                c++;
            };
            result += c;
        }
    }
    return result;
}


int main() {
    freopen("input.txt", "r", stdin);
    size_t x;
    // Чтение префикс-функции
    std::vector<size_t> pi;
    while (std::cin >> x) {
        pi.push_back(x);
    }

    // Поиск строки
    std::cout << buildStrByPi(pi);
}