#linedoc

An in-memory character document that tracks the on-disk file name and line of each character.

## Design goals

* Expose class methods to be interacted with similar to a bare std::basic_string
* Line breaks in the input document are implicit in the linedoc.
