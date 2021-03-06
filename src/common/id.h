#pragma once
#include "common/endpoint.h"
namespace raftcpp {

class BaseID {
public:
    BaseID() { data_ = ""; }

    bool operator==(const BaseID &rhs) const { return (this->ToHex() == rhs.ToHex()); }

    bool operator!=(const BaseID &rhs) const { return (this->ToHex() != rhs.ToHex()); }

    std::string ToBinary() const { return data_; }

    std::string ToHex() const {
        std::string result;
        result.resize(data_.length() * 2);
        for (size_t i = 0; i < data_.size(); i++) {
            uint8_t cTemp = data_[i];
            for (size_t j = 0; j < 2; j++) {
                uint8_t cCur = (cTemp & 0x0f);
                if (cCur < 10) {
                    cCur += '0';
                } else {
                    cCur += ('a' - 10);
                }
                result[2 * i + 1 - j] = cCur;
                cTemp >>= 4;
            }
        }
        return result;
    }

protected:
    std::string data_;
};

class NodeID : public BaseID {
public:
    NodeID() {}
    NodeID(const Endpoint &endpoint_id) {
        data_ = "";
        data_.resize(sizeof(uint32_t) + sizeof(uint16_t));
        uint32_t inet = ip2uint(endpoint_id.GetHost());
        uint16_t port = endpoint_id.GetPort();
        memcpy(data_.data(), &inet, sizeof(uint32_t));
        memcpy(data_.data() + sizeof(uint32_t), &port, sizeof(uint16_t));
    }
    NodeID(const NodeID &nid) { data_ = nid.data_; }
    NodeID &operator=(const NodeID &o) {
        if (this == &o) return *this;
        data_ = o.data_;
        return *this;
    }

private:
    const std::vector<std::string> explode(const std::string &s, const char &c) {
        std::string buff{""};
        std::vector<std::string> v;

        for (auto n : s) {
            if (n != c)
                buff += n;
            else if (n == c && buff != "") {
                v.push_back(buff);
                buff = "";
            }
        }
        if (buff != "") v.push_back(buff);

        return v;
    }
    uint32_t ip2uint(const std::string &ip) {
        std::vector<std::string> v{explode(ip, '.')};
        uint32_t result = 0;
        for (auto i = 1; i <= v.size(); i++)
            if (i < 4)
                result += (stoi(v[i - 1])) << (8 * (4 - i));
            else
                result += stoi(v[i - 1]);

        return result;
    }
};

class TermID : public BaseID {
public:
    TermID() { term_ = 0; }
    TermID(int32_t term) : term_(term) {
        data_ = "";
        data_.resize(sizeof(int32_t));
        memcpy(data_.data(), &term_, sizeof(int32_t));
    }
    TermID(const TermID &tid) { data_ = tid.data_; }
    TermID &operator=(const TermID &o) {
        if (this == &o) return *this;
        data_ = o.data_;
        return *this;
    }
    int32_t getTerm() { return term_; }
    void setTerm(int32_t term) {
        term_ = term;
        data_ = "";
        data_.resize(sizeof(int32_t));
        memcpy(data_.data(), &term_, sizeof(int32_t));
    }

private:
    int32_t term_;
};

}  // namespace raftcpp
