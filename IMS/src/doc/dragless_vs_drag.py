import matplotlib.pyplot as plt
import numpy as np




f_x = open(f"../data/X_dragless.txt")
f_y = open(f"../data/Y_dragless.txt")

x_data = f_x.read()
y_data = f_y.read()

x = x_data.split(" ")
y = y_data.split(" ")
x.pop()
y.pop()
x_f = [float(num) for num in x]
y_f = [float(num) for num in y]

plt.plot(x_f, y_f, label="Bez odporu")



f_x = open(f"../data/X_dragless2.txt")
f_y = open(f"../data/Y_dragless2.txt")

x_data = f_x.read()
y_data = f_y.read()

x = x_data.split(" ")
y = y_data.split(" ")
x.pop()
y.pop()
x_f = [float(num) for num in x]
y_f = [float(num) for num in y]
plt.plot(x_f, y_f, label="S odporom")
plt.xlabel("dĺžka (m)")
plt.ylabel("výška (m)")
plt.legend()
plt.title("Porovnanie trajektórie s ohľadom na odpor vzduchu")
plt.savefig("Dragless_vs_with_drag.png")


