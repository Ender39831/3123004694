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

// 文本预处理：去除标点符号，保留汉字、字母、数字
string preprocessText(const string& text) {
    string result;
    for (size_t i = 0; i < text.size();) {
        // 处理UTF-8汉字（3字节）
        if ((unsigned char)text[i] >= 0xE0 && i + 2 < text.size()) {
            result += text.substr(i, 3);
            i += 3;
        }
        // 处理ASCII字符
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

// 分词：对中文按单个字符分割，对英文按空格分割
vector<string> splitText(const string& text) {
    vector<string> tokens;
    string currentWord;

    for (size_t i = 0; i < text.size();) {
        // 判断是否为汉字
        if ((unsigned char)text[i] >= 0xE0 && i + 2 < text.size()) {
            string chineseChar = text.substr(i, 3);
            tokens.push_back(chineseChar);
            i += 3;
        }
        // 处理空格（作为英文单词的分隔符）
        else if (text[i] == ' ') {
            if (!currentWord.empty()) {
                tokens.push_back(currentWord);
                currentWord.clear();
            }
            i++;
        }
        // 处理英文字母和数字
        else {
            currentWord += text[i];
            i++;
        }
    }

    // 添加最后一个单词
    if (!currentWord.empty()) {
        tokens.push_back(currentWord);
    }

    return tokens;
}

// 计算词频
map<string, int> calculateWordFrequency(const vector<string>& words) {
    map<string, int> freq;
    for (const string& word : words) {
        freq[word]++;
    }
    return freq;
}

// 计算余弦相似度
double calculateCosineSimilarity(const map<string, int>& origFreq, const map<string, int>& copyFreq) {
    // 收集所有唯一词
    map<string, bool> allWords;
    for (const auto& pair : origFreq) allWords[pair.first] = true;
    for (const auto& pair : copyFreq) allWords[pair.first] = true;

    // 计算分子：点积
    double dotProduct = 0.0;
    // 计算两个向量的模长
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

    // 防止除零错误
    if (origMagnitude == 0 || copyMagnitude == 0) {
        return 0.0;
    }

    // 计算余弦相似度
    return dotProduct / (sqrt(origMagnitude) * sqrt(copyMagnitude));
}

// 读取文件内容
string readFile(const string& filePath) {
    ifstream file(filePath, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "错误：无法打开文件 '" << filePath << "'，请检查路径是否正确。" << endl;
        exit(1);
    }

    string content;
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        content.append(buffer, sizeof(buffer));
    }
    // 读取剩余内容
    content.append(buffer, file.gcount());

    file.close();
    return content;
}

int main(int argc, char* argv[]) {
    // 检查参数
    if (argc != 4) {
        cerr << "参数错误：请提供3个文件路径参数" << endl;
        cerr << "正确用法：原文文件路径 抄袭版论文路径 输出结果路径" << endl;
        return 1;
    }

    // 从命令行参数获取路径
    string origPath = argv[1];
    string copyPath = argv[2];
    string outputPath = argv[3];

    // 读取文件内容
    string origText = readFile(origPath);
    string copyText = readFile(copyPath);

    // 调试输出：查看原始内容
    cerr << "=== 原文内容 ===" << endl << origText << endl;
    cerr << "=== 抄袭版内容 ===" << endl << copyText << endl;

    // 预处理文本
    string processedOrig = preprocessText(origText);
    string processedCopy = preprocessText(copyText);

    // 调试输出：查看预处理结果
    cerr << "=== 预处理后原文 ===" << endl << processedOrig << endl;
    cerr << "=== 预处理后抄袭版 ===" << endl << processedCopy << endl;

    // 分词
    vector<string> origWords = splitText(processedOrig);
    vector<string> copyWords = splitText(processedCopy);

    // 调试输出：查看分词结果
    cerr << "=== 原文分词结果 ===" << endl;
    for (const auto& word : origWords) cerr << word << "|";
    cerr << endl << "=== 抄袭版分词结果 ===" << endl;
    for (const auto& word : copyWords) cerr << word << "|";
    cerr << endl;

    // 计算词频
    map<string, int> origFreq = calculateWordFrequency(origWords);
    map<string, int> copyFreq = calculateWordFrequency(copyWords);

    // 调试输出：查看词频
    cerr << "=== 原文词频 ===" << endl;
    for (const auto& pair : origFreq) cerr << pair.first << ":" << pair.second << " ";
    cerr << endl << "=== 抄袭版词频 ===" << endl;
    for (const auto& pair : copyFreq) cerr << pair.first << ":" << pair.second << " ";
    cerr << endl;

    // 计算相似度
    double similarity = calculateCosineSimilarity(origFreq, copyFreq);
    double similarityPercent = similarity * 100; 

    // 输出结果（保留两位小数）
    cout.precision(2);
    cout << fixed << similarity << endl;
    cout << fixed << similarityPercent << "%" << endl;
    return 0;
}
