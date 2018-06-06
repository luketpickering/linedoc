#ifndef LINEDOC_DOC_LINE_HXX_SEEN
#define LINEDOC_DOC_LINE_HXX_SEEN

#include <string>

namespace linedoc {

/// Document line type
template <typename T> struct doc_line_ {
  /// The UID of the on-disk file that this line was read from
  size_t fileID;
  /// The line number in the on-disk file that this line corresponds to
  size_t file_line_no;
  /// The character line read from the on-disk file
  std::basic_string<T> characters;

  bool is_empty() const { return !characters.size(); }
  bool is_EOL(size_t character) const { return character >= characters.size(); }
  size_t last_char() const {
    return is_empty() ? std::string::npos : (characters.size() - 1);
  }
  size_t size() const { return characters.size(); }
};

typedef doc_line_<char> doc_line;
typedef doc_line_<wchar_t> wdoc_line;

} // namespace linedoc

#endif
