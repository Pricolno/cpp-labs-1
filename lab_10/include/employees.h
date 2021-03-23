#ifndef LAB11_EMPLOYEES_H_INCLUDED
#define LAB11_EMPLOYEES_H_INCLUDED

#include <sstream>
#include <memory>
#include <vector>
#include <fstream>

class Employee {
public:
    virtual int salary() const = 0;

    friend std::istream &operator>>(std::istream &in, Employee &emp);
    friend std::ostream &operator<<(std::ostream &out, const Employee &emp);
    friend std::istream &operator>>(std::ifstream &in, Employee &emp);
    friend std::ostream &operator<<(std::ofstream &out, const Employee &emp);

    virtual ~Employee() = default;

protected:
    virtual void write_txt_to(std::ostream &out) const = 0;
    virtual void read_txt_from(std::istream &in) = 0;
    virtual void write_bin_to(std::ofstream &out) const = 0;
    virtual void read_bin_from(std::ifstream &in) = 0;

    std::string _name;
    int _base_salary = 0;
};

class Developer : public Employee {
public:
    int salary() const override {
        int salary = _base_salary;
        if (_has_bonus) { salary += 1000; }
        return salary;
    }

protected:
    void write_txt_to(std::ostream &out) const override;
    void read_txt_from(std::istream &in) override;
    void write_bin_to(std::ofstream &out) const override;
    void read_bin_from(std::ifstream &in) override;

private:
    bool _has_bonus = false;
};

class SalesManager : public Employee {
public:
    int salary() const override {
        return _base_salary + _sold_nm * _price / 100;
    }

protected:
    void write_txt_to(std::ostream &out) const override;
    void read_txt_from(std::istream &in) override;
    void write_bin_to(std::ofstream &out) const override;
    void read_bin_from(std::ifstream &in) override;

private:
    int _sold_nm = 0, _price = 0;
};

enum EmpType {
    DEV = 1,
    MANAGER = 2
};

std::unique_ptr<Employee> createEmployee(int type);

class EmployeesArray {
public:
    void add(std::unique_ptr<Employee> e);
    int total_salary() const;

    friend std::ostream &operator<<(std::ostream &out, const EmployeesArray &arr);
    friend std::ostream &operator<<(std::ofstream &out, const EmployeesArray &arr);
    friend std::istream &operator>>(std::ifstream &in, EmployeesArray &arr);

private:
    std::vector<std::unique_ptr<Employee>> _employees;
};

#endif
