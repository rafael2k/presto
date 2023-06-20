# Simple C/C++ lexer
#
# Module Contents
# ===============
#
# split(source[, include_ws=True, include_comments=True])
#
#   Returns an iterator that returns the tokens in the C/C++ source as
#   individual strings.  If 'include_ws' is true, sequences of whitespace
#   (including linebreaks) separating tokens are returned as well.  If
#   'include_comments' is true, comments are returned as individual tokens as
#   well.
#
#   Preprocessor lines are returned as single tokens, starting with the first
#   character of the line and ending before the linebreak character that ends
#   the preprocessor directive; including any backslashes and linebreak
#   characters following backslashes.
#
#   Character and string literal tokens are returned exactly as they occured in
#   the source string, with any escape sequences (including escaped linebreaks)
#   preserved.
#
#   Escaped linebreaks outside of preprocessor directives and string literals
#   are not handled; in practice the backslash will vanish and the linebreak
#   will be returned as a whitespace token (possibly combined with any folloing
#   whitespace.)  If whitespace preceded the backslash, there will be two
#   separate whitespace tokens, split where the backslash was.
#
# tokenize(tokens[, filename="<unknown>"])
#
#   Returns an iterator that returns each string returned by the iterable
#   'tokens' converted into a Token object.  The token's have line and column
#   numbers calculated assuming that the token sequence contains all tokens and
#   whitespace sequences from a single file.  If not, the line and column
#   numbers will not be correct.  The supplied 'filename' is also stored in each
#   token.
#
# group(tokens[, groups={ '(':')', '{':'}', '[':']' }])
#
#   Returns a list containing all tokens returned by the iterable 'tokens',
#   grouped into sublists according to 'groups', which should be a dictionary
#   mapping group start tokens to group end tokens.
#
#   Each sublist created will start with a token from the set of keys in
#   'groups' and end with the corresponding end token.  Every token returned by
#   the iterable 'tokens' will occur exactly once in the tree of lists returned
#   by this function.
#
#   Throws a CLexerException if an unexpected group end token is encountered, or
#   if the sequence of tokens ends inside a group.
#
# group1(tokens, end[, groups={ '(':')', '{':'}', '[':']' }])
#
#   Like group(), but returns a single group ending with the token 'end'.
#   Typically 'tokens' will be an iterator that has just returned the
#   corresponding start token.  Upon return, if 'tokens' is an iterator, it will
#   just have returned a token identical to 'end'.
#
#   The returned list contains neither the group start or end token, and is
#   grouped as if group() had been used to group that particular sequence of
#   tokens.  (It is necessary to do this grouping anyway to identify the token
#   that actually ends the group while ignoring sub-groupings using the same
#   pair of start/end tokens.)
#
# partition(tokens, separator)
#
#   Splits the sequence of tokens returned by the iterable 'tokens' by the token
#   'separator'.  Normally, the token sequence 'tokens' should be grouped using
#   group() first, so that occurences of 'separator' inside groups are ignored.
#   The return value is a list of lists of tokens (or lists as created by
#   group()).
#
#   If 'tokens' returns zero tokens, an empty list is returned.
#
# flatten(tokens)
#
#   Returns an iterator that returns each token returned by the iterable
#   'tokens' while reversing the type of grouping done by group()/group1().
#
#   This means that flatten(group(tokens)) is a no-op.
#
# join(tokens[, insertSpaces=True])
#
#   Returns a string produced by concatenating all tokens returned by the
#   iterable 'tokens'.  If 'insertSpaces' is true, a single space is inserted
#   between each token unless there was whitespace strings in the token sequence
#   there already.  Automatically flattens the token sequence first.
#
# CLexerException
#
#   Exception type thrown by the functions group() and group1().  Inherits
#   the builtin Exception type and adds absolutely nothing.
#
# Token Objects
# =============
#
# Token objects are created by the function tokenize(), but can also be
# constructed manually using the constructor
#
# Token(value[, filename="<unknown>", line=0, column=0])
#
# Token instances are comparable, hashable, are true (non-zero) unless they
# represent whitespace or comments, and can be converted back to string form by
# str().  In addition they support the following methods:
#
# filename()
#
#   Returns the filename from which the token stems.  In pracice, either the
#   filename passed to tokenize() or to the Token constructor.
#
# line()
#
#   Returns the line number (first line=1) at which the token occured.
#
# column()
#
#   Returns the column number (first column=0) at which the token occured.
#
# isidentifier()
#
#   Returns true if the token is an identifier.
#
# isspace()
#
#   Returns true if the token is whitespace.
#
# iscomment()
#
#   Returns true if the token is a comment.
#
# isppdirective()
#
#   Returns true if the token is a preprocessor directive.
#
# reduced()
#
#   Returns a string where special tokens (whitespace, comments and preprocessor
#   directives) are converted to the shortest possible sequence of whitespace
#   that preserves the line and column number of following tokens.

import re
import sys
import itertools
import traceback

def rejoin(items, escape):
    if escape:
        fixed = []
        for item in sorted(items, key=len, reverse=True):
            for ch in "(){}[]*+?|.^$": item = item.replace(ch, "\\" + ch)
            fixed.append(item)
        items = fixed
    return "|".join(items)

OPERATORS_AND_PUNCTUATORS = [ "(", ")", "{", "}", "[", "]", "<", ">", "<=", ">=", "<<", ">>", "<<=", ">>=",
                              "+", "-", "*", "/", "%", "+=", "-=", "*=", "/=", "%=", "&&", "||",
                              "&", "|", "^", "!", ",", ".", "::", ":", ";", "=", "==", "!=",
                              "&=", "|=", "^=", "++", "--", "~", "?", "->", "->*", ".*", "##", "#" ];

INT_LITERAL = "(?:0|[1-9][0-9]*)[lLuU]*(?![0-9a-zA-Z_\\.])"
FLOAT_LITERAL = "(?:(?:[0-9]*\\.[0-9]+|[0-9]+\\.)(?:[eE][+-]?[0-9]+)?|[0-9]+[eE][+-]?[0-9]+)[fFlL]*(?![0-9a-zA-Z_])"
IDENTIFIER = "[a-zA-Z_][a-zA-Z0-9_]*"
MULTILINE_COMMENT = "/\\*.*?\\*/"
SINGLELINE_COMMENT = "//.*?(?=\n|$)"
PREPROCESSOR_DIRECTIVE = "^[ \t]*#(?:\\\\\n|[^\n])*"
STRING_LITERAL = '"(?:\\\\.|.)*?"'
CHARACTER_LITERAL = "'(?:\\\\.|.)*?'"
OPERATOR_OR_PUNCTUATOR = rejoin(OPERATORS_AND_PUNCTUATORS, escape=True)
WHITESPACE = "\\s+"

SUBPATTERNS = [FLOAT_LITERAL, INT_LITERAL, IDENTIFIER, MULTILINE_COMMENT, SINGLELINE_COMMENT, PREPROCESSOR_DIRECTIVE, STRING_LITERAL, CHARACTER_LITERAL]

RE_CTOKENS = re.compile(rejoin([IDENTIFIER, FLOAT_LITERAL, MULTILINE_COMMENT, SINGLELINE_COMMENT, PREPROCESSOR_DIRECTIVE, OPERATOR_OR_PUNCTUATOR, INT_LITERAL, STRING_LITERAL, CHARACTER_LITERAL], escape=False), re.DOTALL | re.MULTILINE)
RE_CTOKENS_INCLUDE_WS = re.compile(rejoin([IDENTIFIER, FLOAT_LITERAL, MULTILINE_COMMENT, SINGLELINE_COMMENT, PREPROCESSOR_DIRECTIVE, OPERATOR_OR_PUNCTUATOR, INT_LITERAL, STRING_LITERAL, CHARACTER_LITERAL, WHITESPACE], escape=False), re.DOTALL | re.MULTILINE)

RE_IDENTIFIER = re.compile(IDENTIFIER)

class CLexerException(Exception):
    def __init__(self, message):
        Exception.__init__(self, message)

def isidentifier(value): return str(value)[0].isalpha() or str(value)[0] == "_"
def isspace(value): return str(value).isspace()
def iscomment(value): return str(value)[0:2] in ("/*", "//")
def isppdirective(value): return str(value).lstrip(" \t").startswith("#")

def split(input, include_ws=True, include_comments=True):
    if include_ws: expression = RE_CTOKENS_INCLUDE_WS
    else: expression = RE_CTOKENS
    tokens = itertools.imap(lambda match: match.group(0), expression.finditer(input))
    if include_comments: return tokens
    else: return itertools.ifilter(lambda token: not iscomment(token), tokens)

class Token:
    def __init__(self, value, filename="<unknown>", line=0, column=0):
        self.__value = value
        self.__filename = filename
        self.__line = line
        self.__column = column

    def __cmp__(self, other):
        return cmp(self.__value, other)

    def __str__(self):
        return self.__value

    def __repr__(self):
        return repr(self.__value)

    def __hash__(self):
        return hash(self.__value)

    def __nonzero__(self):
        return not (self.isspace() or self.iscomment())

    def filename(self): return self.__filename
    def line(self): return self.__line
    def column(self): return self.__column

    def isidentifier(self): return isidentifier(self.__value)
    def isspace(self): return isspace(self.__value)
    def iscomment(self): return iscomment(self.__value)
    def isppdirective(self): return isppdirective(self.__value)

    def reduced(self):
        if self.isspace() or self.iscomment():
            if self.__value.startswith("//"): return ""
            else: 
                linebreaks = self.__value.count("\n")
                if linebreaks:
                    last = self.__value.rindex("\n")
                    return "\n" * linebreaks + " " * (len(self.__value) - last - 1)
                else:
                    return " " * len(self.__value)
        elif self.isppdirective():
            return "\n" * self.__value.count("\n")
        else:
            return self.__value

def tokenize(tokens, filename="<unknown>"):
    line = 1
    column = 0

    for token in tokens:
        if isinstance(token, Token):
            yield token
            token = str(token)
        else: yield Token(token, filename, line, column)
        
        if token.isspace() or token.startswith("/*"):
            linebreaks = token.count("\n")
            if linebreaks:
                line += linebreaks
                column = len(token) - 1 - token.rindex("\n")
        else:
            column += len(token)

def locate(tokens, index):
    line = 1
    column = 0

    for token_index, token in enumerate(flatten(tokens)):
        if index == token_index: break
        if token.isspace() or token[0:2] == "/*":
            linebreaks = token.count("\n")
            if linebreaks:
                line += linebreaks
                column = len(token) - 1 - token.rindex("\n")
            else:
                column += len(token)
        else:
            column += len(token)

    return line, column

DEFAULT_GROUP = {'(': ')', '{': '}', '[': ']'}
DEFAULT_GROUP_REVERSE = {')': '(', '}': '{', ']': '['}

def group(tokens, groups=None):
    if groups is None:
        groups = DEFAULT_GROUP
        reverse = DEFAULT_GROUP_REVERSE
    else:
        reverse = dict([(end, start) for start, end in groups.items()])

    stack = [('<EOF>', [], -1)]
    currentEnd = stack[-1][0]
    currentList = stack[-1][1]

    for index, token in enumerate(tokens):
        if token in groups:
            stack.append((groups[token], [token], index))
            currentList = stack[-1][1]
            currentEnd = stack[-1][0]
        elif token == currentEnd:
            currentList.append(token)
            stack.pop()
            stack[-1][1].append(currentList)
            currentEnd = stack[-1][0]
            currentList = stack[-1][1]
        elif token in reverse:
            if isinstance(token, Token):
                line, column = token.line(), token.column()
            else:
                line, column = locate(tokens, index)
            raise CLexerException, "%d:%d: expected '%s', got '%s'" % (line, column, currentEnd, token)
        else:
            currentList.append(token)

    if len(stack) > 1:
        token = stack[-1][1][0]
        if isinstance(token, Token):
            line, column = token.line(), token.column()
        else:
            line, column = locate(tokens, stack[-1][2])
        raise CLexerException, "%d:%d: unmatched group opener '%s'" % (line, column, token)

    return currentList

def group1(iterable, end, groups=None):
    if groups is None:
        groups = DEFAULT_GROUP
        reverse = DEFAULT_GROUP_REVERSE
    else:
        reverse = dict([(end, start) for start, end in groups.items()])

    stack = [('<EOF>', [])]
    currentEnd = stack[-1][0]
    currentList = stack[-1][1]

    for token in iterable:
        if token in groups:
            stack.append((groups[token], [token]))
            currentList = stack[-1][1]
            currentEnd = stack[-1][0]
        elif token == end and len(stack) == 1:
            return currentList
        elif token == currentEnd:
            stack.pop()
            currentList.append(token)
            stack[-1][1].append(currentList)
            currentEnd = stack[-1][0]
            currentList = stack[-1][1]
        elif token in reverse:
            raise CLexerException, "expected '%s', got '%s'" % (currentEnd, token)
        else:
            currentList.append(token)

    token = stack[-1][1][0]
    raise CLexerException, "unmatched group opener '%s'" % token

def partition(tokens, separator):
    current = []
    partitions = [current]
    try:
        tokens = iter(tokens)
        while True:
            token = tokens.next()
            if token == separator:
                current = []
                partitions.append(current)
            else:
                current.append(token)
    except StopIteration:
        if len(partitions) == 1 and not partitions[0]: return []
        else: return partitions
    
def flatten(tokens):
    tokens = iter(tokens)

    try:
        while True:
            token = tokens.next()
            if isinstance(token, list):
                tokens = itertools.chain(token, tokens)
            else:
                yield token
    except StopIteration:
        pass

def join(tokens, insertSpaces=True):
    if insertSpaces:
        result = ""
        lastWasSpace = True
        for token in flatten(tokens):
            if not lastWasSpace and not token.isspace(): result += " "
            result += str(token)
            lastWasSpace = token.isspace()
        return result
    else:
        return "".join(itertools.imap(str, flatten(tokens)))

# Run regression tests if we're the main script and not being imported as a module.
if __name__ == "__main__":
    # The token expression does not match whitespace.
    assert not RE_CTOKENS.match(" ")
    assert not RE_CTOKENS.match("\t")
    assert not RE_CTOKENS.match("\r")
    assert not RE_CTOKENS.match("\n")

    def testToken(token, subpattern, rest="", isOperator=False):
        wholeMatch = RE_CTOKENS.match(token)
        assert wholeMatch
        assert wholeMatch.group(0) + rest == token

        subMatch = re.match(subpattern, token, re.DOTALL | re.MULTILINE)
        assert subMatch
        assert subMatch.group(0) + rest == token

        for other in SUBPATTERNS:
            if other != subpattern and not (isOperator and other == PREPROCESSOR_DIRECTIVE and (token == "#" or token == "##")):
                assert not re.match(other, token, re.DOTALL | re.MULTILINE)

    for operatorOrPunctuator in OPERATORS_AND_PUNCTUATORS:
        testToken(operatorOrPunctuator, OPERATOR_OR_PUNCTUATOR, isOperator=True)

    testToken("0", INT_LITERAL)
    testToken("0u", INT_LITERAL)
    testToken("0l", INT_LITERAL)
    testToken("0ul", INT_LITERAL)
    testToken("0lu", INT_LITERAL)

    testToken("1", INT_LITERAL)
    testToken("4711", INT_LITERAL)

    testToken("0.", FLOAT_LITERAL)
    testToken("123.f", FLOAT_LITERAL)
    testToken("123.f", FLOAT_LITERAL)
    testToken("123.l", FLOAT_LITERAL)
    testToken("123.e1", FLOAT_LITERAL)
    testToken("123.e1f", FLOAT_LITERAL)
    testToken("123.e1l", FLOAT_LITERAL)
    testToken(".0", FLOAT_LITERAL)
    testToken(".123", FLOAT_LITERAL)
    testToken(".123f", FLOAT_LITERAL)
    testToken(".123l", FLOAT_LITERAL)
    testToken(".123e1", FLOAT_LITERAL)
    testToken(".123e1f", FLOAT_LITERAL)
    testToken(".123e1l", FLOAT_LITERAL)
    testToken("0.0", FLOAT_LITERAL)
    testToken("123.123", FLOAT_LITERAL)
    testToken("123.123f", FLOAT_LITERAL)
    testToken("123.123l", FLOAT_LITERAL)
    testToken("123.123e1", FLOAT_LITERAL)
    testToken("123.123e1f", FLOAT_LITERAL)
    testToken("123.123e1l", FLOAT_LITERAL)
    testToken("0e1", FLOAT_LITERAL)
    testToken("123e1", FLOAT_LITERAL)
    testToken("123e1f", FLOAT_LITERAL)
    testToken("123e1l", FLOAT_LITERAL)
    testToken("123e100", FLOAT_LITERAL)
    testToken("123e+100", FLOAT_LITERAL)
    testToken("123e-100", FLOAT_LITERAL)

    testToken("i", IDENTIFIER)
    testToken("this_or_that", IDENTIFIER)
    testToken("_", IDENTIFIER)
    testToken("__i", IDENTIFIER)
    testToken("i1", IDENTIFIER)

    testToken("/**/", MULTILINE_COMMENT)
    testToken("/***/", MULTILINE_COMMENT)
    testToken("/****/", MULTILINE_COMMENT)
    testToken("/*****/", MULTILINE_COMMENT)
    testToken("/*foo*/", MULTILINE_COMMENT)
    testToken("/*foo\nfoo\nfoo*/", MULTILINE_COMMENT)

    testToken("//", SINGLELINE_COMMENT)
    testToken("///", SINGLELINE_COMMENT)
    testToken("////", SINGLELINE_COMMENT)
    testToken("//foo", SINGLELINE_COMMENT)
    testToken("//\n", SINGLELINE_COMMENT, "\n")
    testToken("///\n", SINGLELINE_COMMENT, "\n")
    testToken("////\n", SINGLELINE_COMMENT, "\n")
    testToken("//bar\n", SINGLELINE_COMMENT, "\n")

    testToken("#", PREPROCESSOR_DIRECTIVE)
    testToken("#foo", PREPROCESSOR_DIRECTIVE)
    testToken(" #", PREPROCESSOR_DIRECTIVE)
    testToken(" #foo", PREPROCESSOR_DIRECTIVE)
    testToken("#\n", PREPROCESSOR_DIRECTIVE, "\n")
    testToken("#foo\n", PREPROCESSOR_DIRECTIVE, "\n")
    testToken(" #\n", PREPROCESSOR_DIRECTIVE, "\n")
    testToken(" #foo\n", PREPROCESSOR_DIRECTIVE, "\n")

    testToken('""', STRING_LITERAL)
    testToken('"foo"', STRING_LITERAL)
    testToken('"\\"\\"\\""', STRING_LITERAL)

    testToken("''", CHARACTER_LITERAL)
    testToken("'foo'", CHARACTER_LITERAL)
    testToken("'\\'\\'\\''", CHARACTER_LITERAL)
