import matplotlib.pyplot as plt

x = [i for i in range(256)]
y = []
for i in range(256):
	if i < 64:
		y.append(i*4)
	else:
		y.append(255)

plt.plot(x,y)
plt.show()
