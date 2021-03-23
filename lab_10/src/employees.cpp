#include "employees.h"

std::istream &operator>>(std::istream &in, Employee &emp) {
    emp.read_txt_from(in);
    return in;
}

std::ostream &operator<<(std::ostream &out, const Employee &emp) {
    emp.write_txt_to(out);
    return out;
}

std::istream &operator>>(std::ifstream &in, Employee &emp) {
    emp.read_bin_from(in);
    return in;
}

std::ostream &operator<<(std::ofstream &out, const Employee &emp) {
    emp.write_bin_to(out);
    return out;
}

void EmployeesArray::add(std::unique_ptr<Employee> e) {
    _employees.emplace_back(std::move(e));
}

int EmployeesArray::total_salary() const {
    int total = 0;
    for (const auto &emp : _employees)
        total += emp->salary();
    return total;
}

void Developer::read_txt_from(std::istream &in) {
    in >> _name >> _base_salary >> _has_bonus;
}

void Developer::write_txt_to(std::ostream &out) const {
    out << "Developer" << std::endl;
    out << "Name: " << _name << std::endl;
    out << "Base Salary: " << _base_salary << std::endl;
    out << "Has bonus: " << (_has_bonus ? "+" : "-") << std::endl;
}

void SalesManager::read_txt_from(std::istream &in) {
    in >> _name >> _base_salary >> _sold_nm >> _price;
}

void SalesManager::write_txt_to(std::ostream &out) const {
    out << "Sales Manager" << std::endl;
    out << "Name: " << _name << std::endl;
    out << "Base Salary: " << _base_salary << std::endl;
    out << "Sold items: " << _sold_nm << std::endl;
    out << "Item price: " << _price << std::endl;
}

std::ostream &operator<<(std::ostream &out, const EmployeesArray &arr) {
    for (std::size_t i = 0; i < arr._employees.size(); ++i) {
        const auto &emp = arr._employees[i];
        out << i + 1 << ". ";
        out << *emp;
    }
    out << "== Total salary: " << arr.total_salary() << std::endl;
    return out;
}

static std::string read_c_str(std::ifstream &in) {
    std::string str;
    char c;
    while (in >> c && c)
        str += c;
    return str;
}

static void write_c_str(std::ofstream &out, const std::string &str) {
    out.write(str.c_str(), str.size() + 1);
}

void Developer::write_bin_to(std::ofstream &out) const {
    int type = EmpType::DEV;
    out.write((const char *) &type, sizeof(int32_t));

    write_c_str(out, _name);
    out.write((const char *) &_base_salary, sizeof(int32_t));
    out.write((const char *) &_has_bonus, sizeof(bool));
}

void Developer::read_bin_from(std::ifstream &in) {
    _name = read_c_str(in);
    in.read((char *) &_base_salary, sizeof(int32_t));
    in.read((char *) &_has_bonus, sizeof(bool));
}

void SalesManager::write_bin_to(std::ofstream &out) const {
    int type = EmpType::MANAGER;
    out.write((const char *) &type, sizeof(int32_t));

    write_c_str(out, _name);
    out.write((const char *) &_base_salary, sizeof(int32_t));
    out.write((const char *) &_sold_nm, sizeof(int32_t));
    out.write((const char *) &_price, sizeof(int32_t));
}

void SalesManager::read_bin_from(std::ifstream &in) {
    _name = read_c_str(in);
    in.read((char *) &_base_salary, sizeof(int32_t));
    in.read((char *) &_sold_nm, sizeof(int32_t));
    in.read((char *) &_price, sizeof(int32_t));
}

std::ostream &operator<<(std::ofstream &out, const EmployeesArray &arr) {
    auto size = (int32_t) arr._employees.size();
    out.write((const char *) &size, sizeof(int32_t));
    for (const auto &emp : arr._employees)
        out << *emp;
    return out;
}

std::istream &operator>>(std::ifstream &in, EmployeesArray &arr) {
    int32_t size;
    in.read((char *) &size, sizeof(int32_t));
    for (; size > 0; --size) {
        int32_t type;
        in.read((char *) &type, sizeof(int32_t));

        std::unique_ptr<Employee> emp = createEmployee(type);
        if (!emp) continue;

        in >> *emp;
        arr.add(std::move(emp));
    }
    return in;
}

std::unique_ptr<Employee> createEmployee(int type) {
    if (type == EmpType::DEV)
        return std::make_unique<Developer>();
    else if (type == EmpType::MANAGER)
        return std::make_unique<SalesManager>();
    return nullptr;
}