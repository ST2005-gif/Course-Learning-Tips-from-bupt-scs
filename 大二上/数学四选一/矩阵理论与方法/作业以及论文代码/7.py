#2023210896 王书翰
import sympy as sp
import numpy as np
from sympy import Matrix

def trans(A):
    A=A+2*A.T
    return A
def create(A,B,C):
    a=trans(A)
    b=trans(B)
    if(np.isclose(b[1,0],0)):
        b[1,0]=0
    c=trans(C)
    a=a.flatten()
    b=b.flatten()
    c=c.flatten()
    A=A.flatten()
    B=B.flatten()
    C=C.flatten()
    X=np.column_stack((A,B,C))
    x1,r1,rank1,s=np.linalg.lstsq(X,a,rcond=None)
    x2,r1,rank1,s=np.linalg.lstsq(X,b,rcond=None)
    x3,r1,rank1,s=np.linalg.lstsq(X,c,rcond=None)
    return np.column_stack((x1,x2,x3))
    
def schmidt(vectors):
    vectors = [v.astype(np.float64) for v in vectors]
    orthonormal_vectors = []
    for vector in vectors:
        projection = np.zeros_like(vector)
        for v in orthonormal_vectors:
            projection = np.dot(v, vector) * v+projection
        orthogonal_vector = vector - projection
        orthogonal_vector /= np.linalg.norm(orthogonal_vector)
        orthonormal_vectors.append(orthogonal_vector)
    return orthonormal_vectors

def caculateJ(A):
    A=Matrix(A)
    P,J=A.jordan_form()
    P=np.array(P)
    P=P.astype(float)
    return P

A=np.array([[4,-4],[0,-3]])
A=trans(trans(trans(A)))
print("直接计算")
print(A)
A=np.array([[-1,1],[0,0]])
B=np.array([[-1,0],[1,0]])
C=np.array([[0,0],[0,1]])
vectors=[A.flatten(),B.flatten(),C.flatten()]
orthonormal_vectors = schmidt(vectors)
A=orthonormal_vectors[0].reshape(2,2)
B=orthonormal_vectors[1].reshape(2,2)
C=orthonormal_vectors[2].reshape(2,2)
D=create(A,B,C)
P=caculateJ(D)
E3=A*P[0,0]+B*P[1,0]+C*P[2,0]
E2=A*P[1,1]*(-1)+B*P[0,2]+C*P[2,1]
E1=A*P[0,2]+B*P[1,2]+C*P[2,2]
if(np.isclose(E2[0,0],0)):
    E2[0,0]=0
A1=E1*27*(-1)*np.sqrt(6)-E2*np.sqrt(2)+E3*27*(-3)
print("使用矩阵论框架计算")
print(A1)   