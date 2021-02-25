# file: test_all.py
# author: Hadryan Salles
# date: 09/18/2020
# brief: automatize the compilation/run of all testbenchs

import os
import subprocess

files = os.listdir('tests')
testbenchs = [f.split('.')[0] for f in files]
fails = []

for t in testbenchs:
    try:
        subprocess.run(["make", "target=" + t], check=True)
    except subprocess.CalledProcessError:
        fails.append(t)

print("\n======================================================================================\n")
print("Compiled successfully: " + str(len(testbenchs)-len(fails)))
print("Failed: " + str(len(fails)))
print("Tests failed: " + str(fails))
print("\n======================================================================================\n")
