import numpy as np
import pandas as pd

def mlem(R,data,x,tolerance):
    """
    R --> Response matrix, shape is (n,m)
    data --> pulse height spectrum, shape is (n,)
    x --> initial guess at neutron spectrum, shape is (m,)
    tolerance --> user-defined stopping condition
    """
    x = x.copy()
    n = len(data) # true is the recorded specturm
    m = len(x)
    J0 = 0 ; dJ0 = 1 ; ddJ = 1
    error = []
    #elimanate division by zero in vector calculation
    eps = 1e-10
    stepcount = 1
    while ddJ > tolerance:
        vector = np.zeros((n,))
        q = np.zeros((n,))
        for i in range(n):
            factor = (R[i,:]@x + eps)
            vector[i] = (data[i]+eps)/factor
            q[i] = factor

        for j in range(m):
            term = np.dot(R[:,j],vector)
            x[j] *= (1 / sum(R[:,j]))*term

        J = sum((data-q)**2) / sum(q)
        print(f"J MLEM = {J}")
        if(abs(J - 1) < 1e-1):
            print(f"Stopping early {stepcount} due to J convergence.")
            return(x,np.array(error))
            break

        dJ = J0 - J
        ddJ = abs(dJ-dJ0)
        error.append(ddJ)
        J0 = J
        dJ0 = dJ
        print("Iteration {}, dJ = {}".format(stepcount,ddJ))
        stepcount += 1



    return(x,np.array(error))


def mlem_updated(R,data,data_error, x,tolerance):
    """
    R --> Response matrix, shape is (n,m)
    data --> pulse height spectrum, shape is (n,)
    x --> initial guess at neutron spectrum, shape is (m,)
    tolerance --> user-defined stopping condition
    """
    x = x.copy()
    n = len(data) # true is the recorded specturm
    m = len(x)
    J0 = 0 ; dJ0 = 1 ; ddJ = 1
    error = []
    #elimanate division by zero in vector calculation
    eps = 1e-10
    stepcount = 1
    while ddJ > tolerance:
        vector = np.zeros((n,))
        q = np.zeros((n,))
        for i in range(n):
            factor = (R[i,:]@x + eps)
            vector[i] = (data[i]+eps)/factor
            q[i] = factor

        for j in range(m):
            term = np.dot(R[:,j],vector)
            x[j] *= (1 / sum(R[:,j]))*term

        J = sum((data-q)**2) / sum(q)
        print(f"J MLEM = {J}")
        if(abs(J - 1) < 1e-1):
            print(f"Stopping early {stepcount} due to J convergence.")
            return(x,np.array(error))
            break

        dJ = J0 - J
        ddJ = abs(dJ-dJ0)
        error.append(ddJ)
        J0 = J
        dJ0 = dJ
        print("Iteration {}, dJ = {}".format(stepcount,ddJ))
        stepcount += 1



    return(x,np.array(error))