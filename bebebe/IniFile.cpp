#include <iostream>
#include "IniFile.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <ctype.h>


std::string delete_space(std::string str) {
    std::string str_new;
    int i = 0, j = str.size() - 1;
    while (isspace(str[i])) {
        i++;
    }
    while (isspace(str[j])) {
        j--;
    }
    return str.substr(i, j - i + 1);
}

std::string get_name_section(std::string str) {
    int k_open = 0, k_close = 0;
    std::string section;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '[') {
            k_open++;
        }
        else if (str[i] == ']') {
            k_close++;
        }
        else if (str[i] == '=') {
            return "";
        }
        else if (k_open == 1 && k_close == 0 && str[i] != '[' && str[i] != ']') {
            section += str[i];
        }
        else if (k_open == k_close) {
            break;
        }
        else {
            return "";
        }
    }
    section = delete_space(section);
    return section;

}

std::pair<std::string, std::string> get_key_value(std::string str) {
    std::string key, value;
    int flag = 0;

    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '=') {
            flag = 1;
            continue;
        }
        if (flag) {
            value += str[i];
        }
        else {
            key += str[i];
        }
    }

    key = delete_space(key);
    value = delete_space(value);
    if (key.empty() || value.empty()) {
        return std::make_pair("", "");
    }
    return std::make_pair(key, value);
}

IniFile::IniFile(const std::string &path) : filePath(path) {//, data(new_data) {
    std::ifstream fp(filePath);
    if (!fp.is_open()) {
        //std::cout << "Error!" << std::endl;
        return;
    }
    std::string str, new_str, section_name, key_value_name;
    KeysMap _k;
    while (getline(fp, str)) {
        str = delete_space(str);
        for (int i = 0; i < str.size(); i++) {
            if (str[i] == ';') {
                str = str.substr(0, i);
                break;
            }
        }
        if (str.empty()) {
            continue;
        }
        if (str[0] == '[') {
            new_str = get_name_section(str);
            if (new_str.empty()) {
                continue;
            }
            section_name = new_str;
            data[section_name];
            continue;
        }
        else {
            std::pair<std::string, std::string> key_value = get_key_value(str);
            if (key_value.first.empty() || key_value.second.empty()) {
                continue;
            }
            data[section_name][key_value.first] = key_value.second;
        }
    }
}

IniFile::~IniFile() {
    //save();
}

void IniFile::save() {
    std::ofstream file(filePath);
    for (auto it = data.begin(); it != data.end(); it++) {
        file << '[' << it->first << ']' << std::endl;
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
            file << jt->first << '=' << jt->second << std::endl;
        }
    }
}

int IniFile::readInt(const std::string &section, const std::string &key, int def) {
    if (!isSectionExist(section) || !isKeysExist(section, key)) {
        return def;
    }
    std::string v = data[section][key];
    try {
        return std::stoi(v);
    }
    catch (std::invalid_argument &) {
        return def;
    }
}

double IniFile::readDouble(const std::string &section, const std::string &key, double def) {
    if (!isSectionExist(section) || !isKeysExist(section, key)) {
        return def;
    }
    std::string v = data[section][key];
    return std::stod(v);
}

std::string IniFile::readString(const std::string &section, const std::string &key, const std::string &def) {
    if (!isSectionExist(section) || !isKeysExist(section, key)) {
        return def;
    }
    return data[section][key];
}

bool f(std::string a, std::string b) {
    int sz = a.size();
    if (b.size() != sz)
        return false;
    for (int i = 0; i < sz; ++i)
        if (std::tolower(a[i]) != std::tolower(b[i]))
            return false;
    return true;
}

bool IniFile::readBool(const std::string &section, const std::string &key, bool def) {
    if (!isSectionExist(section) || !isKeysExist(section, key)) {
        return def;
    }
    std::string v = data[section][key];
    if (f(v, trueValues[0]) || f(v, trueValues[1]) || f(v, trueValues[2]) || f(v, trueValues[3])) {
        return true;
    }
    return false;
}

void IniFile::writeSection(const std::string& section) {
   if (!isSectionExist(section)) {
       data[section] = KeysMap();
   }
}
void IniFile::writeInt(const std::string &section, const std::string &key, int value) {
    if (!isSectionExist(section)) {
        data[section] = KeysMap();
    }
    data[section][key] = std::to_string(value);
}

void IniFile::writeDouble(const std::string &section, const std::string &key, double value) {
    if (!isSectionExist(section)) {
        data[section] = KeysMap();
    }
    data[section][key] = std::to_string(value);
}

void IniFile::writeString(const std::string &section, const std::string &key, const std::string &value) {
    if (!isSectionExist(section)) {
        data[section] = KeysMap();
    }
    data[section][key] = value;
}

void IniFile::writeBool(const std::string &section, const std::string &key, bool value) {
    /*KeysMap _m;
    _m.emplace(key, std::to_string(value));
    data.emplace(section, _m);*/
    if (!isSectionExist(section)) {
        data[section] = KeysMap();
    }
    data[section][key] = std::to_string(value);
}

bool IniFile::isSectionExist(const std::string &section) {
    for (auto it = data.begin(); it != data.end(); it++) {
        if (it->first == section) {
            return true;
        }
    }
    return false;
}

bool IniFile::isKeysExist(const std::string &section, const std::string &key) {
    if (isSectionExist(section)) {
        for (auto it = data.begin(); it != data.end(); it++) {
            if (it->first == section) {
                for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
                    if (jt->first == key) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

size_t IniFile::getSectionsCount() {
    size_t c = 0;
    for (auto it = data.begin(); it != data.end(); it++) {
        c++;
    }
    return c;
}

size_t IniFile::getKeysCount(const std::string &section) {
    if (!isSectionExist(section)) {
        //std::cout << "Error!" << std::endl;
        return 0;
    }
    size_t c = 0;
    for (auto it = data.begin(); it != data.end(); it++) {
        if (it->first == section) {
            for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
                c++;
            }
            return c;
        }
    }
    return 0;
}

SectionsMap IniFile::getSections() const {
    return data;
}

bool IniFile::deleteSection(const std::string &section) {
    if (!isSectionExist(section)) {
        return false;
    }
    for (auto it = data.begin(); it != data.end(); it++) {
        if (it->first == section) {
            data.erase(it->first);
            return true;
        }
    }
    return false;
}

bool IniFile::deleteKey(const std::string &section, const std::string &key) {
    if (!isKeysExist(section, key)) {
        return false;
    }
    for (auto it = data.begin(); it != data.end(); it++) {
        if (it->first == section) {
            for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
                if (jt->first == key) {
                    it->second.erase(jt);
                    return true;
                }
            }
        }
    }
    return false;
}


