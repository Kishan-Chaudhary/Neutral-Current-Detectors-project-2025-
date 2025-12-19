import re
import numpy as np
import matplotlib.pyplot as plt
from skimage.filters import gaussian
from PIL import Image
from scipy.signal import convolve2d

# --------------------------------------------------
# 1. Load a real photo
# --------------------------------------------------
img = Image.open("figure1.jpg").convert("L")   # use any normal photo
img = img.resize((100, 100))                    # resize for speed
true_img = np.array(img, dtype=float)
true_img /= true_img.max()                       # normalize

img = Image.open("figure1.jpg")
img = img.resize((100, 100))
img_arr = np.array(img, dtype=float) / 255.0

R = img_arr[:,:,0]
G = img_arr[:,:,1]
B = img_arr[:,:,2]


# --------------------------------------------------
# 4. 2D MLEM / Richardson–Lucy deblurring
# --------------------------------------------------
def richardson_lucy(image, psf, iterations):
    estimate = np.full_like(image, 1.0)
    psf_mirror = psf[::-1, ::-1]

    for _ in range(iterations):
        relative_blur = image / (convolve2d(estimate, psf, "same", boundary="symm") + 1e-8)
        estimate *= convolve2d(relative_blur, psf_mirror, "same", boundary="symm")
        print("Iteration:",iterations,"\n")
    return estimate
# --------------------------------------------------
# 2. Define a blur kernel (PSF)
# --------------------------------------------------
#Astro photo settings size=20, sigma=40.0. Steffon size =5, sigma =20 natural, 
def gaussian_psf(size=11, sigma=2.0):
    x = np.arange(size) - size//2
    X, Y = np.meshgrid(x, x)
    g = np.exp(-(X**2 + Y**2) / (2*sigma**2))
    return g / g.sum()

def motion_kernel(length=21, angle=30):
    x = np.linspace(-1,1,length)
    kernel = np.zeros((length,length))
    kernel[len(x)//2, :] = 1
    kernel = gaussian(kernel, sigma=1)
    # rotate kernel to desired angle
    from scipy.ndimage import rotate
    return rotate(kernel, angle, reshape=False)


def circular_psf(radius):
    y,x = np.ogrid[-radius:radius+1, -radius:radius+1]
    mask = x**2 + y**2 <= radius**2
    psf = mask.astype(float)
    return psf / psf.sum()

def add_poisson(img, peak_counts):
    counts = np.clip(img, 0, None) * peak_counts
    noisy = np.random.poisson(counts)
    return noisy / peak_counts

def airy_psf(size=21, radius=5.0):
    x = np.linspace(-size//2, size//2, size)
    X, Y = np.meshgrid(x, x)
    r = np.sqrt(X**2 + Y**2)
    # Bessel function (Airy pattern)
    from scipy.special import j1
    eps = 1e-12
    airy = (2 * j1(np.pi * r / radius + eps) / (np.pi * r / radius + eps))**2
    airy /= airy.sum()
    return airy

def rotational_psf(size=23, angle=45, sigma=4):
    psf = np.zeros((size, size))
    center = size // 2
    y, x = np.indices((size, size))
    theta = np.radians(angle)
    rot_x = (x - center) * np.cos(theta) - (y - center) * np.sin(theta)
    rot_y = (x - center) * np.sin(theta) + (y - center) * np.cos(theta)
    psf = np.exp(-(rot_x**2 + rot_y**2) / (2*sigma**2))
    psf /= psf.sum()
    return psf

def anisotropic_gaussian(size=21, sx=3.0, sy=1.0, angle=0):
    x = np.arange(size) - size//2
    X, Y = np.meshgrid(x, x)
    theta = np.radians(angle)
    Xr = X*np.cos(theta) - Y*np.sin(theta)
    Yr = X*np.sin(theta) + Y*np.cos(theta)
    psf = np.exp(-(Xr**2/(2*sx**2) + Yr**2/(2*sy**2)))
    psf /= psf.sum()
    return psf

def random_sparse_psf(size=21, density=0.1):
    psf = np.zeros((size, size))
    n = int(size*size*density)
    idx = np.random.choice(size*size, n, replace=False)
    psf.flat[idx] = np.random.rand(n)
    psf = gaussian(psf, sigma=1)  # smooth for stability
    psf /= psf.sum()
    return psf

# Create individual PSFs
psf_gauss = gaussian_psf()
psf_circular = circular_psf(5)
psf_motion = motion_kernel()
poisson = add_poisson(psf_gauss, peak_counts=100)
psf_airy = airy_psf()
psf_rotational = rotational_psf()
psf_anisotropic_gauss = anisotropic_gaussian()
psf_random_sparse = random_sparse_psf()

# Combine PSFs by convolution
combined_psf = psf_gauss
combined_psf = convolve2d(combined_psf, psf_circular, mode="same")
combined_psf = convolve2d(combined_psf, psf_motion, mode="same")
combined_psf = convolve2d(combined_psf, psf_airy, mode="same")
combined_psf = convolve2d(combined_psf, psf_rotational, mode="same")
combined_psf = convolve2d(combined_psf, psf_anisotropic_gauss, mode="same")
combined_psf = convolve2d(combined_psf, psf_random_sparse, mode="same")

combined_psf = np.maximum(combined_psf, 0)
combined_psf /= combined_psf.sum()
# --------------------------------------------------
# 3. Blur the image (simulate measurement)
# --------------------------------------------------

blurred_R = convolve2d(R, combined_psf, mode="same", boundary="symm")
blurred_G = convolve2d(G,combined_psf,mode="same", boundary="symm")
blurred_B = convolve2d(B,combined_psf,mode="same", boundary="symm")



blurred_image = convolve2d(true_img, combined_psf, mode="same", boundary="symm")


reconstructed_R = richardson_lucy(blurred_R, combined_psf, iterations=30)
reconstructed_G = richardson_lucy(blurred_G, combined_psf, iterations=30)
reconstructed_B = richardson_lucy(blurred_B, combined_psf, iterations=30)


reconstructed_img = richardson_lucy(blurred_image, combined_psf, iterations=10)




# --------------------------------------------------
# 5. Plot the results
# --------------------------------------------------



plt.figure(figsize=(4,4))


#plt.subplot(1,3,1)
plt.title(" Original photo")
plt.imshow(true_img,cmap="gray")
plt.axis("off")
plt.show()

#plt.subplot(1,3,2)
plt.title("Blurred")
plt.imshow(blurred_image,cmap="gray")
plt.axis("off")
plt.show()

#plt.subplot(1,3,3)
plt.title("Reconstructed photo")
plt.imshow(reconstructed_img,cmap="gray")
plt.axis("off")
plt.show()




reconstructed = np.dstack([reconstructed_R, reconstructed_G, reconstructed_B])
blurred_RGB = np.dstack([blurred_R, blurred_G, blurred_B])
#plt.subplot(1,3,1)
plt.title(" Original photo")
plt.imshow(img_arr)
plt.axis("off")
plt.show()

#plt.subplot(1,3,2)
plt.title("Blurred")
plt.imshow(blurred_RGB)
plt.axis("off")
plt.show()

#plt.subplot(1,3,3)
plt.title("Reconstructed photo")
plt.imshow(reconstructed)
plt.axis("off")
plt.show()
