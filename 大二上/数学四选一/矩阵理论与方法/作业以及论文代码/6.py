import numpy as np#2023210896-王书翰
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

vectors = np.array([[1, 1,0,0], [1,0,1,0], [-1,0,0,1],[1,-1,-1,1]])
orthonormal_vectors = schmidt(vectors)
print("正交化后的向量：")
for v in orthonormal_vectors:
    print(v)
