#define LINEDOC_PRIVATE public
#include "linedoc/doc.hxx"
#undef LINEDOC_PRIVATE

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

#define assert_are_equiv(doc, a, b)                                            \
  {                                                                            \
    if (!(doc.are_equivalent(a, b))) {                                         \
      std::cout << "ASSERT(" << #doc << ".are_equivalent(" << #a << ", " << #b \
                << ") failed on " << __FILE__ << ":" << __LINE__ << std::endl  \
                << "\t" << #a << " = " << a << std::endl                       \
                << "\t" << #b << " = " << b << std::endl;                      \
    }                                                                          \
    assert(doc.are_equivalent(a, b));                                          \
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
    std::cout << "[PASSED] 2/2 begin tests." << std::endl;
  } // END begin tests

  { // START end tests
    doc_line_point end = test.end();
    operator_assert(end.line_no, ==, 2);
    operator_assert(end.character, ==, std::basic_string<char>::npos);
    std::cout << "[PASSED] 2/2 end tests." << std::endl;
  } // END end tests

  { // START empty_begin tests
    doc test;
    test.push_back("");
    test.push_back("b");
    test.push_back("c");

    doc_line_point begin = test.begin();
    operator_assert(begin.line_no, ==, 0);
    operator_assert(begin.character, ==, std::basic_string<char>::npos);
    std::cout << "[PASSED] 2/2 empty first line tests." << std::endl;
  } // END empty_begin tests

  { // START empty_end tests
    doc test;
    test.push_back("a");
    test.push_back("b");
    test.push_back("");

    doc_line_point end = test.end();
    operator_assert(end.line_no, ==, 2);
    operator_assert(end.character, ==, std::basic_string<char>::npos);
    std::cout << "[PASSED] 2/2 empty final line tests." << std::endl;
  } // END empty_end tests

  { // START are_equivalent tests
    assert_are_equiv(test, (doc_line_point{1, 0}), (doc_line_point{1, 0}));
    assert_are_equiv(test, (doc_line_point{1, 1}), (doc_line_point{1, 2}));
    assert_are_equiv(test, (doc_line_point{2, 1}),
                     (doc_line_point{1000, 1000}));
    assert_are_equiv(test, test.end(), (doc_line_point{1000, 1000}));
    assert_are_equiv(test, (doc_line_point{2, 1}), test.end());
    assert_are_equiv(test, (doc_line_point{2, std::string::npos}), test.end());
    std::cout << "[PASSED] 6/6 are_equivalent tests." << std::endl;
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

    // Last EOL !earlier than EOF
    assert_cond(
        !test.is_earlier(doc_line_point{2, 1}, doc_line_point{1000, 1000}));
    std::cout << "[PASSED] 9/9 is_earlier tests." << std::endl;
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
    std::cout << "[PASSED] 8/8 is_later tests." << std::endl;
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
    std::cout << "[PASSED] 6/6 empty document tests." << std::endl;
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

    assert_are_equiv(test, begin_adv, (doc_line_point{0, 1}));
    assert_are_equiv(test, begin_adv, (doc_line_point{0, std::string::npos}));
    assert_are_equiv(test, begin_adv2, (doc_line_point{1, 0}));
    assert_are_equiv(test, begin_adv3, (doc_line_point{1, std::string::npos}));
    assert_are_equiv(test, begin_adv4, (doc_line_point{2, 0}));
    assert_are_equiv(test, begin_adv5, (doc_line_point{2, std::string::npos}));
    assert_are_equiv(test, begin_adv1000, begin_adv5);
    assert_are_equiv(test, begin_adv1000, test.end());
    assert_are_equiv(test, end_adv, test.end());
    assert_are_equiv(test, end_adv, last_char_adv);
    assert_are_equiv(test, test.end(), last_char_adv);
    std::cout << "[PASSED] 11/11 advance tests." << std::endl;
  } // END advance tests
  { // START rewind tests
    doc_line_point end_rew = test.rewind(test.end());
    doc_line_point end_rew2 = test.rewind(test.end(), 2);
    doc_line_point end_rew3 = test.rewind(test.end(), 3);
    doc_line_point end_rew4 = test.rewind(test.end(), 4);
    doc_line_point end_rew5 = test.rewind(test.end(), 5);
    doc_line_point end_rew1000 = test.rewind(test.end(), 1000);
    doc_line_point begin_rew = test.rewind(test.begin());

    assert_are_equiv(test, end_rew, (doc_line_point{2, 0}));
    assert_are_equiv(test, end_rew2, (doc_line_point{1, 1}));
    assert_are_equiv(test, end_rew2, (doc_line_point{1, std::string::npos}));
    assert_are_equiv(test, end_rew3, (doc_line_point{1, 0}));
    assert_are_equiv(test, end_rew4, (doc_line_point{0, 1}));
    assert_are_equiv(test, end_rew5, test.begin());
    assert_are_equiv(test, end_rew1000, end_rew5);
    assert_are_equiv(test, end_rew1000, test.begin());
    assert_are_equiv(test, begin_rew, test.begin());
    std::cout << "[PASSED] 9/9 rewind tests." << std::endl;
  } // END rewind tests
  { // START rewind_line tests
    doc_line_point end_rew_l = test.rewind_line(test.end());
    doc_line_point begin_rew_l = test.rewind_line(test.begin());
    doc_line_point begin_EOL_rew_l = test.rewind_line(test.begin().get_EOL());
    doc_line_point first_rew2_l = test.rewind_line(doc_line_point{1, 0}, 2);

    assert_are_equiv(test, end_rew_l, (doc_line_point{1, 0}.get_EOL()));
    assert_are_equiv(test, begin_rew_l, test.begin());
    assert_are_equiv(test, begin_EOL_rew_l, test.begin());
    assert_are_equiv(test, first_rew2_l, test.begin());
    std::cout << "[PASSED] 4/4 rewind_line tests." << std::endl;
  } // END rewind_line tests
  { // START find_first_of
    doc test;
    test.push_back("a");
    test.push_back("b");
    test.push_back("c");
    test.push_back("efg");
    doc_line_point begin = test.begin();

    doc_line_point next_a = test.find_first_of("a", begin, test.end());
    doc_line_point next_nl = test.find_first_of("\n", begin, test.end());
    doc_line_point next_nl_same_line =
        test.find_first_of("\n", begin.get_EOL(), test.end());
    doc_line_point next_b = test.find_first_of("b", begin, test.end());
    doc_line_point next_b_same_line =
        test.find_first_of("b", begin, begin.get_EOL());
    doc_line_point next_c = test.find_first_of("c", begin, test.end());
    doc_line_point next_d = test.find_first_of("d", begin, test.end());
    doc_line_point next_f =
        test.find_first_of("f", doc_line_point{3, 0}, test.end());
    doc_line_point next_f_miss =
        test.find_first_of("f", doc_line_point{3, 2}, test.end());
    doc_line_point swap = test.find_first_of("X", test.end(), test.begin());

    assert_are_equiv(test, next_a, (doc_line_point{0, 0}));
    assert_are_equiv(test, next_nl, (doc_line_point{0, 1}));
    assert_are_equiv(test, next_nl_same_line, (doc_line_point{0, 1}));
    assert_are_equiv(test, next_b, (doc_line_point{1, 0}));
    assert_are_equiv(test, next_b_same_line, test.end());
    assert_are_equiv(test, next_c, (doc_line_point{2, 0}));
    assert_are_equiv(test, next_d, test.end());
    assert_are_equiv(test, next_f, (doc_line_point{3, 1}));
    assert_are_equiv(test, next_f_miss, test.end());
    assert_are_equiv(test, swap, test.end());
    std::cout << "[PASSED] 10/10 find_first_of tests." << std::endl;
  } // END find_first_of
  { // START find_first_not_of
    doc test;
    test.push_back("a");
    test.push_back("b");
    test.push_back("c");
    test.push_back("efg");
    doc_line_point begin = test.begin();

    doc_line_point next_not_a = test.find_first_not_of("a", begin, test.end());
    doc_line_point next_not_nl = test.find_first_not_of("\n", begin.get_EOL());
    doc_line_point next_not_a_nl =
        test.find_first_not_of("a\n", begin, test.end());
    doc_line_point next_not_f =
        test.find_first_not_of("f", doc_line_point{3, 1}, test.end());

    assert_are_equiv(test, next_not_a, (doc_line_point{0, 1}));
    assert_are_equiv(test, next_not_nl, (doc_line_point{1, 0}));
    assert_are_equiv(test, next_not_a_nl, (doc_line_point{1, 0}));
    assert_are_equiv(test, next_not_f, (doc_line_point{3, 2}));
    std::cout << "[PASSED] 4/4 find_first_not_of tests." << std::endl;
  } // END find_first_not_of
  { // START find_last_of
    doc test;
    test.push_back("a");
    test.push_back("b");
    test.push_back("c");
    test.push_back("effffg");
    doc_line_point begin = test.begin();

    doc_line_point last_a = test.find_last_of("a", begin, test.end());
    doc_line_point last_b = test.find_last_of("b", begin, test.end());
    doc_line_point last_c = test.find_last_of("c", begin, test.end());
    doc_line_point last_f = test.find_last_of("f", begin, test.end());
    doc_line_point middle_f =
        test.find_last_of("f", begin, doc_line_point{3, 4});
    doc_line_point last_nl = test.find_last_of("\n", begin, test.end());
    doc_line_point last_nl2 =
        test.find_last_of("\n", begin, doc_line_point{0, 1});
    doc_line_point last_nl3 =
        test.find_last_of("\n", begin, doc_line_point{0, std::string::npos});
    doc_line_point last_nl4 =
        test.find_last_of("\n", begin, doc_line_point{1, 0});

    assert_are_equiv(test, last_a, (doc_line_point{0, 0}));
    assert_are_equiv(test, last_b, (doc_line_point{1, 0}));
    assert_are_equiv(test, last_c, (doc_line_point{2, 0}));
    assert_are_equiv(test, last_f, (doc_line_point{3, 4}));
    assert_are_equiv(test, middle_f, (doc_line_point{3, 3}));
    assert_are_equiv(test, last_nl, (doc_line_point{2, std::string::npos}));
    assert_are_equiv(test, last_nl2, test.end());
    assert_are_equiv(test, last_nl3, test.end());
    assert_are_equiv(test, last_nl4, (doc_line_point{0, 1}));
    std::cout << "[PASSED] 9/9 find_last_of tests." << std::endl;
  } // END find_last_of
  { // START find_last_not_of
    doc test;
    test.push_back("a");
    test.push_back("b");
    test.push_back("c");
    test.push_back("effffg");
    doc_line_point begin = test.begin();

    doc_line_point last_not_a = test.find_last_not_of("a", begin, test.end());
    doc_line_point last_not_a2 =
        test.find_last_not_of("a", begin, test.begin().get_EOL());
    doc_line_point last_not_g = test.find_last_not_of("g", begin, test.end());
    doc_line_point last_not_f =
        test.find_last_not_of("f", begin, doc_line_point{3, 3});
    doc_line_point last_not_nl =
        test.find_last_not_of("\n", begin, begin.get_EOL());
    doc_line_point last_not_nl2 =
        test.find_last_not_of("\n", begin.get_EOL(), doc_line_point{1, 0});

    assert_are_equiv(test, last_not_a, (doc_line_point{3, 5}));
    assert_are_equiv(test, last_not_a2, test.end());
    assert_are_equiv(test, last_not_g, (doc_line_point{3, 4}));
    assert_are_equiv(test, last_not_f, (doc_line_point{3, 0}));
    assert_are_equiv(test, last_not_nl, begin);
    assert_are_equiv(test, last_not_nl2, test.end());
    std::cout << "[PASSED] 6/6 find_last_not_of tests." << std::endl;
  } // END find_last_not_of
  { // START substr
    std::string str1 = test.substr(test.begin(),test.begin().get_EOL());
    std::string str2 = test.substr(test.begin(),doc_line_point{1, 0});
    std::string str3 = test.substr(test.begin(),test.end());
    std::string str4 = test.substr(test.end(), test.begin());
    std::string str5 = test.substr(test.begin(),test.begin());

    operator_assert(str1,==,"a");
    operator_assert(str2,==,"a ");
    operator_assert(str3,==,"a b c");
    operator_assert(str4,==,"");
    operator_assert(str5,==,"");
    std::cout << "[PASSED] 5/5 substr tests." << std::endl;
  } // END substr
  { // START substr
    doc sub_doc1 = test.subdoc(test.begin(),test.begin().get_EOL());
    doc sub_doc2 = test.subdoc(test.begin(),doc_line_point{1, 0});
    doc sub_doc3 = test.subdoc(test.begin(),test.begin());
    doc sub_doc4 = test.subdoc(test.begin(),doc_line_point{2, 0});

    operator_assert(sub_doc1.substr(),==,"a");
    operator_assert(sub_doc2.substr(),==,"a ");
    operator_assert(sub_doc3.substr(),==,"");
    operator_assert(sub_doc4.substr(),==,"a b ");
    std::cout << "[PASSED] 4/4 subdoc tests." << std::endl;
  } // END substr
}
