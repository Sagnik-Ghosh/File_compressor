#include <bits/stdc++.h>

using namespace std;

void compress(string text, string filename) {
    map<string, int> dictionary;
    map<string, pair<int, int>> metric_values;
    set<pair<pair<int, int>, string>> metric;
    int time = 0, n = text.length(), limit = (1 << 15);
    for (int i = 0; i < 256; ++i) {
        string key;
        if (i >= 32 and i <= 126) {
            key += char(i);
            dictionary[key] = i;
        } else {
            key += char(i);
            dictionary[key] = i;
            metric_values[key] = {0, 0};
            metric.insert({metric_values[key], key});
        }
    }
    int code = 255;
    string current;
    current += text[0];
    vector<int> compressed;
    int zeros = 0;
    for (int i = 1; i < n; ++i) {
        ++time;
        string next = current + text[i];
        if (dictionary.find(next) != dictionary.end()) {
            current = next;
            continue;
        } else {
            compressed.push_back(dictionary[current]);
            if (dictionary[current] < 32 or dictionary[current] > 126) {
                metric.erase({metric_values[current], current});
                metric_values[current].first++;
                metric_values[current].second = time;
                metric.insert({metric_values[current], current});
            }
            if (code < limit) {
                ++code;
                dictionary[next] = code;
                metric_values[next] = {0, time};
                metric.insert({metric_values[next], next});
            } else {
                auto infrequent = *metric.begin();
                metric.erase(infrequent);
                metric_values.erase(infrequent.second);
                dictionary[next] = dictionary[infrequent.second];
                dictionary.erase(infrequent.second);
                metric_values[next] = {0, time};
                metric.insert({metric_values[next], next});
            }
            current = "";
            current += text[i];
        }
    }
    compressed.push_back(dictionary[current]);
    string compressedfile = "compressed_" + filename;
    ofstream outputFile(compressedfile);
    for (auto e: compressed) outputFile << e << " ";
}

void decompress(vector<int> compressed, string filename) {
    string decompressedfile = "original" + filename.substr(11, filename.length() - 11);
    ofstream outFile(decompressedfile);
    int n = compressed.size();
    vector<string> decode(1 << 15);
    for (int i = 0; i < 256; ++i) {
        string value;
        value += char(i);
        decode[i] = value;
    }
    int code = 256;
    outFile << decode[compressed[0]];
    string value = decode[compressed[0]];
    for (int i = 1; i < n; ++i) {
        string current;
        if (compressed[i] < code) {
            current += decode[compressed[i]];
            value += current[0];
        } else {
            value += value[0];
            current = value;
        }
        decode[code] = value;
        ++code;
        value = current;
        outFile << current;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    char choice;
    cin >> choice;
    if (choice == 'c') {
        fstream file;
        string filename, word;
        cin >> filename;
        file.open(filename.c_str());
        string text;
        while (file >> word) {
            if (!text.empty())text += " ";
            text += word;
        }
        compress(text, filename);
    } else {
        fstream file;
        string filename;
        int code;
        cin >> filename;
        file.open(filename);
        vector<int> compressed;
        while (file >> code) {
            compressed.push_back(code);
        }
        decompress(compressed, filename);
    }
}
