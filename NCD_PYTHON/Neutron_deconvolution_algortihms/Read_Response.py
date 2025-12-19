import numpy as np
import pandas as pd

##########################################################################
#Response for 32 energy bin, 3 NCD[cylindrical source(19 cm radius, 227 cm length), triton count/fluence from source, binned]
data1 = pd.read_csv("3NCD_Response_32_Nibi.csv", encoding='latin1', header=None)
#data1 = pd.read_csv("Rebinned_HGResponseMatrix_6x6.csv", encoding='latin1', header=None)
df1 = pd.DataFrame(data1)

R1= [df1.iloc[3:,3].to_numpy(), #1-inch
        df1.iloc[3:,10].to_numpy(), #2-inch
        df1.iloc[3:,17].to_numpy(), #3-inch
        df1.iloc[3:,24].to_numpy()  #bare
        ]

R1 = np.array(R1,dtype=float)
EnergiesHR = df1.iloc[3:,0]
print(EnergiesHR)
   

##############################################################################
#Response for 4*4 binned 3 NCD[cylindrical source(19 cm radius, 227 cm length), triton count/fluence from source, binned]
data2 = pd.read_csv("Rebinned_HGResponseMatrix_4x4.csv", encoding='latin1', header=None)
data2_error = pd.read_csv("Rebinned_HGResponseMatrix_4x4_Error.csv", encoding='latin1', header=None)
#data1 = pd.read_csv("Rebinned_HGResponseMatrix_6x6.csv", encoding='latin1', header=None)
df2 = pd.DataFrame(data2)
df2_error = pd.DataFrame(data2_error)

R2 =   [df2.iloc[:,0].to_numpy(), #1-inch
         df2.iloc[:,1].to_numpy(), #2-inch
         df2.iloc[:,2].to_numpy(), #3-inch
        df2.iloc[:,3].to_numpy()  #bare
        ]
R2_error = [df2_error.iloc[:,0].to_numpy(), #1-inch
            df2_error.iloc[:,1].to_numpy(), #2-inch
            df2_error.iloc[:,2].to_numpy(), #3-inch
            df2_error.iloc[:,3].to_numpy()  #bare
            ]

R2 = np.array(R2,dtype=float)
R2_error = np.array(R2_error,dtype=float)
print("Binned response:",R2,"\n")
bin_labels_2 = [
"Slow",
"Epithermal",
"0.1-4",
"4-11"
 ]
   

##############################################################################
#Response for 3*3 binned(1,2,bare) 3 NCD[cylindrical source(19 cm radius, 227 cm length), triton count/fluence from source, binned]
data3 = pd.read_csv("Rebinned_HGResponseMatrix_3x3.csv", encoding='latin1', header=None)
data3_error = pd.read_csv("Rebinned_HGResponseMatrix_3x3_Error.csv", encoding='latin1', header=None)
#data1 = pd.read_csv("Rebinned_HGResponseMatrix_6x6.csv", encoding='latin1', header=None)
df3 = pd.DataFrame(data3)
df3_error = pd.DataFrame(data3_error)

R3 =   [df3.iloc[:,0].to_numpy(), #1-inch
         df3.iloc[:,1].to_numpy(), #2-inch
        df3.iloc[:,2].to_numpy()  #bare
        ]
R3_error = [df3_error.iloc[:,0].to_numpy(), #1-inch
            df3_error.iloc[:,1].to_numpy(), #2-inch
            df3_error.iloc[:,2].to_numpy()  #bare
            ]

R3 = np.array(R3,dtype=float)
R3_error = np.array(R3_error,dtype=float)
print("Binned response:",R2,"\n")
bin_labels_3 = [
"Slow",
"Epithermal",
"Fast"
 ]
   



