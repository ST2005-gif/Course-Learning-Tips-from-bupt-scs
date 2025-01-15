#2023210896 王书翰
import numpy as np

x=np.array([1,2,-3])
normw=np.linalg.norm(x,ord=np.inf)
print("无穷范数",normw)
norm1=np.linalg.norm(x,ord=1)
print("L1范数",norm1)
norm2=np.linalg.norm(x,ord=2)
print("L2范数",norm2)

A=np.array([[4,-4],[0,3]])
normw=np.linalg.norm(A,ord=np.inf)
print("无穷范数",normw)
norm1=np.linalg.norm(A,ord=1)
print("L1范数",norm1)
norm2=np.linalg.norm(A,ord=2)
print("L2范数",norm2)
y=np.array([0.5,0.2])
print("y=",y)
for i in range(10):
    y[0]=y[0]*y[0]
    y[1]=y[1]*y[1]
    print("y的第",i+2,"次幂为",y)
    print("l",i+2,"范数为",np.linalg.norm(y,ord=i+2))