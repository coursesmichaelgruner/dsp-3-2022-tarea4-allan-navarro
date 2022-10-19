#!/usr/bin/python3

import numpy as np
import sounddevice as sd
from scipy.signal import lfilter
import time

Fs = 44100
print(f'Fs = {Fs} Hz')
duration = 2
samples = sd.rec(int(duration * Fs), samplerate=Fs, channels=1, blocking=True)
print('finished capturing')

alphas = [0.6, 0.4, 0.2]
delays = [0.05, 0.250, 0.5]
# y(n) = αy(n − k) + (1 − α)x(n)

""" 
a[0]*y[n] = b[0]*x[n] + b[1]*x[n-1] + ... + b[M]*x[n-M]
                      - a[1]*y[n-1] - ... - a[N]*y[n-N]
"""
for i in range(len(alphas)):
    alpha = alphas[i]
    k = int(Fs*delays[i])

    a = np.zeros(k)
    a[0] = 1
    a[-1] = -alpha

    b = np.asarray([(1-alpha)])

    result = lfilter(b, a, samples)

    print(f'playing α={alpha} k = Fs*{delays[i]}')
    sd.play(result, samplerate=Fs, blocking=True)

