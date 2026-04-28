#pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>

struct Monom {
  int degre;   // stepen 100+10+1
  double coef; // koef monoma
  Monom *next;

  Monom(int deg = 0, double c = 0.0, Monom *n = nullptr)
      : degre(deg), coef(c), next(n) {}
};

class Polinom {
private:
  Monom *head;
  size_t size;

public:
  Polinom();
  Polinom(const Polinom &other);
  ~Polinom();
  Polinom &operator=(const Polinom &other);
  void insertPol(const Monom &m);
  void removeZero();
  void clear();
  Polinom add(const Polinom &other) const;
  Polinom subtract(const Polinom &other) const;
  Polinom mulonconst(double k) const;
  Polinom mul(const Polinom &other) const;
  void print() const;
  bool operator==(const Polinom &other) const;
  size_t getSize() const { return size; }
};