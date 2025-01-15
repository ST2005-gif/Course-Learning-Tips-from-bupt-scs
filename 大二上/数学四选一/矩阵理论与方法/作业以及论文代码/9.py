#2023210896 王书翰
import numpy as np

x=np.array([[-1j,2,3],[1,0,1j]])
normw=np.linalg.norm(x,ord=np.inf)
print("无穷范数",normw)
norm1=np.linalg.norm(x,ord=1)
print("L1范数",norm1)
norm2=np.linalg.norm(x,ord=2)
print("L2范数",norm2)
A=np.array([[0.1,0.3],[0.7,0.6]])
eigenvalues, eigenvectors = np.linalg.eig(A)
a=np.max(eigenvalues)
print("ρ(A)=",a,"因为ρ(A)＜1,所以A是收敛矩阵")

