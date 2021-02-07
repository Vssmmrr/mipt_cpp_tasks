#include <unordered_map>
#include <set>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

struct Person {
    std::string name;
    std::set<std::string> phones;
    std::string email;
};

class Book {
private:
    std::unordered_map<std::string, Person> book_;

public:
    bool AddPerson(const std::string& name, const std::string& phone, const std::string& email) {
        if (book_.count(name) > 0) {
            return false;
        }
        std::set<std::string> phones;
        if (phone != "") {
            phones.insert(phone);
        }
        book_[name] = {name, phones, email};
        return true;
    }

    bool AddPhone(const std::string& name, const std::string& phone) {
        if (book_.count(name) == 0) {
            return false;
        }
        Person& person = book_.at(name);
        if (person.phones.count(phone) > 0) {
            return false;
        }
        person.phones.insert(phone);
        return true;
    }

    bool ReplaceEmail(const std::string& name, const std::string& email) {
        if (book_.count(name) == 0) {
            return false;
        }
        book_.at(name).email = email;
        return true;
    }

    bool ReplacePhone(const std::string& name, const std::string& old_phone, const std::string& new_phone) {
        if (book_.count(name) == 0) {
            return false;
        }
        Person& person = book_.at(name);
        if (person.phones.count(old_phone) == 0 || person.phones.count(new_phone) > 0) {
            return false;
        }
        person.phones.erase(old_phone);
        person.phones.insert(new_phone);
        return true;
    }

    bool DeletePerson(const std::string& name) {
        if (book_.count(name) == 0) {
            return false;
        }
        book_.erase(name);
        return true;
    }

    bool DeletePhone(const std::string& name, const std::string& phone) {
        if (book_.count(name) == 0) {
            return false;
        }
        Person& person = book_.at(name);
        if (person.phones.count(phone) == 0) {
            return false;
        }
        person.phones.erase(phone);
        return true;
    }

    const Person& GetPerson(const std::string& name) const {
        return book_.at(name);
    }
};

std::vector<std::string> Split(const std::string& str) {
    auto pos = str.begin();
    std::vector<std::string> res;
    while (pos != str.end()) {
        pos = std::find_if_not(pos, str.end(), isspace);
        if (pos != str.end()) {
            auto next = std::find_if(pos, str.end(), isspace);
            res.emplace_back(pos, next);
            pos = next;
        }
    }
    return res;
}

int main() {
    Book book;

    while (true) {
        std::string cmd;
        std::getline(std::cin, cmd);

        auto splitted = Split(cmd);

        if (!splitted.empty()) {
            if (splitted[0] == "Finish") {
                break;
            } else if (splitted[0] == "AddPerson") {
                std::string phone;
                std::string email;
                for (auto str = splitted.begin() + 2; str != splitted.end(); ++str) {
                    if (str->find('@') != std::string::npos) {
                        email = *str;
                    } else {
                        phone = *str;
                    }
                }
                if (book.AddPerson(splitted[1], phone, email)) {
                    std::cout << "DONE\n";
                } else {
                    std::cout << "FAIL\n";
                }
            } else if (splitted[0] == "AddPhone") {
                if (book.AddPhone(splitted[1], splitted[2])) {
                    std::cout << "DONE\n";
                } else {
                    std::cout << "FAIL\n";
                }
            } else if (splitted[0] == "ReplaceEmail") {
                if (book.ReplaceEmail(splitted[1], splitted[2])) {
                    std::cout << "DONE\n";
                } else {
                    std::cout << "FAIL\n";
                }
            } else if (splitted[0] == "ReplacePhone") {
                if (book.ReplacePhone(splitted[1], splitted[2], splitted[3])) {
                    std::cout << "DONE\n";
                } else {
                    std::cout << "FAIL\n";
                }
            } else if (splitted[0] == "DeletePerson") {
                if (book.DeletePerson(splitted[1])) {
                    std::cout << "DONE\n";
                } else {
                    std::cout << "FAIL\n";
                }
            } else if (splitted[0] == "DeletePhone") {
                if (book.DeletePhone(splitted[1], splitted[2])) {
                    std::cout << "DONE\n";
                } else {
                    std::cout << "FAIL\n";
                }
            } else {
                try {
                    const Person &person = book.GetPerson(splitted[1]);
                    std::cout << "Name: " << person.name << ";\n";
                    if (!person.phones.empty()) {
                        std::cout << "Phones: ";
                        for (auto phone = person.phones.begin(); phone != --person.phones.end(); ++phone) {
                            std::cout << *phone << ", ";
                        }
                        std::cout << *(--person.phones.end()) << ";\n";
                    }
                    if (person.email != "") {
                        std::cout << "Email: " << person.email << ";\n";
                    }
                } catch (...) {
                    std::cout << "FAIL\n";
                }
            }
        }
    }
}
