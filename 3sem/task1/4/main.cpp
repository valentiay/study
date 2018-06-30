#include <iostream>
#include <vector>

// a-z
int const ALPHABET_SIZE = 26;

class AhoCorasickMachine {
public:
    /**
     * Создает автомат Ахо-Корасик, по подшаблонам без "вопросов" из маски.
     *
     * @param mask маска, состоящая из символов a-z и символов ?
     */
    AhoCorasickMachine(const std::string & mask);

    /**
     * Ищет вхождения маски в тексте.
     *
     * @param text текст, в котором нужно найти вхождения.
     * @return вектор, состоящий из индексов вхождений.
     */
    std::vector<int> findMatches(const std::string & text);

private:
    /** Структура, соответвующая вершине бора */
    struct Vertex;

    /**
     * Добавляет шаблон в бор. Использует поле #mask_
     *
     * @param submaskPos пара <int, int> начало и конец строки включительно.
     * @param pattIndex номер шаблона.
     */
    void addPattern(const std::pair<int, int> & submaskPos, int pattIndex);

    /**
     * Находит подшаблоны без символов ? в маске. Записывает результат в поле #submask_poitions_
     *
     * @param mask маска.
     */
    void findSubmaskPositions(const std::string & mask);

    int getSuffixLink(int index);
    int getLink(int index, char character);

    /** Маска, для которой ищутся вхождения */
    std::string mask_;
    /** Вектор вершин бора */
    std::vector<Vertex> bohr_;
    /** Вектор пар начало-конец подшаблона без символов ? в маске */
    std::vector<std::pair<int, int>> submask_positions_;
};

/**************************************************************************************************/

int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;

    AhoCorasickMachine ahoCorasickMachine(pattern);

    std::vector<int> entries = ahoCorasickMachine.findMatches(text);
    for (int i = 0; i < entries.size(); i++) {
        std::cout << entries[i] << " ";
    }

    return 0;
}

/**************************************************************************************************/

struct AhoCorasickMachine::Vertex {
    /**
     * Создает вершину бора без исходящих вершин и суффиксных сыллок, не являющуюся концом шаблона.
     *
     * @param parent индекс родителя.
     * @param parentChar символ на ребре родитель - данная вершина.
     */
    Vertex(int parent, char parentChar);

    /** Исходящие вершины */
    int edges[ALPHABET_SIZE];
    /** Переходы автомата из данной вершины */
    int fastMove[ALPHABET_SIZE];
    /** Суффиксная ссылка из данной вершины */
    int suffixLink;
    /** Родительская вершина */
    int parent;
    /** Символ на ребре родитель - данная вершина */
    char parentChar;
    /** Флаг, является ли вершина концом шаблона */
    bool isPattern;
    /** Шаблоны, с которыми связана данная вершина */
    std::vector<int> indicesOfPattern;
};



AhoCorasickMachine::Vertex::Vertex(int parent, char parentChar)
        : parentChar(parentChar)
        , parent(parent)
        , suffixLink(-1)
        , isPattern(false)
{
    for (int j = 0; j < ALPHABET_SIZE; j++) {
        edges[j] = -1;
        fastMove[j] = -1;
    }
}

/**************************************************************************************************/


AhoCorasickMachine::AhoCorasickMachine(const std::string & mask)
        : bohr_(1, Vertex(0, -1))
        , mask_(mask)
{
    bohr_[0].suffixLink = 0;

    findSubmaskPositions(mask);

    for (int i = 0; i < submask_positions_.size(); i++) {
        addPattern(submask_positions_[i], i);
    }
}



void AhoCorasickMachine::addPattern(const std::pair<int, int> & submaskPos, int pattIndex) {
    int current_vertex = 0;
    for (int i = submaskPos.first; i <= submaskPos.second; i++) {
        char character = mask_[i] - 'a';
        if (bohr_[current_vertex].edges[character] == -1) {
            bohr_.push_back(Vertex(current_vertex, character));
            bohr_[current_vertex].edges[character] = (int)(bohr_.size() - 1);
        }
        current_vertex = bohr_[current_vertex].edges[character];
    }
    bohr_[current_vertex].isPattern = true;
    bohr_[current_vertex].indicesOfPattern.push_back(pattIndex);
}



int AhoCorasickMachine::getSuffixLink(int index) {
    if (bohr_[index].suffixLink == -1) {
        // Если ссылка не определена...
        if (bohr_[index].parent == 0) {
            // ...и вершина - сын корня - задаем ссылку, как сслыку на корень
            bohr_[index].suffixLink = 0;
        } else {
            // иначе рекурсивно ищем ее
            bohr_[index].suffixLink =
                    getLink(getSuffixLink(bohr_[index].parent), bohr_[index].parentChar);
        }
    }
    return bohr_[index].suffixLink;
}



void AhoCorasickMachine::findSubmaskPositions(const std::string &mask) {
    std::pair<int, int> current_submask_pos;
    if (isalpha(mask[0])) {
        current_submask_pos.first = 0;
    }
    if (mask_[1] == '?' && isalpha(mask_[0])) {
        current_submask_pos.second = 0;
        submask_positions_.push_back(current_submask_pos);
    }

    for (int i = 1; i < mask_.length() - 1; i++) {
        if (mask[i - 1] == '?' && isalpha(mask[i])) {
            current_submask_pos.first = i;
        }
        if (mask[i + 1] == '?' && isalpha(mask[i])) {
            current_submask_pos.second = i;
            submask_positions_.push_back(current_submask_pos);
        }
    }

    if (mask_[mask.length() - 2] == '?' && isalpha(mask[mask.length() - 1])) {
        current_submask_pos.first = (int)mask.length() - 1;
    }
    if (isalpha(mask_[mask_.length() - 1])) {
        current_submask_pos.second = (int)(mask.length() - 1);
        submask_positions_.push_back(current_submask_pos);
    }
}



int AhoCorasickMachine::getLink(int index, char character) {
    if (bohr_[index].fastMove[character] == -1) {
        if (bohr_[index].edges[character] != -1) {
            bohr_[index].fastMove[character] = bohr_[index].edges[character];
        } else if (index == 0) {
            bohr_[index].fastMove[character] = 0;
        } else {
            bohr_[index].fastMove[character] = getLink(getSuffixLink(index), character);
        }
    }
    return bohr_[index].fastMove[character];
}



std::vector<int> AhoCorasickMachine::findMatches(const std::string & text) {
    std::vector<int> entries(text.length());
    int v = 0;
    // Ищем вхождения всех подшаблонов и увеличиваем счетчик entries в индексе,
    // соответвующем началу маски.
    for (int i = 0; i < text.length(); i++) {
        v = getLink(v, text[i] - 'a');
        int u = v;
        do {
            if (bohr_[u].isPattern) {
                for (int index = 0; index < bohr_[u].indicesOfPattern.size(); index++) {

                    int startIndex = i - submask_positions_[bohr_[u].indicesOfPattern[index]].second
                                     + submask_positions_[bohr_[u].indicesOfPattern[index]].first;

                    if ((startIndex - submask_positions_[bohr_[u].indicesOfPattern[index]].first >=
                         0) &&
                        (startIndex - submask_positions_[bohr_[u].indicesOfPattern[index]].first +
                         mask_.length() - 1 < text.length())) {
                        entries[startIndex -
                                submask_positions_[bohr_[u].indicesOfPattern[index]].first]++;
                    }
                }
            }
            u = getSuffixLink(u);
        } while (u != 0);
    }

    // Если индекс равен количеству подшаблонов, он является вхождением подстроки
    std::vector<int> result;
    for (int i = 0; i < entries.size(); i++) {
        if (entries[i] == submask_positions_.size()) {
            result.push_back(i);
        }
    }
    return result;
}
