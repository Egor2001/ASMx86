#!/usr/bin/python

import sys
import random as rnd

# random string generator
def generate(symbols, lower, upper, count = 1):
	for _ in range(count):
		cur_str = []
		strlen = rnd.randrange(lower, upper);
		for _ in range(strlen):
			cur_str.append(symbols[rnd.randrange(len(symbols))])
		yield str().join(cur_str)

# console interface
if __name__ == "__main__":
	if len(sys.argv) != 5:
		print("USAGE: ", sys.argv[0], " SYMBOLS LOWER UPPER COUNT")
	else:
		gen = generate(
				symbols = sys.argv[1], 
				lower = int(sys.argv[2]), 
				upper = int(sys.argv[3]) + 1,
				count = int(sys.argv[4])
				)

		for s in gen: 
			print(s)
