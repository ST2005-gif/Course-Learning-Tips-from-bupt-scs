#2023210896-王书翰
import numpy as np
from sympy import Matrix
def caculateJ(A):
    A=Matrix(A)
    P,J=A.jordan_form()
    A=np.array(A)
    P=np.array(P)
    J=np.array(J)
    print("A=",A)
    print("J=",J)
def caculateP(A):
    A=Matrix(A)
    P,J=A.jordan_form()
    A=np.array(A)
    P=np.array(P)
    J=np.array(J)
    print("A=",A)
    print("P=",P)
A=np.array([[1,-1,2],[3,-3,6],[2,-2,4]])
caculateJ(A)
A=np.array([[-1,1,0],[-4,3,0],[1,0,2]])
caculateJ(A)
A=np.array([[1,-1,2],[3,-3,6],[2,-2,4]])
caculateP(A)
A=np.array([[1,2,3,4],[0,1,2,3],[0,0,1,2],[0,0,0,1]])
caculateP(A)