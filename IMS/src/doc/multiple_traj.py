import matplotlib.pyplot as plt
import numpy as np



angle = 15
while(angle <= 65):
	print(f"on angle : {angle}")
	f_x = open(f"../data/X_angle_{angle}")
	f_y = open(f"../data/Y_angle_{angle}")

	x_data = f_x.read()

	y_data = f_y.read()

	x = x_data.split(" ")
	y = y_data.split(" ")
	x.pop()
	y.pop()
	x_f = [float(num) for num in x]
	y_f = [float(num) for num in y]
	plt.plot(x_f,y_f, label=f'{angle}°')
	angle += 10


plt.xlabel("dĺžka (m)")
plt.ylabel("výška (m)")
plt.legend()
plt.title("Trajektórie letov pri rôznych uhlov odpalu")
plt.savefig("Multiple_trajectories.png")


