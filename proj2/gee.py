import re, sys, string

debug = False
dict = {}
tokens = []


#  Expression class and its subclasses
class Expression(object):
    def __str__(self):
        return ""


class BinaryExpr(Expression):
    def __init__(self, op, left, right):
        self.op = op
        self.left = left
        self.right = right

    def __str__(self):
        return str(self.op) + " " + str(self.left) + " " + str(self.right)

    def value(self, state):
        left = self.left.value(state)
        right = self.right.value(state)

        fn_table = {
            "+": lambda x, y: x + y,
            "-": lambda x, y: x - y,
            "*": lambda x, y: x * y,
            "/": lambda x, y: x / y,
            "==": lambda x, y: x == y,
            "!=": lambda x, y: x != y,
            ">": lambda x, y: x > y,
            ">=": lambda x, y: x >= y,
            "<": lambda x, y: x < y,
            "<=": lambda x, y: x <= y,
            "and": lambda x, y: x and y,
            "or": lambda x, y: x or y,
        }

        return fn_table[self.op](left, right)

    def typing(self, tm):
        left = self.left.typing(tm)
        right = self.right.typing(tm)

        if left == right:
            if (
                self.op in ["+", "-", "*", "/"]
                and left == "number"
                and right == "number"
            ):
                return "number"
            if (
                self.op in ["==", "!=", ">", ">=", "<", "<="]
                and left == "number"
                and right == "number"
            ):
                return "boolean"

        error(
            "Type Error: Operations between type "
            + left
            + " and "
            + right
            + " not allowed!"
        )


class Number(Expression):
    def __init__(self, val):
        self.val = val

    def __str__(self):
        return str(self.val)

    def value(self, state):
        return int(self.val)

    def typing(self, tm):
        return "number"


class VarRef(Expression):
    def __init__(self, val):
        self.val = val

    def __str__(self):
        return str(self.val)

    def value(self, state):
        return state[self.val]

    def typing(self, tm):
        if self.val in tm:
            return tm[str(self.val)]
        else:
            error(f"Type Error: " + self.val + " is referenced before being defined!")


class String(Expression):
    def __init__(self, val):
        self.value = val

    def __str__(self):
        return str(self.val)

    def value(self, state):
        return self.val

    def typing(self, tm):
        return "str"


class Statement(object):
    def __str__(self):
        return ""


class Assignment(Statement):
    def __init__(self, var_ref, expr):
        self.var_ref = var_ref
        self.expr = expr

    def __str__(self):
        return "= " + str(self.var_ref) + " " + str(self.expr)

    def meaning(self, state):
        state[self.var_ref.val] = self.expr.value(state)
        return state

    def tipe(self, tm):
        if self.var_ref.val not in tm:
            tm[str(self.var_ref)] = self.expr.typing(tm)
            print(self.var_ref, tm[str(self.var_ref)])
        elif tm[str(self.var_ref)] != self.expr.typing(tm):
            error(
                "Type Error: "
                + tm[str(self.var_ref)]
                + " = "
                + self.expr.typing(tm)
                + "!"
            )


class IfStatement(Statement):
    def __init__(self, expression, if_block, else_block):
        self.expression = expression
        self.if_block = if_block
        self.else_block = else_block

    def __str__(self):
        stmt = "if " + str(self.expression) + "\n" + str(self.if_block)

        if self.else_block:
            stmt += "\nelse\n" + str(self.else_block)

        stmt += "\nendif"

        return stmt

    def meaning(self, state):
        if self.expression.value(state):
            self.if_block.meaning(state)
        else:
            self.else_block.meaning(state)

    def tipe(self, tm):
        if self.expression.typing(tm) == "boolean":
            self.if_block.tipe(tm)
            if self.else_block:
                self.else_block.tipe(tm)
        else:
            error("Type Error: If statement condition must be boolean!")


class WhileStatement(Statement):
    def __init__(self, expression, block):
        self.expression = expression
        self.block = block

    def __str__(self):
        return "while " + str(self.expression) + "\n" + str(self.block) + "endwhile"

    def meaning(self, state):
        while self.expression.value(state):
            self.block.meaning(state)
        return state

    def tipe(self, tm):
        if self.expression.typing(tm) == "boolean":
            self.block.tipe(tm)
        else:
            error("Type Error: While statement condition must be boolean!")


class Block(Statement):
    def __init__(self, old, new):
        self.old = old
        self.new = new

    def __str__(self):
        return str(self.old) + "\n" + str(self.new) + "\n"

    def meaning(self, state):
        return self.new.meaning(self.old.meaning(state))

    def tipe(self, tm):
        self.old.tipe(tm)
        self.new.tipe(tm)


class StmtList(Statement):
    def __init__(self, list):
        self.list = list

    def meaning(self, state):
        for stmt in self.list:
            stmt.meaning(state)
        return state

    def tipe(self, tm):
        for stmt in self.list:
            stmt.tipe(tm)
        return tm


def error(msg):
    # print msg
    sys.exit(msg)


# The "parse" function. This builds a list of tokens from the input string,
# and then hands it to a recursive descent parser for the PAL grammar.


def match(matchtok):
    tok = tokens.peek()
    if tok != matchtok:
        error("Expecting " + matchtok)
    tokens.next()
    return tok


def factor():
    """factor     = number | string | ident |  "(" expression ")" """

    tok = tokens.peek()
    if debug:
        print("Factor: ", tok)
    if re.match(Lexer.number, tok):
        expr = Number(tok)
        tokens.next()
        return expr
    if re.match(Lexer.string, tok):
        expr = String(tok)
        tokens.next()
        return expr
    if re.match(Lexer.identifier, tok):
        expr = VarRef(tok)
        tokens.next()
        return expr
    if tok == "(":
        tokens.next()  # or match( tok )
        expr = expression()
        tokens.peek()
        tok = match(")")
        return expr
    error("Invalid operand")
    return


def term():
    """term    = factor { ('*' | '/') factor }"""

    tok = tokens.peek()
    if debug:
        print("Term: ", tok)
    left = factor()
    tok = tokens.peek()
    while tok == "*" or tok == "/":
        tokens.next()
        right = factor()
        left = BinaryExpr(tok, left, right)
        tok = tokens.peek()
    return left


def addExpr():
    """addExpr    = term { ('+' | '-') term }"""

    tok = tokens.peek()
    if debug:
        print("addExpr: ", tok)
    left = term()
    tok = tokens.peek()
    while tok == "+" or tok == "-":
        tokens.next()
        right = term()
        left = BinaryExpr(tok, left, right)
        tok = tokens.peek()
    return left


def relationalExpr():
    """relationalExpr = addExpr [ relation addExpr ]"""
    tok = tokens.peek()
    if debug:
        print("relationalExpr: ", tok)
    left = addExpr()

    tok = tokens.peek()

    while re.match(Lexer.relational, tok):
        tokens.next()
        right = addExpr()
        left = BinaryExpr(tok, left, right)
        tok = tokens.peek()
    return left


def andExpr():
    """andExpr    = relationalExpr { "and" relationalExpr }"""
    tok = tokens.peek()
    if debug:
        print("andExpr: ", tok)

    left = relationalExpr()
    tok = tokens.peek()
    while tok == "and":
        tokens.next()
        right = relationalExpr()
        left = BinaryExpr(tok, left, right)
        tok = tokens.peek()
    return left


def expression():
    """expression = andExpr { "or" andExpr }"""
    tok = tokens.peek()
    if debug:
        print("expression: ", tok)

    left = andExpr()
    tok = tokens.peek()
    while tok == "or":
        tokens.next()
        right = andExpr()
        left = BinaryExpr(tok, left, right)
        tok = tokens.peek()
    return left


def parseAssign():
    """assign = ident "=" expression  eoln"""
    tok = tokens.peek()
    if debug:
        print("parseAssign: ", tok)
    if re.match(Lexer.identifier, tok):
        variable_reference = VarRef(tok)
    tokens.next()
    match("=")
    expr = expression()
    match(";")
    return Assignment(variable_reference, expr)


def parseWhile():
    """whileStatement = "while"  expression  block"""
    tok = tokens.peek()
    if debug:
        print("parseWhile: ", tok)

    match("while")
    expr = expression()
    block = parseBlock()
    return WhileStatement(expr, block)


def parseIf():
    """ifStatement = "if" expression block   [ "else" block ]"""
    tok = tokens.peek()
    if debug:
        print("parseIf: ", tok)
    match("if")
    expr = expression()
    if_block = parseBlock()
    tok = tokens.peek()
    if tok == "else":
        match("else")
        else_block = parseBlock()
    else:
        else_block = None
    return IfStatement(expr, if_block, else_block)


def parseBlock():
    """block = ":" eoln indent stmtList undent"""
    tok = tokens.peek()
    if debug:
        print("parseBlock: ", tok)

    match(":")
    match(";")
    match("@")
    block = parseStmt()
    while tokens.peek() != "~":
        prev_block = block
        block = Block(prev_block, parseStmt())
    match("~")
    return block


def parseStmt():
    """statement = ifStatement |  whileStatement  |  assign"""
    tok = tokens.peek()

    if debug:
        print("parseStmt: ", tok)

    if tok == "if":
        return parseIf()
    elif tok == "while":
        return parseWhile()
    else:
        return parseAssign()


def parseStmtList():
    """gee = { Statement }"""
    tok = tokens.peek()
    stmtList = []
    while tok is not None:
        # need to store each statement in a list
        ast = parseStmt()
        stmtList.append(ast)
        # print(str(ast))
        tok = tokens.peek()
    return StmtList(stmtList)


def parse(text):
    global tokens
    tokens = Lexer(text)
    stmtlist = parseStmtList()
    print(str(stmtlist))
    # semantics(stmtlist)
    types(stmtlist)
    return


def print_state(state):
    out = "{"

    for key, value in state.items():
        out += "<" + str(key) + ", " + str(value) + ">" + ", "
    out = out[:-2]
    out += "}"
    print(out)


def semantics(stmtlist):
    state = {}
    state = stmtlist.meaning(state)
    print_state(state)


def types(stmtlist):
    tm = {}
    tm = stmtlist.tipe(tm)


# Lexer, a private class that represents lists of tokens from a Gee
# statement. This class provides the following to its clients:
#
#   o A constructor that takes a string representing a statement
#       as its only parameter, and that initializes a sequence with
#       the tokens from that string.
#
#   o peek, a parameterless message that returns the next token
#       from a token sequence. This returns the token as a string.
#       If there are no more tokens in the sequence, this message
#       returns None.
#
#   o removeToken, a parameterless message that removes the next
#       token from a token sequence.
#
#   o __str__, a parameterless message that returns a string representation
#       of a token sequence, so that token sequences can print nicely


class Lexer:
    # The constructor with some regular expressions that define Gee's lexical rules.
    # The constructor uses these expressions to split the input expression into
    # a list of substrings that match Gee tokens, and saves that list to be
    # doled out in response to future "peek" messages. The position in the
    # list at which to dole next is also saved for "nextToken" to use.

    special = r"\(|\)|\[|\]|,|:|;|@|~|;|\$"
    relational = "<=?|>=?|==?|!="
    arithmetic = "\+|\-|\*|/"
    # char = r"'."
    string = r"'[^']*'" + "|" + r'"[^"]*"'
    number = r"\-?\d+(?:\.\d+)?"
    literal = string + "|" + number
    # idStart = r"a-zA-Z"
    # idChar = idStart + r"0-9"
    # identifier = "[" + idStart + "][" + idChar + "]*"
    identifier = "[a-zA-Z]\w*"
    lexRules = (
        literal + "|" + special + "|" + relational + "|" + arithmetic + "|" + identifier
    )

    def __init__(self, text):
        self.tokens = re.findall(Lexer.lexRules, text)
        self.position = 0
        self.indent = [0]

    # The peek method. This just returns the token at the current position in the
    # list, or None if the current position is past the end of the list.

    def peek(self):
        if self.position < len(self.tokens):
            return self.tokens[self.position]
        else:
            return None

    # The removeToken method. All this has to do is increment the token sequence's
    # position counter.

    def next(self):
        self.position = self.position + 1
        return self.peek()

    # An "__str__" method, so that token sequences print in a useful form.

    def __str__(self):
        return "<Lexer at " + str(self.position) + " in " + str(self.tokens) + ">"


def chkIndent(line):
    ct = 0
    for ch in line:
        if ch != " ":
            return ct
        ct += 1
    return ct


def delComment(line):
    pos = line.find("#")
    if pos > -1:
        line = line[0:pos]
        line = line.rstrip()
    return line


def mklines(filename):
    """Takes a file and converts it to the lexer conventions"""
    inn = open(filename, "r")
    lines = []
    pos = [0]
    ct = 0
    for line in inn:
        ct += 1
        line = line.rstrip() + ";"
        line = delComment(line)
        if len(line) == 0 or line == ";":
            continue
        indent = chkIndent(line)
        line = line.lstrip()
        if indent > pos[-1]:
            pos.append(indent)
            line = "@" + line
        elif indent < pos[-1]:
            while indent < pos[-1]:
                del pos[-1]
                line = "~" + line
        print(ct, "\t", line)
        lines.append(line)
    # print len(pos)
    undent = ""
    for i in pos[1:]:
        undent += "~"
    lines.append(undent)
    # print undent
    return lines


def main():
    """main program for testing"""
    global debug
    ct = 0
    for opt in sys.argv[1:]:
        if opt[0] != "-":
            break
        ct = ct + 1
        if opt == "-d":
            debug = True
    if len(sys.argv) < 2 + ct:
        print("Usage:  %s filename" % sys.argv[0])
        return
    parse("".join(mklines(sys.argv[1 + ct])))
    return


if __name__ == "__main__":
    main()
