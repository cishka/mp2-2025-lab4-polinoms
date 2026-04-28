#include "polinom.h"

#include <cmath>
#include <stdexcept>

Polinom::Polinom() : size(0) {
  head = new Monom;
  head->next = nullptr;
}
Polinom::Polinom(const Polinom &other) : size(0) {
  head = new Monom;
  head->next = nullptr;
  Monom *curr = other.head->next;
  while (curr) {
    insertPol(*curr);
    curr = curr->next;
  }
}

Polinom::~Polinom() {
  clear();
  delete head;
}

Polinom &Polinom::operator=(const Polinom &other) {
  if (this != &other) {
    Polinom temp(other);
    std::swap(head, temp.head);
    std::swap(size, temp.size);
  }
  return *this;
}

void Polinom::clear() {
  Monom *curr = head->next;
  while (curr != nullptr) {
    Monom *temp = curr;
    curr = curr->next;
    delete temp;
  }
  head->next = nullptr;
  size = 0;
}

void Polinom::removeZero() {
  Monom *prev = head;
  Monom *curr = head->next;
  while (curr != nullptr) {
    if (fabs(curr->coef) < 1e-12) {
      prev->next = curr->next;
      delete curr;
      curr = prev->next;
      size--;
    } else {
      prev = curr;
      curr = curr->next;
    }
  }
}

void Polinom::insertPol(const Monom &m) {
  if (fabs(m.coef) < 1e-12)
    return;

  Monom *prev = head;
  Monom *curr = head->next;
  while (curr != nullptr && curr->degre > m.degre) {
    prev = curr;
    curr = curr->next;
  }

  if (curr != nullptr && curr->degre == m.degre) {
    curr->coef += m.coef;
    if (fabs(curr->coef) < 1e-12) {
      prev->next = curr->next;
      delete curr;
      size--;
    }
  } else {
    Monom *newN = new Monom(m.degre, m.coef, curr);
    prev->next = newN;
    size++;
  }
}

Polinom Polinom::add(const Polinom &other) const {
  Polinom result;
  Monom *a = head->next;
  Monom *b = other.head->next;
  while (a != nullptr && b != nullptr) {
    if (a->degre > b->degre) {
      result.insertPol(*a);
      a = a->next;
    } else if (a->degre < b->degre) {
      result.insertPol(*b);
      b = b->next;
    } else {
      double summ = a->coef + b->coef;
      if (fabs(summ) > 1e-12) {
        result.insertPol(Monom(a->degre, summ));
      }
      a = a->next;
      b = b->next;
    }
  }

  while (a != nullptr) {
    result.insertPol(*a);
    a = a->next;
  }
  while (b != nullptr) {
    result.insertPol(*b);
    b = b->next;
  }
  return result;
}

Polinom Polinom::subtract(const Polinom &other) const {
  Polinom b = other.mulonconst(-1.0);
  return this->add(b);
}

Polinom Polinom::mulonconst(double k) const {
  Polinom result;
  if (fabs(k) < 1e-12)
    return result;
  Monom *curr = head->next;
  while (curr != nullptr) {
    double newCoef = curr->coef * k;
    if (fabs(newCoef) > 1e-12)
      result.insertPol(Monom(curr->degre, newCoef));
    curr = curr->next;
  }
  return result;
}

Polinom Polinom::mul(const Polinom &other) const {
  Polinom result;
  Monom *a = head->next;
  while (a != nullptr) {
    Monom *b = other.head->next;
    while (b != nullptr) {
      int x1 = a->degre / 100;
      int y1 = (a->degre % 100) / 10;
      int z1 = a->degre % 10;

      int x2 = b->degre / 100;
      int y2 = (b->degre % 100) / 10;
      int z2 = b->degre % 10;

      int x = x1 + x2;
      int y = y1 + y2;
      int z = z1 + z2;

      if (x > 9 || y > 9 || z > 9)
        throw std::runtime_error("error: degre>9");

      int newDegre = x * 100 + y * 10 + z;
      double newCoef = a->coef * b->coef;

      if (fabs(newCoef) > 1e-12)
        result.insertPol(Monom(newDegre, newCoef));
      b = b->next;
    }
    a = a->next;
  }
  return result;
}

void Polinom::print() const {
  if (head->next == nullptr) {
    std::cout << "0";
    return;
  }

  Monom *curr = head->next;
  bool first = true;
  while (curr != nullptr) {
    if (first) {
      if (curr->coef < 0)
        std::cout << "-";
      first = false;
    } else {
      if (curr->coef > 0)
        std::cout << "+";
      else
        std::cout << "-";
    }

    double absCoef = fabs(curr->coef);
    if (fabs(absCoef - 1.0) > 1e-12 || curr->degre == 0) {
      std::cout << absCoef;
    }

    int x = curr->degre / 100;
    int y = (curr->degre % 100) / 10;
    int z = curr->degre % 10;

    if (x > 0)
      std::cout << "x^" << x;
    if (y > 0)
      std::cout << "y^" << y;
    if (z > 0)
      std::cout << "z^" << z;

    curr = curr->next;
  }
}

bool Polinom::operator==(const Polinom &other) const {
  Monom *a = head->next;
  Monom *b = other.head->next;

  while (a != nullptr && b != nullptr) {
    if (a->degre != b->degre)
      return false;
    if (fabs(a->coef - b->coef) > 1e-12)
      return false;
    a = a->next;
    b = b->next;
  }
  return (a == nullptr && b == nullptr);
}