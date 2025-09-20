#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>  // ��ϣ�����map
#include <unordered_set>  // ��ϣ�������map
#include <algorithm>
#include <cmath>
#include <cctype>
#include <sstream>
#include <chrono>

using namespace std;
using namespace chrono;

// �ı�Ԥ����
string preprocessText(const string& text) {
    string result;
    for (size_t i = 0; i < text.size();) {
        // ����UTF-8����
        if ((unsigned char)text[i] >= 0xE0 && i + 2 < text.size()) {
            result += text.substr(i, 3);
            i += 3;
        }
        // ����ASCII�ַ�
        else {
            char c = text[i];
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') || c == ' ') {
                if (c >= 'A' && c <= 'Z') {
                    result += tolower(c);
                }
                else {
                    result += c;
                }
            }
            i++;
        }
    }
    return result;
}

// �ִ�
vector<string> splitText(const string& text) {
    vector<string> tokens;
    string currentWord;

    for (size_t i = 0; i < text.size();) {
        // �ж��Ƿ�Ϊ���֣�UTF-8����ĺ���ռ3���ֽڣ�
        if ((unsigned char)text[i] >= 0xE0 && i + 2 < text.size()) {
            string chineseChar = text.substr(i, 3);
            tokens.push_back(chineseChar);
            i += 3;
        }
        // ����ո���ΪӢ�ĵ��ʵķָ�����
        else if (text[i] == ' ') {
            if (!currentWord.empty()) {
                tokens.push_back(currentWord);
                currentWord.clear();
            }
            i++;
        }
        // ����Ӣ����ĸ������
        else {
            currentWord += text[i];
            i++;
        }
    }

    // ������һ������
    if (!currentWord.empty()) {
        tokens.push_back(currentWord);
    }

    return tokens;
}

// �����Ƶ��ʹ��unordered_map���map
unordered_map<string, int> calculateWordFrequency(const vector<string>& words) {
    unordered_map<string, int> freq;
    for (const string& word : words) {
        freq[word]++;
    }
    return freq;
}

// �����������ƶȣ�ʹ��unordered_set���map
double calculateCosineSimilarity(const unordered_map<string, int>& origFreq,
    const unordered_map<string, int>& copyFreq) {
    // �ù�ϣ�����ռ����д�
    unordered_set<string> allWords;
    for (const auto& pair : origFreq) allWords.insert(pair.first);
    for (const auto& pair : copyFreq) allWords.insert(pair.first);

    // ������ӣ����
    double dotProduct = 0.0;
    // ��������������ģ��
    double origMagnitude = 0.0, copyMagnitude = 0.0;

    for (const string& word : allWords) {
        int origCount = 0, copyCount = 0;

        auto origIt = origFreq.find(word);
        if (origIt != origFreq.end()) origCount = origIt->second;

        auto copyIt = copyFreq.find(word);
        if (copyIt != copyFreq.end()) copyCount = copyIt->second;

        dotProduct += origCount * copyCount;
        origMagnitude += origCount * origCount;
        copyMagnitude += copyCount * copyCount;
    }

    // ��ֹ�������
    if (origMagnitude == 0 || copyMagnitude == 0) {
        return 0.0;
    }

    // �����������ƶ�
    return dotProduct / (sqrt(origMagnitude) * sqrt(copyMagnitude));
}

// ��ȡ�ļ�����
string readFile(const string& filePath) {
    ifstream file(filePath, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "�����޷����ļ� '" << filePath << "'������·���Ƿ���ȷ��" << endl;
        exit(1);
    }

    string content;
    string line;
    while (getline(file, line)) {
        content += line + "\n";
    }

    file.close();
    return content;
}

// д�������ļ�
void writeResult(const string& filePath, double similarityPercent) {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "�����޷���������ļ� '" << filePath << "'������·���Ƿ���ȷ��" << endl;
        exit(1);
    }

    // д��ٷֱȸ�ʽ��������λС��
    file.precision(2);
    file << fixed << similarityPercent << "%" << endl;

    file.close();
}

int main(int argc, char* argv[]) {
    // ������
    if (argc != 4) {
        cerr << "�����������ṩ3���ļ�·������" << endl;
        cerr << "��ȷ�÷���ԭ���ļ�·�� ��Ϯ������·�� ������·��" << endl;
        return 1;
    }

    // ��¼��ʼʱ���
    auto startTime = high_resolution_clock::now();

    // �������в�����ȡ·��
    string origPath = argv[1];
    string copyPath = argv[2];
    string outputPath = argv[3];

    // ��ȡ�ļ�����
    string origText = readFile(origPath);
    string copyText = readFile(copyPath);

    // �������
    //cerr << "=== ԭ������ ===" << endl << origText << endl;
    //cerr << "=== ��Ϯ������ ===" << endl << copyText << endl;

    // Ԥ�����ı�
    string processedOrig = preprocessText(origText);
    string processedCopy = preprocessText(copyText);

    // �������
    //cerr << "=== Ԥ�����ԭ�� ===" << endl << processedOrig << endl;
    //cerr << "=== Ԥ�����Ϯ�� ===" << endl << processedCopy << endl;

    // �ִ�
    vector<string> origWords = splitText(processedOrig);
    vector<string> copyWords = splitText(processedCopy);

    // �������
    /*
    cerr << "=== ԭ�ķִʽ�� ===" << endl;
    for (const auto& word : origWords) cerr << word << "|";
    cerr << endl << "=== ��Ϯ��ִʽ�� ===" << endl;
    for (const auto& word : copyWords) cerr << word << "|";
    cerr << endl;
    */

    // �����Ƶ
    auto origFreq = calculateWordFrequency(origWords);
    auto copyFreq = calculateWordFrequency(copyWords);

    // ���������
    /*
    cerr << "=== ԭ�Ĵ�Ƶ ===" << endl;
    for (const auto& pair : origFreq) cerr << pair.first << ":" << pair.second << " ";
    cerr << endl << "=== ��Ϯ���Ƶ ===" << endl;
    for (const auto& pair : copyFreq) cerr << pair.first << ":" << pair.second << " ";
    cerr << endl;
    */

    // �������ƶ�
    double similarity = calculateCosineSimilarity(origFreq, copyFreq);
    double similarityPercent = similarity * 100;

    // ����̨������
    cout.precision(2);
    cout << fixed << similarityPercent << "%" << endl;

    // д�������ļ�
    writeResult(outputPath, similarityPercent);

    // ��¼����ʱ��㲢�����ʱ
    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(endTime - startTime);

    // �����ʱ��Ϣ
    cerr << "������ɣ��ظ��ʽ���ѱ�����: " << outputPath << endl;
    cerr << "���β����ܺ�ʱ: " << duration.count() << " ����" << endl;

    return 0;
}
