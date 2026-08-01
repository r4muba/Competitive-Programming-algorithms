#include <bits/stdc++.h>
const int base {1'000'000'000};

class Bint {
public:
    Bint() = default;
    Bint(int num) : d(1, num) {};
    Bint(std::string& num) {
        int n{std::ssize(num)};
        for (int i{n}; i > 0; i -= 9) {
            if (i >= 9) {
                auto s = num.substr(i - 9, 9);
                d.push_back(std::stoi(s));
            } else {
                auto s = num.substr(0, i);
                d.push_back(std::stoi(s));
            }
        }
    }

    Bint operator+(Bint& o) {
        Bint bint{};
        auto& ans{bint.d};
        int carry{};
        for (int i{}; i < std::max(size(), o.size()); ++i) {
            auto x{(i < size()) ? d[i] : 0};
            auto y {(i < o.size()) ? o[i] : 0};
            auto v {(x + y + carry)};
            ans.push_back(v % base);
            carry = (v >= base);
        }
        if (carry) ans.push_back(carry);
        bint.trim();
        return bint;
    }
    
    Bint operator*(int b) {
        Bint bint{};
        auto& ans {bint.d};
        long long c{};
        for (int i{}; i < size(); ++i) {
            long long x {(d[i] * 1ll * b) + c};
            ans.push_back(x % base);
            c = (x / base);
        }
        while (c) {
            ans.push_back(c % base);
            c /= base;
        }
        bint.trim();
        return bint;
    }
    bool operator==(Bint& o) {
        if (size() != o.size()) return false;
        for (int i{}; i < size(); ++i) {
            if (d[i] != o[i]) return false;
        }
        return true;
    }
    bool operator<(Bint& o) {
        if (size() != o.size()) return size() < o.size();
        for (int i{size() - 1}; i >= 0; --i) {
            if (d[i] != o[i]) return d[i] < o[i];
        }
        return false;
    }

    std::string toString() {
        if (d.empty()) return "0";
        std::string s{std::format("{}", d.back())};
        s.reserve(9 * size());
        for (int i{size() - 2}; i >= 0; --i) {
            s += std::format("{:09d}", d[i]);
        }
        return s;
    }
private:
    std::vector<int> d{};
    int operator[](int i) { return d[i]; }
    int size() { return std::ssize(d); }
    void trim() {
        while (size() > 1 && d.back() == 0) d.pop_back();
    }
};
