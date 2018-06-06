#linedoc

An in memory character document that remembers the on-disk file name and line of each character.

## Design goals

* Expose class methods to be interacted with like a bare std::string
* Newlines are not considered characters and being 'off the end' of one line is equivalent to being at the first character on the next line.

* Methods to check if a line_point is off the end of the document or line and set corresponding values
* Methods to check for equivalence of line_points, ignores newlines
* Methods to check if a line point is at the beginning or end of a document
  * get first character
  * get last character

* newlines are considered space

* if a < b and b < a then a == b,
* if doc.end() == doc.begin(), empty doc (== EOL on line 0)
* last end() == EOL()
