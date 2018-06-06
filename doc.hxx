#ifndef LINEDOC_DOC_HXX_SEEN
#define LINEDOC_DOC_HXX_SEEN

#include "doc_line.hxx"
#include "doc_line_point.hxx"
#include "doc_range.hxx"

#include <string>
#include <vector>

namespace linedoc {
template <typename T> class doc_ : private std::vector<doc_line_<T>> {
private:
  std::vector<std::basic_string<T>> filenames;
  /// Check whether line_no or character valid, update to special values if not.

  inline doc_line_point_<T> validate_line_point(doc_line_point_<T>) const;

  /// Move to the next character
  ///
  ///\note newline counts as a character
  ///
  ///\note If n = 0, do nothing.

  inline doc_line_point_<T> advance(doc_line_point_<T>, size_t n = 1) const;
  /// Move to the beginning of the line n lines after this.
  ///
  ///\note If n = 0, do nothing.

  inline doc_line_point_<T> advance_line(doc_line_point_<T>,
                                         size_t n = 1) const;

  /// Move to the previous character
  ///
  ///\note newline counts as a character
  ///
  ///\note If n = 0, do nothing.

  inline doc_line_point_<T> rewind(doc_line_point_<T>, size_t n = 1) const;
  /// Move to the end of the previous line
  ///
  ///\note If n = 0, do nothing.

  inline doc_line_point_<T> rewind_line(doc_line_point_<T>, size_t n = 1) const;

  inline bool line_is_empty(doc_line_point_<T>) const;

  inline size_t get_filename_id(std::basic_string<T> const &filename = "");

public:
  /// First character of the document

  inline doc_line_point_<T> begin() const;
  /// Last character of the document
  inline doc_line_point_<T> end() const;

  /// Whether two line_points are equivalent

  inline bool are_equivalent(doc_line_point_<T>, doc_line_point_<T>) const;
  /// Whether one line_point is earlier than another

  inline bool is_earlier(doc_line_point_<T>, doc_line_point_<T>) const;
  /// Whether one line_point is later than another

  inline bool is_later(doc_line_point_<T>, doc_line_point_<T>) const;

  /// Whether a line_point is equivalent to the beginning of the document.
  inline bool is_begin(doc_line_point_<T>) const;
  /// Whether a line_point is equivalent to the end of the document.
  inline bool is_end(doc_line_point_<T>) const;

  /// The number of lines in this document
  inline size_t n_lines() const;

  /// Find the first occurence of a character in the passed basic_string
  inline doc_line_point_<T>
  find_first_of(std::basic_string<T> const &,
                doc_line_point_<T> start = doc_line_point_<T>::begin(),
                doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the first occurence of a character not in the passed basic_string

  inline doc_line_point_<T>
  find_first_not_of(std::basic_string<T> const &,
                    doc_line_point_<T> start = doc_line_point_<T>::begin(),
                    doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the last occurence of a character in the passed basic_string

  inline doc_line_point_<T>
  find_last_of(std::basic_string<T> const &,
               doc_line_point_<T> start = doc_line_point_<T>::begin(),
               doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the last occurence of a character not in the passed basic_string

  inline doc_line_point_<T>
  find_last_not_of(std::basic_string<T> const &,
                   doc_line_point_<T> start = doc_line_point_<T>::begin(),
                   doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Get a document spanning the passed range
  inline doc_<T> subdoc(doc_range_<T>) const;

  /// Get a string spanning the passed range
  ///
  /// \note newlines are converted to a single space
  inline std::basic_string<T> substr(doc_range_<T>) const;

  inline std::basic_string<T> get_line(doc_line_point_<T>) const;

  inline std::basic_string<T> get_line_info(doc_line_point_<T>) const;

  void push_back(std::basic_string<T> const &line,
                 std::basic_string<T> const &filename = "",
                 size_t line_no = std::numeric_limits<size_t>::max());

  void push_back(std::basic_string<T> &&,
                 std::basic_string<T> const &filename = "",
                 size_t line_no = std::numeric_limits<size_t>::max());
};

typedef doc_<char> doc;
typedef doc_<wchar_t> wdoc;

template <typename T>
doc_line_point_<T> doc_<T>::validate_line_point(doc_line_point_<T> lp) const {
  // If off the end, return the EOL on the last line
  size_t size = std::vector<doc_line_<T>>::size();
  if (lp.line_no >= size) {
    if (!size) {
      return doc_line_point_<T>{std::numeric_limits<size_t>::max(),
                                std::basic_string<T>::npos};
    }
    return doc_line_point_<T>{size - 1, std::basic_string<T>::npos};
  } else if (std::vector<doc_line_<T>>::at(lp.line_no).is_EOL(lp.character)) {
    lp.character = std::string::npos;
  }
  return lp;
}

template <typename T>
doc_line_point_<T> doc_<T>::advance(doc_line_point_<T> lp, size_t n) const {
  lp = validate_line_point(lp);
  if (!n) {
    return lp;
  }

  size_t n_left = n;

  while (n_left) {
    if (lp >= end()) {
      return lp;
    }
    size_t n_in_line =
        std::vector<doc_line_<T>>::at(lp.line_no).size() - lp.character;
    if (n_left <= n_in_line) {
      lp.character += n_left;
      break;
    }
    n_left -= n_in_line;
    lp.character = std::basic_string<T>::npos;
    if (n_left) {
      n_left -= 1;
      lp = advance_line(lp);
    }
  }
  return validate_line_point(lp);
}
template <typename T>
doc_line_point_<T> doc_<T>::advance_line(doc_line_point_<T> lp,
                                         size_t n) const {
  return validate_line_point({lp.line_no + n, 0});
}
template <typename T>
doc_line_point_<T> doc_<T>::rewind(doc_line_point_<T> lp, size_t n) const {
  lp = validate_line_point(lp);

  if (!n) {
    return lp;
  }

  if (lp >= end()) {
    n -= 1;
    lp = doc_line_point_<T>{
        lp.line_no, std::vector<doc_line_<T>>::at(lp.line_no).last_char()};
  }

  size_t n_left = n;

  while (n_left) {

    if (lp == begin()) {
      return begin();
    }

    size_t n_in_line =
        (lp.character == std::basic_string<T>::npos)
            ? (std::vector<doc_line_<T>>::at(lp.line_no).last_char() + 1)
            : lp.character;

    if (n_left < n_in_line) {
      lp.character -= n_left;
      break;
    }
    n_left -= n_in_line;
    lp.character = 0;
    if (n_left) {
      n_left -= 1;
      lp = rewind_line(lp);
    }
  }

  return validate_line_point(lp);
}
template <typename T>
doc_line_point_<T> doc_<T>::rewind_line(doc_line_point_<T> lp, size_t n) const {
  if (lp.line_no < n) {
    return begin();
  }
  return validate_line_point({lp.line_no - n, std::basic_string<T>::npos});
}

template <typename T> bool doc_<T>::line_is_empty(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  if (lp == end()) {
    false;
  }
  return std::vector<doc_line_<T>>::at(lp.line_no).is_empty();
}

template <typename T>
size_t doc_<T>::get_filename_id(std::basic_string<T> const &filename) {
  if (!filename.size()) {
    return std::numeric_limits<size_t>::max();
  }
  for (size_t it = 0; it < filenames.size(); ++it) {
    if (filename == filenames[it]) {
      return it;
    }
  }
  filenames.push_back(filename);
  return (filenames.size() - 1);
}

template <typename T> doc_line_point_<T> doc_<T>::begin() const {
  return validate_line_point(doc_line_point_<T>::begin());
}
template <typename T> doc_line_point_<T> doc_<T>::end() const {
  validate_line_point(
      std::vector<doc_line_<T>>::size()
          ? doc_line_point_<T>{std::vector<doc_line_<T>>::size() - 1,
                               std::string::npos}
          : doc_line_point_<T>::begin());
}
template <typename T>
bool doc_<T>::are_equivalent(doc_line_point_<T> l, doc_line_point_<T> r) const {
  l = validate_line_point(l);
  r = validate_line_point(r);
  return (l == r);
}

template <typename T>
bool doc_<T>::is_earlier(doc_line_point_<T> l, doc_line_point_<T> r) const {
  l = validate_line_point(l);
  r = validate_line_point(r);
  return l < r;
}
template <typename T>
bool doc_<T>::is_later(doc_line_point_<T> l, doc_line_point_<T> r) const {
  l = validate_line_point(l);
  r = validate_line_point(r);
  return !(l < r) && !(l == r);
}
template <typename T> bool doc_<T>::is_begin(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  return lp == begin();
}
template <typename T> bool doc_<T>::is_end(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  return lp == end();
}

template <typename T> size_t doc_<T>::n_lines() const {
  return std::vector<doc_line_<T>>::size();
}

template <typename T>
doc_line_point_<T> doc_<T>::find_first_of(std::basic_string<T> const &,
                                          doc_line_point_<T> start,
                                          doc_line_point_<T> end) const {}
template <typename T>
doc_line_point_<T> doc_<T>::find_first_not_of(std::basic_string<T> const &,
                                              doc_line_point_<T> start,
                                              doc_line_point_<T> end) const {}
template <typename T>
doc_line_point_<T> doc_<T>::find_last_of(std::basic_string<T> const &,
                                         doc_line_point_<T> start,
                                         doc_line_point_<T> end) const {}
template <typename T>
doc_line_point_<T> doc_<T>::find_last_not_of(std::basic_string<T> const &,
                                             doc_line_point_<T> start,
                                             doc_line_point_<T> end) const {}
template <typename T> doc_<T> doc_<T>::subdoc(doc_range_<T>) const {}
template <typename T>
std::basic_string<T> doc_<T>::substr(doc_range_<T>) const {}

template <typename T>
void doc_<T>::push_back(std::basic_string<T> const &line,
                        std::basic_string<T> const &filename, size_t line_no) {
  std::vector<doc_line_<T>>::push_back(
      doc_line_<T>{get_filename_id(filename), line_no, line});
}

template <typename T>
void doc_<T>::push_back(std::basic_string<T> &&line,
                        std::basic_string<T> const &filename, size_t line_no) {
  std::vector<doc_line_<T>>::push_back(
      doc_line_<T>{get_filename_id(filename), line_no, std::move(line)});
}

} // namespace linedoc

#endif
