#include "linedoc/doc.hxx"
#include "linedoc/doc_utils.hxx"

#include <iostream>

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cout << "[ERROR]: Expected to recieve the path of a text file to read."
              << std::endl;
    return 1;
  }

  linedoc::doc d = linedoc::read_doc(argv[1]);
  std::cout << "[INFO]: Document " << argv[1] << " read " << d.n_lines()
            << " lines." << std::endl;
  for (size_t i = 0; i < d.n_lines(); ++i) {
    std::cout << d.get_line(i) << " -- " << d.get_line_info(i) << std::endl;
  }

  return 0;
}
