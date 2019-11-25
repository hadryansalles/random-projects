import numpy as np
from math import floor, log10
import matplotlib.pyplot as plt

w1 = 1060
w2 = 34423
r1 = 180

def f_c2(c1):
  return 1/((w1 + w2 - 1/(r1*c1)*(1 + 1/(w1*w2)))*r1)

def f_r2(c1, c2):
  return 1/(w1*w2*c1*c2*r1)

def round_sig(x, sig=1):
  return round(x, sig-int(floor(log10(abs(x))))-1)



for i in range(-3, -8, -1):
  for j in range(9, 0, -1):
    for k in range(9, -1, -1):
      c1 = (j+k/10)*10**i
      c2 = f_c2(c1)
      r2 = f_r2(c1, c2)
      print("C1:", round(round_sig(c1, 4)/10**-9), "nF", "\tC2:", round(round_sig(c2, 4)/10**-9), "nF", "\tR2:", round(r2), "Ohms")

# c1 = 4.7*10**-6
# c2 = f_c2(c1)
# r2 = f_r2(c1, c2)
# print(round_sig(c1, 2), round_sig(c2, 3), round(r2))
