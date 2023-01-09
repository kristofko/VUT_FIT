import matplotlib.pyplot as plt

f_x = open(f"../data/X_howitzer.txt")
f_y = open(f"../data/Y_howitzer.txt")

x_data = f_x.read()
y_data = f_y.read()

x = x_data.split(" ")
y = y_data.split(" ")
x.pop()
y.pop()
x_f = [float(num) for num in x]
y_f = [float(num) for num in y]
plt.plot(x_f, y_f, label="Trajektória strely")
plt.xlabel("dĺžka (m)")
plt.ylabel("výška (m)")
plt.legend()
plt.title("Trajektória letu strely pri odpale s uhlom 17.1°")
plt.savefig("target_hit.png")

