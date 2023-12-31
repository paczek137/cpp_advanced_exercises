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

struct Element
{
    Element *behind = nullptr;
    std::vector<double> data;
};

struct QueueDE // Double Ended
{
    Element *_first = nullptr;
    Element *_last = nullptr;
    long _size = 0;

    long const & size() const
    {
        return _size;
    }

    bool const empty() const
    {
        return 0 == _size;
    }

    void add(const initializer_list<double> d)
    {
        Element *el = new Element;
        el->data.insert(el->data.end(), d.begin(), d.end());
        if (not this->empty())
        {
            _last->behind = el;
            _last = el;
        }
        else
        {
            _first = _last = el;
        }
        _size++;
    }

    void remove()
    {
        if (this->empty()) return;
        if (this->size() > 1)
        {
            Element *tmp = _first;
            _first = _first->behind;
            delete tmp;
        }
        else if (this->size() == 1)
        {
            delete _first;
            _first = _last = nullptr;
        }
        _size--;
    }

    vector<double> first()
    {
        if (not this->empty())
        {
            return _first->data;
        }
        cout << "No elements!" << endl;
        return {};
    }

    vector<double> last()
    {
        if (not this->empty())
        {
            return _last->data;
        }
        cout << "No elements!" << endl;
        return {};
    }

    std::string const to_str() const
    {
        if (not this->empty())
        {
            std::string s = "--- first\n";
            Element *tmp = _first;
            while (tmp != nullptr)
            {   
                s += "-> ";
                s += boost::algorithm::join(tmp->data |
                        boost::adaptors::transformed(static_cast<std::string(*)(double)>(std::to_string)), " ");
                s += "\n";
                tmp = tmp->behind;
            }
            return s + "### last\n";
        }
        else return "No elements!\n";
    }

    friend std::ostream & operator<<(std::ostream &os, const QueueDE &queue)
    {
        return os << queue.to_str(); 
    }
};


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
    srand(time(0));
    
    // std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path cwd = getExePath();
    cout << "Hello World: " << cwd.filename() << endl;

    QueueDE queue{};
    queue.add({1., 2., 3.});
    cout << queue << endl;

    queue.add({2., 3., 4.5});
    queue.add({5., 7., 11.});
    cout << queue << endl;

    queue.remove();
    cout << queue << endl;

    queue.remove();
    queue.remove();
    queue.remove();
    cout << queue << endl;

    return EXIT_SUCCESS;
}
