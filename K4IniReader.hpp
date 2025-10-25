#pragma once

/*
 *  K4INIReader - Lightweight cross-platform INI reader
 *  Author: Kevin4e
 */

/*
 *  Copyright (c) 2025 Kevin4e
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, **provided that the above copyright notice and this
 *  permission notice shall be included in all copies or substantial portions of the Software**.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <unordered_map>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <charconv>

class K4INIReader {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

    static inline void trim(std::string& s) noexcept {
        // Trim from start
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));

        // Trim from end
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    }

    static inline void removeInlineComment(std::string& s) noexcept {
        size_t commaOrHashtagPos = s.find_first_of(";#");
        size_t doubleSlashPos = s.find("//");

        if (commaOrHashtagPos != std::string::npos) {
            if (doubleSlashPos != std::string::npos)
                s.erase(std::min(commaOrHashtagPos, doubleSlashPos));
            else
                s.erase(commaOrHashtagPos);
        }
        else {
            if (doubleSlashPos != std::string::npos)
                s.erase(doubleSlashPos);
        }
    }

    inline bool find(const std::string& s, const std::string& k, std::string& out) const noexcept {
        auto sectionIt = data.find(s);
        if (sectionIt == data.end()) return false;

        auto keyIt = sectionIt->second.find(k);
        if (keyIt == sectionIt->second.end()) return false;

        out = keyIt->second;

        return true;
    }

public:
    // Extracts all the sections and keys found and their values
    K4INIReader(const std::string& fileName, size_t nSections = 32, size_t nKeys = 8) {
        std::ifstream file(fileName);

        data.reserve(nSections); // Reserve sections

        std::string line;
        std::string currentSection = "";

        while (std::getline(file, line)) {
            removeInlineComment(line);

            if (line.empty()) continue;

            size_t posBracketStart = line.find('[');
            size_t posEqualSing = line.find('=');

            if (posBracketStart != std::string::npos) {
                size_t posBracketEnd = line.find(']', posBracketStart);
                if (posBracketEnd == std::string::npos) continue;

                std::string sectionExtracted = line.substr(posBracketStart + 1, posBracketEnd - posBracketStart - 1);
                trim(sectionExtracted);
                currentSection = sectionExtracted;
                data[currentSection].reserve(nKeys); // Reserve keys for this section
            }
            else if (posEqualSing != std::string::npos) {
                std::string keyExtracted = line.substr(0, posEqualSing);
                trim(keyExtracted);

                std::string value = line.substr(posEqualSing + 1);
                trim(value);

                data[currentSection][keyExtracted] = value;
            }
        }
    }
    template<typename T>
    T read(const std::string& section, const std::string& key, T defaultValue, bool toLowerString = true) const noexcept {
        std::string outValue;
        if (!find(section, key, outValue)) return defaultValue;

        if constexpr (std::is_integral_v<T> || std::is_same_v<T, float> || std::is_same_v<T, double>) { // If T is integral (int, int32_t, uint64_t, etc.) or float/double
            T outParsedValue;
            auto result = std::from_chars(outValue.data(), outValue.data() + outValue.size(), outParsedValue);
            if (result.ec != std::errc()) return defaultValue;
            return outParsedValue;
        }

        if constexpr (std::is_same_v<T, char>)
            return outValue.empty() ? defaultValue : outValue[0];

        if constexpr (std::is_same_v<T, std::string>) {
            if (toLowerString) {
                bool hasUpper = std::any_of(outValue.begin(), outValue.end(), [](unsigned char c) { return std::isupper(c); });
                if (hasUpper)
                    std::transform(outValue.begin(), outValue.end(), outValue.begin(), [](unsigned char c) { return std::tolower(c); });
            }

            return outValue;
        }
    }
};