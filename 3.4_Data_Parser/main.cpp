#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <memory>
#include <map>
#include <unordered_map>
#include <set>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/range/adaptor/tokenized.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <chrono>
#include <thread>
#include <compare>
#include <initializer_list>
#include <ctime>
#include <concepts>
#include <regex>
#include <deque>
#include <climits>
#include <thread>
#include <codecvt>

using namespace std;

size_t headingPosition(std::vector<wstring> &headings, wstring skill)
{
    size_t pos = 0;
    for (auto x : headings)
    {
        pos++;
        if (x == skill)
        {
            // cout << "return: " << pos << endl;
            return pos;
        }
    }
    return pos;
}

std::string getStringFromWstring(const wstring &wstr)
{
    static wstring_convert<codecvt_utf8<wchar_t>> cvu32{};
    return cvu32.to_bytes(wstr);
}

std::string getExePath()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

int main()
{
    setlocale(LC_ALL, "");
    // std::ios_base::sync_with_stdio(false);
    // std::wcout.imbue(std::locale{""});
    // std::locale::global(std::locale("pl_PL.utf8"));
    // system("chcp 65001");
    srand(time(0));
    
    // std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path cwd = getExePath();
    cout << "Hello World: " << cwd.filename() << endl;

    string filename(cwd.parent_path()/"4_wyniki.txt");
    cout << "reading: " << filename << endl;
    wifstream inputFile(filename);
    inputFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    // ifstream inputFile2(filename);
    wstring temp{};
    string temp2{};

    vector<wstring> headings = {L"agent", L"strzelanie", L"hakowanie", L"walka wręcz", L"pływanie", 
                                    L"wspinaczka", L"inteligencja", L"wiedza"};
    getline(inputFile, temp);
    // std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    wcout << temp << endl;
    cout << getStringFromWstring(temp) << endl;

    unordered_map<wstring, multimap<int, wstring>> data{};
    for (size_t i=1; i<headings.size(); i++)
    {
        data.insert({headings[i], {}});
        cout << getStringFromWstring(headings[i]) << endl;
        // wcout << headings[i] << endl;
    }

    while(not inputFile.eof())
    {
        size_t p = 0;
        wstring agent{};
        getline(inputFile, agent, L',');
        p++;

        for (int i=6; i--;)
        {
            getline(inputFile, temp, L',');
            data[headings[p]].insert({stoi(temp), agent});
            // cout << data.size() << endl;
            p++;
        }
        getline(inputFile, temp);
        data[headings[p]].insert({stoi(temp), agent});
    }

    cout << "a) 10 best results from swimming" << endl;
    set<int> ten{};
    auto iten = data[L"pływanie"s].rbegin();
    while(iten != data[L"pływanie"s].rend() and ten.size() <= 10)
    {
        auto &[v, agent] = *iten++;
        ten.insert(v);
        if (ten.size() <= 10) 
        {
            // wcout << agent << L"->" << v << endl;
            cout << getStringFromWstring(agent) << "->" << v << endl;
            // cout << "something" << endl;
        }
    }

    cout << "b) In which competition agents perform best" << endl;
    double maxAverage = -1;
    wstring skill{};
    for (size_t i=1; i<headings.size(); i++)
    {
        double average{};
        for (auto &[v, agent] : data[headings[i]])
        {
            average += v;
        }
        average /= data[headings[i]].size();

        if(maxAverage < average)
        {
            maxAverage = average;
            skill = headings[i];
        }
    }
    cout << "Best average: " << maxAverage << " in " << getStringFromWstring(skill) << endl;

    cout << "c)" << endl;
    for (size_t i=1; i<headings.size(); i++)
    {
        auto first = data[headings[i]].begin();
        int fv = first->first;
        while (fv == first->first)
        {
            auto &[v, agent] = *first;
            cout << getStringFromWstring(agent) << " in " << getStringFromWstring(headings[i]) << " has only " << v << endl;
            ++first;
        }
    }

    cout << "d)\n";
    for (auto &e : vector<wstring>{ L"wspinaczka",L"hakowanie",L"wiedza" })
    {
        cout << "------------" << getStringFromWstring(e) << endl;
        for (auto it = data[e].rbegin(); it != data[e].rend(); it++)
        {
          auto &[v, agent] = *it;
          cout << getStringFromWstring(agent) << "\t\t" << v << endl;
        }
    }

    return EXIT_SUCCESS;
}
