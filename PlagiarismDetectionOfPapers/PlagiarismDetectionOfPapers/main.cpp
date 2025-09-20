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

// �ı�Ԥ����ȥ�������ţ�ת��ΪСд
string preprocessText(const string& text) {
    string result;
    for (char c : text) {
        // ������ĸ�����ֺͿո�������Ϊ������ȥ��
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == ' ' ||
            (c >= 0x4e00 && c <= 0x9fa5)) { // ��������
            // ת��ΪСд
            if (c >= 'A' && c <= 'Z') {
                result += tolower(c);
            }
            else {
                result += c;
            }
        }
    }
    return result;
}

// �ִʣ��򵥰��ո�ִ�
vector<string> splitText(const string& text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
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
    // ������ӣ����������ĵ��
    double dotProduct = 0.0;
    for (const auto& pair : origFreq) {
        const string& word = pair.first;
        int origCount = pair.second;

        auto it = copyFreq.find(word);
        if (it != copyFreq.end()) {
            dotProduct += origCount * it->second;
        }
    }

    // �����ĸ������������ģ�ĳ˻�
    double origMagnitude = 0.0, copyMagnitude = 0.0;
    for (const auto& pair : origFreq) {
        origMagnitude += pow(pair.second, 2);
    }
    origMagnitude = sqrt(origMagnitude);

    for (const auto& pair : copyFreq) {
        copyMagnitude += pow(pair.second, 2);
    }
    copyMagnitude = sqrt(copyMagnitude);

    // ��ֹ�������
    if (origMagnitude == 0 || copyMagnitude == 0) {
        return 0.0;
    }

    return dotProduct / (origMagnitude * copyMagnitude);

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
void writeResult() {

}

int main(int argc, char* argv[]) {
    // ����3��������ԭ�ġ���Ϯ�桢������
    if (argc != 4) {
        cerr << "�����������ṩ3���ļ�·������" << endl;
        cerr << "��ȷ�÷���ԭ���ļ�·�� ��Ϯ������·�� ������·��" << endl;
        cerr << "ʾ����PlagiarismDetectionOfPapers.exe C:\orig.txt C:\copy.txt C:\result.txt" << endl;
        return 1;
    }
    else {
        cerr << "��ʼ���в����ʼ���" << endl;
    }

    // �������в�����ȡ·��
    string origPath = argv[1];
    string copyPath = argv[2];
    string outputPath = argv[3];

    // ��ȡ�ļ�����
    string origText = readFile(origPath);
    string copyText = readFile(copyPath);

    cerr << origText << endl;
    cerr << copyText << endl;

    // Ԥ�����ı�
    string processedOrig = preprocessText(origText);
    string processedCopy = preprocessText(copyText);

    //cerr << processedOrig << endl;
    //cerr << processedCopy << endl;

    // �ִ�
    vector<string> origWords = splitText(processedOrig);
    vector<string> copyWords = splitText(processedCopy);

    // �����Ƶ
    map<string, int> origFreq = calculateWordFrequency(origWords);
    map<string, int> copyFreq = calculateWordFrequency(copyWords);

    // �������ƶ�
    double similarity = calculateCosineSimilarity(origFreq, copyFreq);

    cerr << similarity << endl;


    // д����
    writeResult();

    return 0;

}