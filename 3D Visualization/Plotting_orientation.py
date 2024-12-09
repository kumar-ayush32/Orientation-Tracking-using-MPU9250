import serial
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
ser = serial.Serial('COM9', 115200, timeout=1) #Check in which USB port ESP32 is connected. In my case it is in COM9

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
plt.ion() #Make the plot interactive

ax.set_xlabel('X Axis')
ax.set_ylabel('Y Axis')
ax.set_zlabel('Z Axis')
ax.set_xlim([-10, 10])
ax.set_ylim([-10, 10])
ax.set_zlim([-10, 10])
x1=0
def update_plot(a, b, c, m1,m2,m3):
    if(a==0):
        a+=0.00001
    if(b==0):
        b+=0.00001
    global x1
    z1=np.sqrt((9.8*b)**2/(c**2+b**2)) #Length of arrow
    y1=-c*z1/b;
    theta=m3*3.14/180
    norm_N_sq = a**2 + b**2 + c**2
    norm_N = np.sqrt(norm_N_sq)
    #Calculating the points
    x2 = (x1 * np.cos(theta) +
          (b * z1 - c * y1) * np.sin(theta) / norm_N +
          (a * (a * x1 + b * y1 + c * z1)) * (1 - np.cos(theta)) / norm_N_sq)
    y2 = (y1 * np.cos(theta) +
          (c * x1 - a * z1) * np.sin(theta) / norm_N +
          (b * (a * x1 + b * y1 + c * z1)) * (1 - np.cos(theta)) / norm_N_sq)
    z2 = (z1 * np.cos(theta) +
          (a * y1 - b * x1) * np.sin(theta) / norm_N +
          (c * (a * x1 + b * y1 + c * z1)) * (1 - np.cos(theta)) / norm_N_sq)
    x3 = (x1 * np.cos(theta+90*3.14/180) +
          (b * z1 - c * y1) * np.sin(theta+90*3.14/180) / norm_N +
          (a * (a * x1 + b * y1 + c * z1)) * (1 - np.cos(theta+90*3.14/180)) / norm_N_sq)
    y3 = (y1 * np.cos(theta+90*3.14/180) +
          (c * x1 - a * z1) * np.sin(theta+90*3.14/180) / norm_N +
          (b * (a * x1 + b * y1 + c * z1)) * (1 - np.cos(theta+90*3.14/180)) / norm_N_sq)
    z3 = (z1 * np.cos(theta+90*3.14/180) +
          (a * y1 - b * x1) * np.sin(theta+90*3.14/180) / norm_N +
          (c * (a * x1 + b * y1 + c * z1)) * (1 - np.cos(theta+90*3.14/180)) / norm_N_sq)
    print(f"Accel: {a}, {b}, {c}")
    ax.clear()
    #Plotting the Arrow
    ax.quiver(0,0,0,a,b,c, color='b', arrow_length_ratio=0.1)
    ax.text(a, b, c, 'Z', color='b')
    ax.quiver(0,0,0,x2,y2,z2, color='r', arrow_length_ratio=0.1)
    ax.quiver(0,0,0,x3,y3,z3, color='g', arrow_length_ratio=0.1)
    ax.quiver(0,0,0,-a,-b,-c, color='b', arrow_length_ratio=0.1)
    ax.quiver(0,0,0,-x2,-y2,-z2, color='r', arrow_length_ratio=0.1)
    ax.quiver(0,0,0,-x3,-y3,-z3, color='g', arrow_length_ratio=0.1)
    
    vertices = np.array([[a,b,c],
                         [x2,y2,z2],
                         [x3,y3,z3],
                         [-a,-b,-c],
                         [-x2,-y2,-z2],
                         [-x3,-y3,-z3]])
    faces = [[vertices[j] for j in [0, 1, 2]],
             [vertices[j] for j in [0, 1, 5]],
             [vertices[j] for j in [3, 1, 2]],
             [vertices[j] for j in [3, 1, 4]],
             [vertices[j] for j in [3, 4, 2]],
             [vertices[j] for j in [3, 4, 5]],
             [vertices[j] for j in [0, 4, 2]],
             [vertices[j] for j in [0, 4, 5]]]
    #Plotting the outside structure
    ax.add_collection3d(Poly3DCollection(faces, facecolors='cyan', linewidths=1, edgecolors='r', alpha=.25))
    
    ax.set_xlabel('X Axis')
    ax.set_ylabel('Y Axis')
    ax.set_zlabel('Z Axis')
    ax.set_xlim([-12, 12])
    ax.set_ylim([-12, 12])
    ax.set_zlim([-12, 12])
    plt.draw()
    plt.pause(0.01)

try:
    while True:
        if ser.in_waiting > 0:
            #Received data is in the format of {Accel X: 0.004121  Accel Y: -0.071851  Accel Z: 9.812561  Deg X: 0.01  Deg Y: 0.01  Deg Z: 0.02}
            line = ser.readline().decode('utf-8').rstrip()
            line = line.split('\t')
            ax_accel = float(line[0].split(': ')[1])
            ay_accel = float(line[1].split(': ')[1])
            az_accel = float(line[2].split(': ')[1])
            dx_gyro = float(line[3].split(': ')[1])
            dy_gyro = float(line[4].split(': ')[1])
            dz_gyro = float(line[5].split(': ')[1])
            print(dx_gyro,dy_gyro,dz_gyro)
            try:
                #Plot real-time data
                update_plot(ax_accel, ay_accel, az_accel,dx_gyro,dy_gyro,dz_gyro)
            except ValueError:
                print("Invalid data format received")
except KeyboardInterrupt:
    print("Program interrupted")
finally:
    ser.close()
    plt.ioff()
    plt.show()
