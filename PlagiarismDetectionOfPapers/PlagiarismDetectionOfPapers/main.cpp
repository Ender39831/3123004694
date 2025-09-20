#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <sstream>

using namespace std;

// �ı�Ԥ����ȥ�������ţ��������֡���ĸ������
string preprocessText(const string& text) {
    string result;
    for (size_t i = 0; i < text.size();) {
        // ����UTF-8���֣�3�ֽڣ�
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

// �ִʣ������İ������ַ��ָ��Ӣ�İ��ո�ָ�
vector<string> splitText(const string& text) {
    vector<string> tokens;
    string currentWord;

    for (size_t i = 0; i < text.size();) {
        // �ж��Ƿ�Ϊ����
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

// �����Ƶ
map<string, int> calculateWordFrequency(const vector<string>& words) {
    map<string, int> freq;
    for (const string& word : words) {
        freq[word]++;
    }
    return freq;
}

// �����������ƶ�
double calculateCosineSimilarity(const map<string, int>& origFreq, const map<string, int>& copyFreq) {
    // �ռ�����Ψһ��
    map<string, bool> allWords;
    for (const auto& pair : origFreq) allWords[pair.first] = true;
    for (const auto& pair : copyFreq) allWords[pair.first] = true;

    // ������ӣ����
    double dotProduct = 0.0;
    // ��������������ģ��
    double origMagnitude = 0.0, copyMagnitude = 0.0;

    for (const auto& pair : allWords) {
        const string& word = pair.first;
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
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        content.append(buffer, sizeof(buffer));
    }
    // ��ȡʣ������
    content.append(buffer, file.gcount());

    file.close();
    return content;
}

int main(int argc, char* argv[]) {
    // ������
    if (argc != 4) {
        cerr << "�����������ṩ3���ļ�·������" << endl;
        cerr << "��ȷ�÷���ԭ���ļ�·�� ��Ϯ������·�� ������·��" << endl;
        return 1;
    }

    // �������в�����ȡ·��
    string origPath = argv[1];
    string copyPath = argv[2];
    string outputPath = argv[3];

    // ��ȡ�ļ�����
    string origText = readFile(origPath);
    string copyText = readFile(copyPath);

    // ����������鿴ԭʼ����
    cerr << "=== ԭ������ ===" << endl << origText << endl;
    cerr << "=== ��Ϯ������ ===" << endl << copyText << endl;

    // Ԥ�����ı�
    string processedOrig = preprocessText(origText);
    string processedCopy = preprocessText(copyText);

    // ����������鿴Ԥ������
    cerr << "=== Ԥ�����ԭ�� ===" << endl << processedOrig << endl;
    cerr << "=== Ԥ�����Ϯ�� ===" << endl << processedCopy << endl;

    // �ִ�
    vector<string> origWords = splitText(processedOrig);
    vector<string> copyWords = splitText(processedCopy);

    // ����������鿴�ִʽ��
    cerr << "=== ԭ�ķִʽ�� ===" << endl;
    for (const auto& word : origWords) cerr << word << "|";
    cerr << endl << "=== ��Ϯ��ִʽ�� ===" << endl;
    for (const auto& word : copyWords) cerr << word << "|";
    cerr << endl;

    // �����Ƶ
    map<string, int> origFreq = calculateWordFrequency(origWords);
    map<string, int> copyFreq = calculateWordFrequency(copyWords);

    // ����������鿴��Ƶ
    cerr << "=== ԭ�Ĵ�Ƶ ===" << endl;
    for (const auto& pair : origFreq) cerr << pair.first << ":" << pair.second << " ";
    cerr << endl << "=== ��Ϯ���Ƶ ===" << endl;
    for (const auto& pair : copyFreq) cerr << pair.first << ":" << pair.second << " ";
    cerr << endl;

    // �������ƶ�
    double similarity = calculateCosineSimilarity(origFreq, copyFreq);
    double similarityPercent = similarity * 100; 

    // ��������������λС����
    cout.precision(2);
    cout << fixed << similarity << endl;
    cout << fixed << similarityPercent << "%" << endl;
    return 0;
}
