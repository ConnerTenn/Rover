
from tkinter import *
import time
import random
from MatrixCode import *
from math import *

run = True

class Point:
	def __init__(self, x=0, y=0, z=0, r=0, g=0, b=0):
		self.X = x
		self.Y = y
		self.Z = z

		self.R = r
		self.G = g
		self.B = b
	
	def __add__(self, other):
		return Point(self.X + other.X, self.Y + other.Y, self.Z + other.Y)

	def Mat(self):
		return Matrix.fromList([[self.X],[self.Y],[self.Z],[0]])

	def FromMat(self, mat):
		self.X = mat[0][0]
		self.Y = mat[1][0]
		self.Z = mat[2][0]

	def __str__(self):
		return "["+str(self.X)+","+str(self.Y)+","+str(self.Z)+"]"

class Triangle:
	def __init__(self, p1=Point(), p2=Point(), p3=Point()):
		self.Points = [p1, p2, p3]
	
	def Clockwise(self):
		None
		
	def __str__(self):
		return "{ " + str(self.Points[0]) + " " + str(self.Points[1]) + " " + str(self.Points[2]) + " }"

class Renderer:
	def __init__(self, canvas, width=500, height=300, fov=pi/2, cam=Point()):
		self.Canvas = canvas
		self.Width = width
		self.Height = height
		self.FOV = fov

		self.Camera = cam
		
		self.Resize()

	def Resize(self):
		self.PixelBuff = [[[0 for i in range(3)] for y in range(self.Height)] for x in range(self.Width)]
		self.DepthBuf = [[0 for y in range(self.Height)] for x in range(self.Width)]

	def GetProjectionMatrix(self):
		n=0.1; f=100; l=-10; r=10; t=10; b=-10
		return Matrix.fromList(                                             \
			[                                                               \
				[ (2*n)/(r-l), 0,            (r+l)/(r-l), 0              ], \
				[ 0,           (2*n)/(t-b),  (t+b)/(t-b), 0              ], \
				[ 0,           0,           -(f+n)/(f-n), -(2*f*n)/(f-n) ], \
				[ 0,           0,           -1          , 0              ]  \
			])

	def Shader(self):
		None

	def Render(self, tri):
		proj = self.GetProjectionMatrix()
		triProj = Triangle()
		#print("TRI:\n")
		for i in range(3):
			#print(tri.Points[i].Mat())
			triProj.Points[i].FromMat(proj * tri.Points[i].Mat())
		
		print(str(tri))
		print(str(triProj))
		print()
		

class Screen:
	def __init__(self, Width=1000, Height=600, Title="Projection"):
		self.Width=Width
		self.Height=Height
		self.Title=Title

		self.Play=True

		self.tk=Tk()
		self.tk.title(self.Title)
		self.tk.resizable(0,0)
		self.canvas=Canvas(self.tk, width=self.Width, height=self.Height)
		self.canvas.pack()
		
		self.tk.protocol("WM_DELETE_WINDOW", self.Close)
		
		self.Renderer = Renderer(self.canvas)

		self.Update()
		self.tk.mainloop()
	
	def Close(self):
		self.tk.destroy()

	def Update(self):
		self.canvas.delete("all")
		
		self.Renderer.Render(Triangle(Point(1,1,1), Point(-1,-1,-1), Point(-1,1,-1)))

		self.canvas.update()
		self.tk.after(10, self.Update)

#a = Matrix.fromList([[1],[2]])
#b = Matrix.fromList([[3, 4]])
#print(str(a) + "\nTimes\n" + str(b) + "\nEquals\n" + str(a*b))

screen = Screen()

