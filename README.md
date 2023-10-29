# IniEditor

This is a tiny, header only C++ library for manipulating INI files.

It conforms to the following format:

section and key names are case insensitive
whitespace around sections, keys and values is ignored
empty section and key names are allowed
keys that do not belong to a section are ignored
comments are lines where the first non-whitespace character is a semicolon (;)
trailing comments are allowed on section lines, but not key/value lines
every entry exists on a single line and multiline is not supported

```INI
; comment
[section]
key = value
```

Section and key order is preserved on read and write operations. Iterating through data will take the same order as the original file or the order in which keys were added to the structure.

This library operates with the `std::string` type to hold values and relies on your host environment for encoding. It should play nicely with UTF-8 but your mileage may vary.
