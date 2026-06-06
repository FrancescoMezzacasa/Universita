class EmptyStackException(Exception):
    pass


class Stack:

    def __init__(self):
        self.data = []

    def push(self, x):
        self.data.append(x)

    def pop(self):
        if self.data == []:
            raise EmptyStackException
        res = self.data[-1]
        self.data = self.data[0:-1]
        return res

    def __str__(self):
        return " ".join([str(s) for s in self.data])


class Expression:

    def __init__(self):
        pass

    @classmethod
    def from_program(cls, text, dispatch):
        stringa = text.split()#separo togliendo gli spazi
        stack = Stack()
        for token in stringa:
            try:
                #se riesco a convertirlo a float è una costante
                val = float(token)
                stack.push(Constant(val))
                continue
            except ValueError:
                pass
            #se sono qua vuol dire che non ho una costante
            #controllo se ho una variabile o un operazione (le operazioni sono solo quelle dentro a dispatch)
            if(token not in dispatch):
                stack.push(Variable(token))
                continue
            #se sono qua è sicuramente un'operazione dentro dispatch
            operazione = dispatch[token]
            #devo vedere l'arità cosi so di quanti fare il pop
            #la posso vedere perchè dentro a dispatch trovo gia la classe corrispondente all operazione giusta
            arita = operazione.arity
            
            argomenti = []
            for i in range(arita):
                argomenti.append(stack.pop())
            
            #faccio effettivamente l'operazione e la metto nello stack
            ris = operazione(argomenti)
            stack.push(ris)
        return stack.pop()#ritorno l'elemetno con il risultato di tutto

    def evaluate(self, env):
        raise NotImplementedError()


class MissingVariableException(Exception):
    pass


class Variable(Expression):

    def __init__(self, name):
        self.name = name

    def evaluate(self, env):
        try:
            val = env[self.name]
            return val
        except KeyError:
            raise MissingVariableException

    def __str__(self):
        return(str(self.name))


class Constant(Expression):

    def __init__(self, value):
        self.value = value

    def evaluate(self, env):
        return self.value #una costante la valuto col suo stesso valore

    def __str__(self):
        return str(self.value)


class Operation(Expression):

    def __init__(self, args):
        self.args = args

    def evaluate(self, env):
        evaluated_args = [arg.evaluate(env) for arg in self.args]
        #devo arrivare con argomenti completamente valutati non robe composte
        #cosi poi chiamo l'operazione vera e propria
        return self.op(*evaluated_args)

    def op(self, *args):
        raise NotImplementedError()

    def __str__(self):
        pass


#la stampa la faccio nelle classi binary e unary cosi so gia quanti elementi ho da stampare (gli argomenti piu l operazione)
class BinaryOp(Operation):
    arity = 2#lo creo non dentro init(se no lo sovrascriverei)

    def __str__(self):
        return f"({self.symbol} {self.args[0]} {self.args[1]})"


class UnaryOp(Operation):
    arity = 1

    def __str__(self):
        return f"({self.symbol} {self.args[0]})"


class Addition(BinaryOp):
    symbol = "+"
    def op(self, x, y):
        return x + y


class Subtraction(BinaryOp):
    symbol = "-"
    def op(self, x, y):
        return x - y


class Division(BinaryOp):
    symbol = "/"
    def op(self, x, y):
        return x / y


class Multiplication(BinaryOp):
    symbol = "*"
    def op(self, x, y):
        return x * y


class Power(BinaryOp):
    symbol = "**"
    def op(self, x, y):
        return x ** y


class Modulus(BinaryOp):
    symbol = "%"
    def op(self, x, y):
        return x % y


class Reciprocal(UnaryOp):
    symbol = "1/"
    def op(self, x):
        return 1 / x


class AbsoluteValue(UnaryOp):
    symbol = "abs"
    def op(self, x):
        return abs(x)
    

d = {"+": Addition, "*": Multiplication, "**": Power, "-": Subtraction,
     "/": Division, "1/": Reciprocal, "abs": AbsoluteValue}
example = "2 3 + x * 6 5 - / abs 2 ** y 1/ + 1/"
e = Expression.from_program(example, d)
print(e)
res = e.evaluate({"x": 3, "y": 7})
print(res)

# Ouput atteso:
# (1/ (+ (1/ y) (** 2 (abs (/ (- 5 6) (* x (+ 3 2)))))))
# 0.84022932953024
