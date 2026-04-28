#include "pch.h"
#include "polinom.h"
#include <gtest/gtest.h>

struct Term {
  double coef;
  int x;
  int y;
  int z;
};

Polinom makePolynomial(std::initializer_list<Term> terms) {
  Polinom p;
  for (const auto &t : terms) {
    int deg = t.x * 100 + t.y * 10 + t.z;
    p.insertPol(Monom(deg, t.coef));
  }
  return p;
}

TEST(PolinomTest, DefaultConstructor_SizeZero) {
  Polinom p;
  EXPECT_EQ(p.getSize(), 0u);
}

TEST(PolinomTest, InsertSingleMonomial_SizeIncreases) {
  Polinom p;
  p.insertPol(Monom(123, 5.0));
  EXPECT_EQ(p.getSize(), 1u);
}

TEST(PolinomTest, InsertMultipleInOrder_ResultingPolynomial) {
  Polinom p;
  p.insertPol(Monom(321, 3.0));
  p.insertPol(Monom(210, 2.0));
  p.insertPol(Monom(100, 1.0));
  Polinom expected =
      makePolynomial({{3.0, 3, 2, 1}, {2.0, 2, 1, 0}, {1.0, 1, 0, 0}});
  EXPECT_TRUE(p == expected);
}

TEST(PolinomTest, InsertCombinesSameDegree_Size) {
  Polinom p;
  p.insertPol(Monom(111, 2.0));
  p.insertPol(Monom(111, 3.0));
  EXPECT_EQ(p.getSize(), 1u);
}

TEST(PolinomTest, InsertCombinesSameDegree_Result) {
  Polinom p;
  p.insertPol(Monom(111, 2.0));
  p.insertPol(Monom(111, 3.0));
  Polinom expected = makePolynomial({{5.0, 1, 1, 1}});
  EXPECT_TRUE(p == expected);
}

TEST(PolinomTest, InsertCancelsZero_Size) {
  Polinom p;
  p.insertPol(Monom(222, 4.0));
  p.insertPol(Monom(222, -4.0));
  EXPECT_EQ(p.getSize(), 0u);
}

TEST(PolinomTest, InsertCancelsZero_ResultEmpty) {
  Polinom p;
  p.insertPol(Monom(222, 4.0));
  p.insertPol(Monom(222, -4.0));
  Polinom empty;
  EXPECT_TRUE(p == empty);
}

TEST(PolinomTest, InsertIgnoresZeroCoefficient) {
  Polinom p;
  p.insertPol(Monom(333, 0.0));
  EXPECT_EQ(p.getSize(), 0u);
}

TEST(PolinomTest, RemoveZeroAfterCombine_Size) {
  Polinom p;
  p.insertPol(Monom(123, 2.5));
  p.insertPol(Monom(123, -2.5));
  EXPECT_EQ(p.getSize(), 0u);
}

TEST(PolinomTest, RemoveZeroAfterCombine_ResultEmpty) {
  Polinom p;
  p.insertPol(Monom(123, 2.5));
  p.insertPol(Monom(123, -2.5));
  Polinom empty;
  EXPECT_TRUE(p == empty);
}

TEST(PolinomTest, RemoveZeroFunction_Equality) {
  Polinom p = makePolynomial({{1.0, 0, 0, 0}, {0.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  p.removeZero();
  Polinom expected = makePolynomial({{1.0, 0, 0, 0}, {2.0, 2, 0, 0}});
  EXPECT_TRUE(p == expected);
}

TEST(PolinomTest, RemoveZeroFunction_Size) {
  Polinom p = makePolynomial({{1.0, 0, 0, 0}, {0.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  p.removeZero();
  EXPECT_EQ(p.getSize(), 2u);
}

TEST(PolinomTest, CopyConstructor_Equality) {
  Polinom orig = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom copy(orig);
  EXPECT_TRUE(copy == orig);
}

TEST(PolinomTest, CopyConstructor_ModifyOriginalNotAffectCopy) {
  Polinom orig = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom copy(orig);
  orig.insertPol(Monom(100, 5.0));
  EXPECT_FALSE(copy == orig);
}

TEST(PolinomTest, CopyConstructor_CopySize) {
  Polinom orig = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom copy(orig);
  EXPECT_EQ(copy.getSize(), 2u);
}

TEST(PolinomTest, AssignmentOperator_Equality) {
  Polinom a = makePolynomial({{3.0, 3, 0, 0}});
  Polinom b = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  a = b;
  EXPECT_TRUE(a == b);
}

TEST(PolinomTest, AssignmentOperator_SelfAssignment) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  a = a;
  Polinom expected = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  EXPECT_TRUE(a == expected);
}

// ---------- Addition ----------

TEST(PolinomTest, AddDisjointDegrees_Result) {
  Polinom a = makePolynomial({{2.0, 2, 0, 0}});
  Polinom b = makePolynomial({{3.0, 0, 3, 0}});
  Polinom sum = a.add(b);
  Polinom expected = makePolynomial({{2.0, 2, 0, 0}, {3.0, 0, 3, 0}});
  EXPECT_TRUE(sum == expected);
}

TEST(PolinomTest, AddDisjointDegrees_Size) {
  Polinom a = makePolynomial({{2.0, 2, 0, 0}});
  Polinom b = makePolynomial({{3.0, 0, 3, 0}});
  Polinom sum = a.add(b);
  EXPECT_EQ(sum.getSize(), 2u);
}

TEST(PolinomTest, AddOverlappingDegrees_Result) {
  Polinom a = makePolynomial({{1.0, 1, 1, 0}, {2.0, 2, 0, 1}});
  Polinom b = makePolynomial({{3.0, 1, 1, 0}, {4.0, 0, 0, 0}});
  Polinom sum = a.add(b);
  Polinom expected =
      makePolynomial({{4.0, 1, 1, 0}, {2.0, 2, 0, 1}, {4.0, 0, 0, 0}});
  EXPECT_TRUE(sum == expected);
}

TEST(PolinomTest, AddOverlappingDegrees_Size) {
  Polinom a = makePolynomial({{1.0, 1, 1, 0}, {2.0, 2, 0, 1}});
  Polinom b = makePolynomial({{3.0, 1, 1, 0}, {4.0, 0, 0, 0}});
  Polinom sum = a.add(b);
  EXPECT_EQ(sum.getSize(), 3u);
}

TEST(PolinomTest, AddCancellation_Size) {
  Polinom a = makePolynomial({{5.0, 1, 1, 1}});
  Polinom b = makePolynomial({{-5.0, 1, 1, 1}});
  Polinom sum = a.add(b);
  EXPECT_EQ(sum.getSize(), 0u);
}

TEST(PolinomTest, AddCancellation_ResultEmpty) {
  Polinom a = makePolynomial({{5.0, 1, 1, 1}});
  Polinom b = makePolynomial({{-5.0, 1, 1, 1}});
  Polinom sum = a.add(b);
  Polinom empty;
  EXPECT_TRUE(sum == empty);
}

TEST(PolinomTest, AddWithZero_LeftZero) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom zero;
  Polinom sum = zero.add(a);
  EXPECT_TRUE(sum == a);
}

TEST(PolinomTest, AddWithZero_RightZero) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom zero;
  Polinom sum = a.add(zero);
  EXPECT_TRUE(sum == a);
}

TEST(PolinomTest, SubtractDisjoint_Result) {
  Polinom a = makePolynomial({{2.0, 2, 0, 0}});
  Polinom b = makePolynomial({{3.0, 0, 3, 0}});
  Polinom diff = a.subtract(b);
  Polinom expected = makePolynomial({{2.0, 2, 0, 0}, {-3.0, 0, 3, 0}});
  EXPECT_TRUE(diff == expected);
}

TEST(PolinomTest, SubtractDisjoint_Size) {
  Polinom a = makePolynomial({{2.0, 2, 0, 0}});
  Polinom b = makePolynomial({{3.0, 0, 3, 0}});
  Polinom diff = a.subtract(b);
  EXPECT_EQ(diff.getSize(), 2u);
}

TEST(PolinomTest, SubtractOverlap_Result) {
  Polinom a = makePolynomial({{5.0, 1, 1, 0}, {2.0, 0, 0, 0}});
  Polinom b = makePolynomial({{3.0, 1, 1, 0}, {1.0, 0, 0, 0}});
  Polinom diff = a.subtract(b);
  Polinom expected = makePolynomial({{2.0, 1, 1, 0}, {1.0, 0, 0, 0}});
  EXPECT_TRUE(diff == expected);
}

TEST(PolinomTest, SubtractOverlap_Size) {
  Polinom a = makePolynomial({{5.0, 1, 1, 0}, {2.0, 0, 0, 0}});
  Polinom b = makePolynomial({{3.0, 1, 1, 0}, {1.0, 0, 0, 0}});
  Polinom diff = a.subtract(b);
  EXPECT_EQ(diff.getSize(), 2u);
}

TEST(PolinomTest, SubtractCancellation_Size) {
  Polinom a = makePolynomial({{4.0, 2, 2, 2}});
  Polinom b = makePolynomial({{4.0, 2, 2, 2}});
  Polinom diff = a.subtract(b);
  EXPECT_EQ(diff.getSize(), 0u);
}

TEST(PolinomTest, SubtractCancellation_ResultEmpty) {
  Polinom a = makePolynomial({{4.0, 2, 2, 2}});
  Polinom b = makePolynomial({{4.0, 2, 2, 2}});
  Polinom diff = a.subtract(b);
  Polinom empty;
  EXPECT_TRUE(diff == empty);
}

TEST(PolinomTest, MultiplyByConstantPositive_Result) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom prod = a.mulonconst(3.0);
  Polinom expected = makePolynomial({{3.0, 1, 0, 0}, {6.0, 0, 2, 0}});
  EXPECT_TRUE(prod == expected);
}

TEST(PolinomTest, MultiplyByConstantPositive_Size) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom prod = a.mulonconst(3.0);
  EXPECT_EQ(prod.getSize(), 2u);
}

TEST(PolinomTest, MultiplyByConstantNegative_Result) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom prod = a.mulonconst(-2.0);
  Polinom expected = makePolynomial({{-2.0, 1, 0, 0}});
  EXPECT_TRUE(prod == expected);
}

TEST(PolinomTest, MultiplyByConstantNegative_Size) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom prod = a.mulonconst(-2.0);
  EXPECT_EQ(prod.getSize(), 1u);
}

TEST(PolinomTest, MultiplyByConstantZero_Size) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom prod = a.mulonconst(0.0);
  EXPECT_EQ(prod.getSize(), 0u);
}

TEST(PolinomTest, MultiplyByConstantZero_ResultEmpty) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom prod = a.mulonconst(0.0);
  Polinom empty;
  EXPECT_TRUE(prod == empty);
}

TEST(PolinomTest, MultiplySingleMonomials_Result) {
  Polinom a = makePolynomial({{2.0, 1, 2, 0}});
  Polinom b = makePolynomial({{3.0, 0, 1, 3}});
  Polinom prod = a.mul(b);
  Polinom expected = makePolynomial({{6.0, 1, 3, 3}});
  EXPECT_TRUE(prod == expected);
}

TEST(PolinomTest, MultiplySingleMonomials_Size) {
  Polinom a = makePolynomial({{2.0, 1, 2, 0}});
  Polinom b = makePolynomial({{3.0, 0, 1, 3}});
  Polinom prod = a.mul(b);
  EXPECT_EQ(prod.getSize(), 1u);
}

TEST(PolinomTest, MultiplyPolynomials_Result) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 1, 0}});
  Polinom b = makePolynomial({{3.0, 0, 0, 1}, {4.0, 1, 1, 0}});
  Polinom prod = a.mul(b);
  Polinom expected = makePolynomial(
      {{3.0, 1, 0, 1}, {4.0, 2, 1, 0}, {6.0, 0, 1, 1}, {8.0, 1, 2, 0}});
  EXPECT_TRUE(prod == expected);
}

TEST(PolinomTest, MultiplyPolynomials_Size) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 1, 0}});
  Polinom b = makePolynomial({{3.0, 0, 0, 1}, {4.0, 1, 1, 0}});
  Polinom prod = a.mul(b);
  EXPECT_EQ(prod.getSize(), 4u);
}

TEST(PolinomTest, MultiplyByZero_LeftZero) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom zero;
  Polinom prod = zero.mul(a);
  EXPECT_EQ(prod.getSize(), 0u);
}

TEST(PolinomTest, MultiplyByZero_RightZero) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom zero;
  Polinom prod = a.mul(zero);
  EXPECT_EQ(prod.getSize(), 0u);
}

TEST(PolinomTest, MultiplyDegreeExceedsLimitThrows) {
  Polinom a = makePolynomial({{1.0, 9, 0, 0}});
  Polinom b = makePolynomial({{1.0, 1, 0, 0}});
  EXPECT_THROW(a.mul(b), std::runtime_error);
}

TEST(PolinomTest, MultiplyResultCancellation_Result) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {1.0, 0, 1, 0}});
  Polinom b = makePolynomial({{1.0, 1, 0, 0}, {-1.0, 0, 1, 0}});
  Polinom prod = a.mul(b);
  Polinom expected = makePolynomial({{1.0, 2, 0, 0}, {-1.0, 0, 2, 0}});
  EXPECT_TRUE(prod == expected);
}

TEST(PolinomTest, MultiplyResultCancellation_Size) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {1.0, 0, 1, 0}});
  Polinom b = makePolynomial({{1.0, 1, 0, 0}, {-1.0, 0, 1, 0}});
  Polinom prod = a.mul(b);
  EXPECT_EQ(prod.getSize(), 2u);
}

TEST(PolinomTest, EqualitySame_True) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom b = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  EXPECT_TRUE(a == b);
}

TEST(PolinomTest, EqualityDifferentCoefficient_False) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom b = makePolynomial({{2.0, 1, 0, 0}});
  EXPECT_FALSE(a == b);
}

TEST(PolinomTest, EqualityDifferentDegree_False) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom b = makePolynomial({{1.0, 2, 0, 0}});
  EXPECT_FALSE(a == b);
}

TEST(PolinomTest, EqualityDifferentSize_False) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom b = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 1, 0}});
  EXPECT_FALSE(a == b);
}

TEST(PolinomTest, EqualityBothEmpty_True) {
  Polinom a, b;
  EXPECT_TRUE(a == b);
}

TEST(PolinomTest, ClearPolynomial_SizeZero) {
  Polinom p = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  p.clear();
  EXPECT_EQ(p.getSize(), 0u);
}

TEST(PolinomTest, ClearPolynomial_ResultEmpty) {
  Polinom p = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  p.clear();
  Polinom empty;
  EXPECT_TRUE(p == empty);
}

TEST(PolinomTest, ClearPolynomial_CanInsertAfter) {
  Polinom p = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  p.clear();
  p.insertPol(Monom(100, 5.0));
  EXPECT_EQ(p.getSize(), 1u);
}

TEST(PolinomTest, InsertAfterRemoveZero_Size) {
  Polinom p;
  p.insertPol(Monom(111, 1.0));
  p.insertPol(Monom(111, -1.0));
  p.insertPol(Monom(222, 2.0));
  EXPECT_EQ(p.getSize(), 1u);
}

TEST(PolinomTest, InsertAfterRemoveZero_Result) {
  Polinom p;
  p.insertPol(Monom(111, 1.0));
  p.insertPol(Monom(111, -1.0));
  p.insertPol(Monom(222, 2.0));
  Polinom expected = makePolynomial({{2.0, 2, 2, 2}});
  EXPECT_TRUE(p == expected);
}

TEST(PolinomTest, LargePolynomialManyTerms_SizePositive) {
  Polinom p;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      for (int k = 0; k < 10; ++k) {
        if (i + j + k <= 9) {
          int deg = i * 100 + j * 10 + k;
          p.insertPol(Monom(deg, 1.0));
        }
      }
    }
  }
  EXPECT_GT(p.getSize(), 0u);
}

TEST(PolinomTest, AdditionWithLargePolynomials_Result) {
  Polinom a = makePolynomial({{1.0, 9, 0, 0}});
  Polinom b = makePolynomial({{2.0, 0, 9, 0}});
  Polinom sum = a.add(b);
  Polinom expected = makePolynomial({{1.0, 9, 0, 0}, {2.0, 0, 9, 0}});
  EXPECT_TRUE(sum == expected);
}

TEST(PolinomTest, SubtractFromItself_SizeZero) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom zero = a.subtract(a);
  EXPECT_EQ(zero.getSize(), 0u);
}

TEST(PolinomTest, SubtractFromItself_ResultEmpty) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 2, 0}});
  Polinom zero = a.subtract(a);
  Polinom empty;
  EXPECT_TRUE(zero == empty);
}

TEST(PolinomTest, MultiplyByConstantAndThenAdd_Result) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom b = a.mulonconst(2.0);
  Polinom c = a.add(b);
  Polinom expected = makePolynomial({{3.0, 1, 0, 0}});
  EXPECT_TRUE(c == expected);
}

TEST(PolinomTest, InsertDescendingOrderMaintained_Result) {
  Polinom p;
  p.insertPol(Monom(300, 3.0));
  p.insertPol(Monom(100, 1.0));
  p.insertPol(Monom(200, 2.0));
  Polinom expected =
      makePolynomial({{3.0, 3, 0, 0}, {2.0, 2, 0, 0}, {1.0, 1, 0, 0}});
  EXPECT_TRUE(p == expected);
}

TEST(PolinomTest, GetSizeAfterOperations_InitialSize) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  EXPECT_EQ(a.getSize(), 2u);
}

TEST(PolinomTest, GetSizeAfterOperations_InsertExisting) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  a.insertPol(Monom(100, 3.0));
  EXPECT_EQ(a.getSize(), 2u);
}

TEST(PolinomTest, GetSizeAfterOperations_InsertNew) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  a.insertPol(Monom(300, 4.0));
  EXPECT_EQ(a.getSize(), 3u);
}

TEST(PolinomTest, GetSizeAfterOperations_RemoveZeroNoChange) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 2, 0, 0}});
  a.insertPol(Monom(300, 4.0));
  a.removeZero();
  EXPECT_EQ(a.getSize(), 3u);
}

TEST(PolinomTest, MultiplyDegreeExceedsLimitInCombination_Throws) {
  Polinom a = makePolynomial({{1.0, 5, 5, 0}});
  Polinom b = makePolynomial({{1.0, 5, 5, 0}});
  EXPECT_THROW(a.mul(b), std::runtime_error);
}

TEST(PolinomTest, RemoveZeroMultiple_Equality) {
  Polinom p = makePolynomial({{1.0, 1, 0, 0},
                              {0.0, 2, 0, 0},
                              {2.0, 3, 0, 0},
                              {0.0, 4, 0, 0},
                              {3.0, 5, 0, 0}});
  p.removeZero();
  Polinom expected =
      makePolynomial({{1.0, 1, 0, 0}, {2.0, 3, 0, 0}, {3.0, 5, 0, 0}});
  EXPECT_TRUE(p == expected);
}

TEST(PolinomTest, RemoveZeroMultiple_Size) {
  Polinom p = makePolynomial({{1.0, 1, 0, 0},
                              {0.0, 2, 0, 0},
                              {2.0, 3, 0, 0},
                              {0.0, 4, 0, 0},
                              {3.0, 5, 0, 0}});
  p.removeZero();
  EXPECT_EQ(p.getSize(), 3u);
}

TEST(PolinomTest, SubtractionEqualsAddNegative) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom b = makePolynomial({{2.0, 2, 0, 0}});
  Polinom sub = a.subtract(b);
  Polinom negB = b.mulonconst(-1.0);
  Polinom addNeg = a.add(negB);
  EXPECT_TRUE(sub == addNeg);
}

TEST(PolinomTest, EqualityTolerance_True) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom b = makePolynomial({{1.0 + 1e-13, 1, 0, 0}});
  EXPECT_TRUE(a == b);
}

TEST(PolinomTest, EqualityOutsideTolerance_False) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}});
  Polinom b = makePolynomial({{1.0 + 1e-11, 1, 0, 0}});
  EXPECT_FALSE(a == b);
}

TEST(PolinomTest, InsertBelowToleranceRemoves_Size) {
  Polinom p;
  p.insertPol(Monom(111, 1e-11));
  p.insertPol(Monom(111, -1e-11));
  EXPECT_EQ(p.getSize(), 0u);
}

TEST(PolinomTest, InsertBelowToleranceRemoves_ResultEmpty) {
  Polinom p;
  p.insertPol(Monom(111, 1e-11));
  p.insertPol(Monom(111, -1e-11));
  Polinom empty;
  EXPECT_TRUE(p == empty);
}

TEST(PolinomTest, MultiplicationCommutative) {
  Polinom a = makePolynomial({{1.0, 1, 0, 0}, {2.0, 0, 1, 0}});
  Polinom b = makePolynomial({{3.0, 0, 0, 1}, {4.0, 1, 1, 0}});
  Polinom ab = a.mul(b);
  Polinom ba = b.mul(a);
  EXPECT_TRUE(ab == ba);
}