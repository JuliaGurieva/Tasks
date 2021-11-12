#include <map>
#include <string>
#include <vector>
#include <tuple>

struct StudentName {
    std::string name, surname;
};
bool operator<(const StudentName& a, const StudentName& b) {
    if (a.surname != b.surname) {
        return a.surname < b.surname;
    } else {
        return a.name < b.name;
    }
}

bool operator!=(const StudentName& a, const StudentName& b) {
    return (a.surname != b.surname || a.name != b.name);
}

struct Date {
    int year, month, day;
};
bool operator<(const Date& a, const Date& b) {
    if (a.year != b.year) {
        return a.year < b.year;
    } else if (a.month != b.month) {
        return a.month < b.month;
    } else {
        return a.day < b.day;
    }
}

bool operator!=(const Date& a, const Date& b) {
    return (a.year != b.year || a.month != b.month || a.day != b.day);
}

using StudInfo = std::tuple<StudentName, Date, int, std::vector<std::string>>;

bool SortScore(StudInfo& a, StudInfo& b) {
    if (std::get<2>(a) != std::get<2>(b)) {
        return std::get<2>(b) < std::get<2>(a);
    } else if (std::get<1>(a) != std::get<1>(b)) {
        return std::get<1>(a) < std::get<1>(b);
    } else {
        return std::get<0>(a) < std::get<0>(b);
    }
}
bool SortStudents(StudInfo& a, StudInfo& b) {
    if (std::get<0>(a) != std::get<0>(b)) {
        return std::get<0>(a) < std::get<0>(b);
    } else {
        return std::get<1>(a) < std::get<1>(b);
    }
}

std::map<std::string, std::vector<StudentName>> GetStudents(
    const std::vector<std::pair<std::string, int>>& university_info,
    const std::vector<std::tuple<StudentName, Date, int, std::vector<std::string>>>&
        students_info) {
    std::vector<StudInfo> copy = students_info;
    std::sort(copy.begin(), copy.end(), SortScore);
    std::map<std::string, std::vector<StudInfo>> tmp;
    std::map<std::string, std::vector<StudentName>> result;
    std::map<std::string, int> university;
    for (auto p : university_info) {
        university[p.first] = p.second;
        result[p.first] = {};
        tmp[p.first] = {};
    }
    for (StudInfo s : copy) {
        for (std::string i : std::get<3>(s)) {
            if (university[i] > 0) {
                tmp[i].push_back(s);
                university[i]--;
                break;
            }
        }
    }
    for (auto p : tmp) {
        std::sort(p.second.begin(), p.second.end(), SortStudents);
        for (auto stud : p.second) {
            result[p.first].push_back(std::get<0>(stud));
        }
    }
    return result;
}
