coeff = (1.402, 0.34414, 0.71414, 1.772)

import math
results = []
for bits in xrange(1, 33):
    scale = 1 << bits
    icoeff = [int(c * scale + 0.5) for c in coeff]
    scale = 1.0 / scale
    snr = [-10.0 * math.log10(math.fabs(ic * scale - c) / c) for ic, c in zip(icoeff, coeff)]
    results.append((min(snr), bits, icoeff))
results.sort()
bitstep = -0.1 / math.log10(0.5)
for snr, bits, icoeff in results:
    print "%d bits, SNR = %.2f dB (%.2f bits)" % (bits, snr, snr * bitstep)
    print icoeff
    print
