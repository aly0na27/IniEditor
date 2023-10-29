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
