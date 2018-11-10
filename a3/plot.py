import matplotlib.pyplot as plt
import sys

def getSeconds( s ):
	[m,s] = (s.split(":"))
	return (int(m) * 60 + float(s))

with open(sys.argv[1]) as time:
	read = time.readlines()

read = [getSeconds(i) for i in read]

x_val = ["5%","10%", "25%", "50%", "95%"]
x_l = [0,1,2,3,4]
gspan = read[0:5]
fsg = read[5:10]
gaston = read[10:]

fig, ax1 = plt.subplots(3, figsize=(15,15))
plt.subplot(3,1,1)
plt.grid()
plt.title("Running Time of gSpan with varying support")
plt.plot(x_l, gspan, marker='o')
plt.xticks(range(len(x_val)), x_val)
plt.subplot(3,1,2)
plt.grid()
plt.title("Running Time of fsg with varying support")
plt.plot(x_l, fsg, marker='o')
plt.xticks(range(len(x_val)), x_val)
plt.subplot(3,1,3)
plt.grid()
plt.title("Running Time of gaston with varying support")
plt.plot(x_l, gaston, marker='o')
plt.xticks(range(len(x_val)), x_val)
plt.savefig('plot.png')
plt.close()
