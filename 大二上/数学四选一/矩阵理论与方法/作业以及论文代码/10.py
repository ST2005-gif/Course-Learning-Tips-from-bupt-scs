#2023210896王书翰
import numpy as np

def ex(A,n):
    exA=np.zeros_like(A,dtype=float)
    term=np.eye(A.shape[0],dtype=float)
    for i in range(n):
        exA=exA+term
        term=np.dot(term,A)/(i+1)
    return exA
def cosine(A,n):
    coA=np.zeros_like(A,dtype=float)
    term=np.eye(A.shape[0],dtype=float)
    cofficient=1
    for i in range(n):
        coA=coA+term*cofficient
        term=np.dot(term,A)
        term=np.dot(term,A)
        cofficient=cofficient*(-1)/((i+1)*(i+2))
    return coA

def sine(A,n):
    siA=np.zeros_like(A,dtype=float)
    tempA=A
    cofficient=1
    for i in range(n):
        siA=siA+tempA*cofficient
        tempA=np.dot(tempA,A)
        tempA=np.dot(tempA,A)
        cofficient=cofficient*(-1)/((i+1)*2)*((i+1)*2+1)
    return siA

A=np.array([[2,0,0],[1,1,1],[1,-1,3]])
print("例3.5",ex(A,10))
A=np.array([[3.14,0,0,0],[0,-3.14,0,0],[0,0,0,1],[0,0,0,0]])
print("例3.6",sine(A,20))
A=np.array([[4,6,0],[-3,-5,0],[-3,-6,1]])
print("例3.7",cosine(A,10))