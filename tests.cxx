#define private public
#include "doc.hxx"
#undef private

#include <cassert>
#include <iomanip>
#include <iostream>

#define operator_assert(L, OP, R)                                              \
  {                                                                            \
    if (!(L OP R)) {                                                           \
      std::cout << "ASSERT(" << #L << " " << #OP << " " << #R ") failed on "   \
                << __FILE__ << ":" << __LINE__ << std::endl                    \
                << "  " << std::quoted(#L) << " = " << L << std::endl          \
                << "  " << std::quoted(#R) << " = " << R << std::endl;         \
    }                                                                          \
    assert((L OP R));                                                          \
  }

#define not_operator_assert(L, OP, R)                                          \
  {                                                                            \
    if ((L OP R)) {                                                            \
      std::cout << "ASSERT(" << #L << " " << #OP << " " << #R ") failed on "   \
                << __FILE__ << ":" << __LINE__ << std::endl                    \
                << "  " << std::quoted(#L) << " = " << L << std::endl          \
                << "  " << std::quoted(#R) << " = " << R << std::endl;         \
    }                                                                          \
    assert(!(L OP R));                                                         \
  }

#define assert_cond(COND)                                                      \
  {                                                                            \
    if (!(COND)) {                                                             \
      std::cout << "ASSERT(" << #COND << ") failed on " << __FILE__ << ":"     \
                << __LINE__ << std::endl;                                      \
    }                                                                          \
    assert(COND);                                                              \
  }

using namespace linedoc;

int main() {

  doc test;
  test.push_back("a");
  test.push_back("b");
  test.push_back("c");

  {
    // START begin tests
    doc_line_point begin = test.begin();
    operator_assert(begin.line_no, ==, 0);
    operator_assert(begin.character, ==, 0);
  } // END begin tests

  { // START end tests
    doc_line_point end = test.end();
    operator_assert(end.line_no, ==, 2);
    operator_assert(end.character, ==, std::basic_string<char>::npos);
  } // END end tests

  { // START empty_begin tests
    doc test;
    test.push_back("");
    test.push_back("b");
    test.push_back("c");

    doc_line_point begin = test.begin();
    operator_assert(begin.line_no, ==, 0);
    operator_assert(begin.character, ==, std::basic_string<char>::npos);
  } // END empty_begin tests

  { // START empty_end tests
    doc test;
    test.push_back("a");
    test.push_back("b");
    test.push_back("");

    doc_line_point end = test.end();
    operator_assert(end.line_no, ==, 2);
    operator_assert(end.character, ==, std::basic_string<char>::npos);
  } // END empty_end tests

  { // START are_equivalent tests
    assert_cond(
        test.are_equivalent(doc_line_point{1, 0}, doc_line_point{1, 0}));
    assert_cond(
        test.are_equivalent(doc_line_point{1, 1}, doc_line_point{1, 2}));
    assert_cond(
        test.are_equivalent(doc_line_point{2, 1}, doc_line_point{1000, 1000}));
    assert_cond(test.are_equivalent(test.end(), doc_line_point{1000, 1000}));
    assert_cond(test.are_equivalent(doc_line_point{2, 1}, test.end()));
    assert_cond(
        test.are_equivalent(doc_line_point{2, std::string::npos}, test.end()));
  } // END are_equivalent tests

  { // START is_earlier tests
    assert_cond(test.is_earlier(doc_line_point{1, 0}, doc_line_point{1, 1}));
    assert_cond(test.is_earlier(doc_line_point{1, 1}, doc_line_point{2, 0}));
    assert_cond(
        test.is_earlier(doc_line_point{1, 1}, doc_line_point{1000, 1000}));
    assert_cond(test.is_earlier(test.begin(), test.end()));

    assert_cond(!test.is_earlier(doc_line_point{1, 0}, doc_line_point{1, 0}));
    assert_cond(!test.is_earlier(doc_line_point{1, 1}, doc_line_point{1, 1}));
    assert_cond(
        !test.is_earlier(doc_line_point{1, 1000}, doc_line_point{1, 1000}));
    assert_cond(!test.is_earlier(doc_line_point{1000, 1000},
                                 doc_line_point{1000, 1000}));
  } // END is_earlier tests

  { // START is_later tests
    assert_cond(test.is_later(doc_line_point{1, 1}, doc_line_point{1, 0}));
    assert_cond(test.is_later(doc_line_point{2, 1}, doc_line_point{1, 1}));
    assert_cond(
        test.is_later(doc_line_point{1000, 1000}, doc_line_point{1, 1}));
    assert_cond(test.is_later(test.end(), test.begin()));

    assert_cond(!test.is_later(doc_line_point{1, 0}, doc_line_point{1, 0}));
    assert_cond(!test.is_later(doc_line_point{1, 1}, doc_line_point{1, 1}));
    assert_cond(
        !test.is_later(doc_line_point{1, 1000}, doc_line_point{1, 1000}));
    assert_cond(
        !test.is_later(doc_line_point{1000, 1000}, doc_line_point{1000, 1000}));

  } // END is_later tests

  { // START empty_doc tests
    doc test;
    doc_line_point begin = test.begin();
    doc_line_point end = test.end();

    operator_assert(begin.line_no, ==, std::numeric_limits<size_t>::max());
    operator_assert(begin.character, ==, std::basic_string<char>::npos);

    operator_assert(end.line_no, ==, std::numeric_limits<size_t>::max());
    operator_assert(end.character, ==, std::basic_string<char>::npos);

    assert_cond(test.are_equivalent(begin, end));
    assert_cond(!test.n_lines());
  } // END empty_doc tests

  { // START advance tests
    doc_line_point begin_adv = test.advance(test.begin());
    doc_line_point begin_adv2 = test.advance(test.begin(), 2);
    doc_line_point begin_adv3 = test.advance(test.begin(), 3);
    doc_line_point begin_adv4 = test.advance(test.begin(), 4);
    doc_line_point begin_adv5 = test.advance(test.begin(), 5);
    doc_line_point begin_adv1000 = test.advance(test.begin(), 1000);
    doc_line_point end_adv = test.advance(test.end());
    doc_line_point last_char_adv = test.advance(doc_line_point{2, 0});

    assert_cond(test.are_equivalent(begin_adv, doc_line_point{0, 1}));
    assert_cond(
        test.are_equivalent(begin_adv, doc_line_point{0, std::string::npos}));
    assert_cond(test.are_equivalent(begin_adv2, doc_line_point{1, 0}));
    assert_cond(
        test.are_equivalent(begin_adv3, doc_line_point{1, std::string::npos}));
    assert_cond(test.are_equivalent(begin_adv4, doc_line_point{2, 0}));
    assert_cond(
        test.are_equivalent(begin_adv5, doc_line_point{2, std::string::npos}));
    assert_cond(test.are_equivalent(begin_adv1000, begin_adv5));
    assert_cond(test.are_equivalent(begin_adv1000, test.end()));
    assert_cond(test.are_equivalent(end_adv, test.end()));
    assert_cond(test.are_equivalent(end_adv, last_char_adv));
    assert_cond(test.are_equivalent(test.end(), last_char_adv));
  } // END advance tests
  { // START rewind tests
    doc_line_point end_rew = test.rewind(test.end());
    doc_line_point end_rew2 = test.rewind(test.end(), 2);
    doc_line_point end_rew3 = test.rewind(test.end(), 3);
    doc_line_point end_rew4 = test.rewind(test.end(), 4);
    doc_line_point end_rew5 = test.rewind(test.end(), 5);
    doc_line_point end_rew1000 = test.rewind(test.end(), 1000);
    doc_line_point begin_rew = test.rewind(test.begin());

    assert_cond(test.are_equivalent(end_rew, doc_line_point{2, 0}));
    assert_cond(test.are_equivalent(end_rew2, doc_line_point{1, 1}));
    assert_cond(
        test.are_equivalent(end_rew2, doc_line_point{1, std::string::npos}));
    assert_cond(test.are_equivalent(end_rew3, doc_line_point{1, 0}));
    assert_cond(test.are_equivalent(end_rew4, doc_line_point{0, 1}));
    assert_cond(test.are_equivalent(end_rew5, test.begin()));
    assert_cond(test.are_equivalent(end_rew1000, end_rew5));
    assert_cond(test.are_equivalent(end_rew1000, test.begin()));
    assert_cond(test.are_equivalent(begin_rew, test.begin()));
  } // END rewind tests
}
