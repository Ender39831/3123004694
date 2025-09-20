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

// 文本预处理：去除标点符号，转换为小写
string preprocessText(const string& text) {
    string result;
    for (char c : text) {
        // 保留字母、数字和空格，其他视为标点符号去除
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == ' ' ||
            (c >= 0x4e00 && c <= 0x9fa5)) { // 保留汉字
            // 转换为小写
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

// 分词：简单按空格分词
vector<string> splitText(const string& text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
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
    // 计算分子：两个向量的点积
    double dotProduct = 0.0;
    for (const auto& pair : origFreq) {
        const string& word = pair.first;
        int origCount = pair.second;

        auto it = copyFreq.find(word);
        if (it != copyFreq.end()) {
            dotProduct += origCount * it->second;
        }
    }

    // 计算分母：两个向量的模的乘积
    double origMagnitude = 0.0, copyMagnitude = 0.0;
    for (const auto& pair : origFreq) {
        origMagnitude += pow(pair.second, 2);
    }
    origMagnitude = sqrt(origMagnitude);

    for (const auto& pair : copyFreq) {
        copyMagnitude += pow(pair.second, 2);
    }
    copyMagnitude = sqrt(copyMagnitude);

    // 防止除零错误
    if (origMagnitude == 0 || copyMagnitude == 0) {
        return 0.0;
    }

    return dotProduct / (origMagnitude * copyMagnitude);

}

// 读取文件内容
string readFile(const string& filePath) {
    ifstream file(filePath, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "错误：无法打开文件 '" << filePath << "'，请检查路径是否正确。" << endl;
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

// 写入结果到文件
void writeResult() {

}

int main(int argc, char* argv[]) {
    // 输入3个参数：原文、抄袭版、输出结果
    if (argc != 4) {
        cerr << "参数错误：请提供3个文件路径参数" << endl;
        cerr << "正确用法：原文文件路径 抄袭版论文路径 输出结果路径" << endl;
        cerr << "示例：PlagiarismDetectionOfPapers.exe C:\orig.txt C:\copy.txt C:\result.txt" << endl;
        return 1;
    }
    else {
        cerr << "开始进行查重率计算" << endl;
    }

    // 从命令行参数获取路径
    string origPath = argv[1];
    string copyPath = argv[2];
    string outputPath = argv[3];

    // 读取文件内容
    string origText = readFile(origPath);
    string copyText = readFile(copyPath);

    cerr << origText << endl;
    cerr << copyText << endl;

    // 预处理文本
    string processedOrig = preprocessText(origText);
    string processedCopy = preprocessText(copyText);

    //cerr << processedOrig << endl;
    //cerr << processedCopy << endl;

    // 分词
    vector<string> origWords = splitText(processedOrig);
    vector<string> copyWords = splitText(processedCopy);

    // 计算词频
    map<string, int> origFreq = calculateWordFrequency(origWords);
    map<string, int> copyFreq = calculateWordFrequency(copyWords);

    // 计算相似度
    double similarity = calculateCosineSimilarity(origFreq, copyFreq);

    cerr << similarity << endl;


    // 写入结果
    writeResult();

    return 0;

}