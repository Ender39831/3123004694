#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

string temp;

// 文本预处理：去除标点符号，转换为小写
string preprocessText() {
    return temp;
}

// 计算余弦相似度
double calculateCosineSimilarity() {
    return 0;
}

// 读取文件内容
string readFile() {
    return temp;
}

// 写入结果到文件
void writeResult() {
    
}

int main(int argc, char* argv[]) {
    // 输入3个参数：原文、抄袭版、输出结果
    

    // 读取文件内容
    string origText = readFile();
    string copyText = readFile();

    // 预处理文本
    string processedOrig = preprocessText();
    string processedCopy = preprocessText();

    // 计算相似度
    double similarity = calculateCosineSimilarity();

    // 写入结果
    writeResult();

    return 0;
}
