#2023210896王书翰
import numpy as np
def create(A,B,C):
    a=A.T+A
    b=B.T+B
    c=C.T+C
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
    print(np.column_stack((x1,x2,x3)))

A=np.array([[-1,1],[0,0]])
B=np.array([[-1,0],[1,0]])
C=np.array([[0,0],[0,1]])
create(A,B,C)
A=np.array([[1,1],[-2,0]])
B=np.array([[-1,0],[1,1]])
C=np.array([[-1,0],[1,-1]])
create(A,B,C)