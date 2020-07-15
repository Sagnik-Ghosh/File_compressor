#include <bits/stdc++.h>

using namespace std;

void compress(string text, string &compressedfileName) {
    map<string, int> dictionary;
    map<string, pair<int, int>> metric_values;
    set<pair<pair<int, int>, string>> metric;
    int time = 0, n = text.length(), limit = (1 << 15);
    for (int i = 0; i < 256; ++i) {
        string key;
        key += char(i);
        dictionary[key] = i;
    }
    int code = 255;
    string current;
    current += text[0];
    vector<int> compressed;
    for (int i = 1; i < n; ++i) {
        ++time;
        string next = current + text[i];
        if (dictionary.find(next) != dictionary.end()) {
            current = next;
            continue;
        } else {
            compressed.push_back(dictionary[current]);
            if (dictionary[current] >= 256) {
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
    ofstream compressedFile(compressedfileName);
    for (auto e : compressed) compressedFile << e << " ";
}

void decompress(vector<int> compressed, string &decompressedfileName) {
    ofstream decompressedFile(decompressedfileName);
    int n = compressed.size();

    vector<string> decode((1 << 15) + 1);
    vector<pair<int, int>> metric_values((1 << 15) + 1);
    set<pair<pair<int, int>, int>> metric;

    int limit = (1 << 15);
    for (int i = 0; i < 256; ++i) {
        string value;
        value += char(i);
        decode[i] = value;
    }
    int code = 256, full = 0, time = 0;
    decompressedFile << decode[compressed[0]];
    string value = decode[compressed[0]];
    for (int i = 1; i < n; ++i) {
        ++time;
        string current;
        if (full == 0) {
            if (compressed[i] != code) {
                current += decode[compressed[i]];
                value += current[0];
            } else {
                value += value[0];
                current = value;
            }
            decode[code] = value;
            metric_values[code] = {0, time};
            metric.insert({metric_values[code], code});
            if (compressed[i] >= 256) {
                metric.erase({metric_values[compressed[i]], compressed[i]});
                metric_values[compressed[i]].first++;
                metric_values[compressed[i]].second = time;
                metric.insert({metric_values[compressed[i]], compressed[i]});
            }
            code++;
            if (code > limit)full = 1;
        }
        else {
            auto infrequent = *metric.begin();
            metric.erase(infrequent);
            code = infrequent.second;
            if (compressed[i] != code) {
                current += decode[compressed[i]];
                value += current[0];
            } else {
                value += value[0];
                current = value;
            }
            decode[code] = value;
            metric_values[code] = {0, time};
            metric.insert({metric_values[code], code});
            if (compressed[i] >= 256) {
                metric.erase({metric_values[compressed[i]], compressed[i]});
                metric_values[compressed[i]].first++;
                metric_values[compressed[i]].second = time;
                metric.insert({metric_values[compressed[i]], compressed[i]});
            }
        }
        value = current;
        decompressedFile << current;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    string command;
    cout << "Types of command :" << endl << flush;
    cout << "1. c file_name compressed_file_name " << endl << flush;
    cout << "2. d file_name decompressed_file_name " << endl << flush;
    cin >> command;
    char choice = command[0];
    if (command.length() == 1) {
        if (choice == 'c') {
            fstream file;
            string filename, word, compressedfileName;
            cin >> filename >> compressedfileName;
            file.open(filename.c_str());
            string text;
            while (file >> word) {
                if (!text.empty())text += " ";
                text += word;
            }
            compress(text, compressedfileName);
        } else if (choice == 'd') {
            fstream file;
            string filename, decompressedfileName;
            int code;
            cin >> filename >> decompressedfileName;
            file.open(filename);
            vector<int> compressed;
            while (file >> code) {
                compressed.push_back(code);
            }
            decompress(compressed, decompressedfileName);
        } else {
            cout << "Wrong Command!!!" << endl;
        }
    } else {
        cout << "Unidentifiable input format!!!" << endl;
    }
}