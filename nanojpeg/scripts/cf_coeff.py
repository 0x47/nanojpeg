import sys

try:
    A_NUM, A_DEN = map(int, sys.argv[1].split('/'))
except:
    A_NUM = -1
    A_DEN = 2
print "a = %d/%d" % (A_NUM, A_DEN)
print

DENOM = 64 * A_DEN

def fabs(f):
    if f >= 0.0: return f
    return -f

def cubic(x, a=-1.0):
    if x < 0.0: x = -x
    x2 = x * x
    x3 = x * x2
    if x <= 1.0:
        return (a + 2.0) * x3 - (a + 3.0) * x2 + 1.0
    if x < 2.0:
        return a * x3 - 5.0 * a * x2 + 8.0 * a * x - 4.0 * a
    return 0.0

def rnd(f, bias=0.5):
    if f < 0.0: return -rnd(-f, bias)
    return int(f + bias)

def printdefs(seq, base, names):
    for name, value in zip(names, seq):
        print "#define %s%s (%d)" % (base, name, value)
    print

def renorm(seq):
    scale = float(DENOM) / sum(seq)
    seq = [scale * x for x in seq]
    for bias in (0.5, 0.4, 0.6, 0.3, 0.7, 0.2, 0.8, 0.1, 0.9):
        iseq = [rnd(f, bias) for f in seq]
        diff = sum(iseq) - DENOM
        if not diff: return iseq
    raise ValueError, "no working rounding found"

base = [rnd(cubic(x * 0.25, float(A_NUM) / float(A_DEN)) * DENOM) for x in (5, 1, 3, 7)]
printdefs(base, "CF4", "ABCD")
printdefs(renorm(base[2::-1]), "CF3", "ABC")
printdefs(renorm(base[1:]), "CF3", "XYZ")
printdefs(renorm(base[1::-1]), "CF2", "AB")

shift = 0
for i in xrange(32):
    if DENOM == (1 << i):
        shift = i
        break
if shift:
    print "#define CF(x) njClip(((x) + %d) >> %d)" % (DENOM / 2, shift)
else:
    print "#define CF(x) njClip(((x) + %d) / %d)" % (DENOM / 2, DENOM)
