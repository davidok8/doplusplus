import numpy as np

import pylab


def tangent_of_angle_from_optical_axis(points):
    # All the 2D points correspond to 3D points lying on the plane z = 1.
    x = points[:, 0]
    y = points[:, 1]
    z = 1.
    return np.sqrt(x ** 2 + y ** 2) / z


def radial_distance(points):
    # We assume in this exercise that the optical center is at the origin
    # (0, 0, 0).
    x = points[:, 0]
    y = points[:, 1]
    return np.sqrt(x ** 2 + y ** 2)


def rectilinear(points, f):
    print('rectilinear')
    tan_theta = tangent_of_angle_from_optical_axis(points)
    r = f * tan_theta
    return r


def equidistant(points, f):
    print('equidistant')
    theta = np.arctan(tangent_of_angle_from_optical_axis(points))

    r = 2 * f * np.tan(theta / 2)
    return r


def equisolid_angle(points, f):
    print('equisolid_angle')
    theta = np.arctan(tangent_of_angle_from_optical_axis(points))
    r = 2 * f * np.sin(theta / 2)
    return r


def fisheye(points, f, mapping_fn):
    # Apply the mapping function.
    r = mapping_fn(points, f)
    r = r[:, np.newaxis]

    # Recover the unit orientation vector on which to apply the radial
    # transformation.
    angle = np.arctan2(points[:, 1], points[:, 0])
    u = np.stack((np.cos(angle), np.sin(angle))).T

    # The distorted points.
    return r * u


def opensfm_fisheye(points, f, k1=1e-1, k2=1e-1):
    # All the 2D points correspond to 3D points lying on the plane z = 1.
    x = points[:, 0]
    y = points[:, 1]
    r2 = x ** 2 + y ** 2
    r = np.sqrt(r2)
    theta = np.arctan(r)
    d = 1 + k1 * r2 + k2 * theta ** 4
    u = f * d * theta * x/r
    v = f * d * theta * y/r
    return np.stack((u, v)).T


def opensfm_spherical(points):
    # All the 2D points correspond to 3D points lying on the plane z = 1.
    x = points[:, 0]
    y = points[:, 1]
    r = np.sqrt(x ** 2 + y ** 2)
    lon = np.arctan(x)
    lat = -np.arctan(-y/r)
    u = lon / (2 * np.pi)
    v = lat / (2 * np.pi)
    return np.stack((u, v)).T


# The points.
a1 = np.arange(-1, 1, 0.1, dtype=np.float)
a1 = np.stack((np.ones(len(a1)), a1)).T

a2 = np.arange(-1, 1, 0.1, dtype=np.float)
a2 = np.stack((a2, np.ones(len(a2)))).T


# The focal length.
f = 1.2

# The 3D points lying on the plane z = 1.
a = np.concatenate((a1, a2))

# Apply the fisheye distortion model.
a_d = [fisheye(a, f, rectilinear),
       fisheye(a, f, equidistant),
       fisheye(a, f, equisolid_angle),
       opensfm_fisheye(a, f),
       opensfm_spherical(a)]

pylab.scatter(a[:, 0], a[:, 1])
for a_di in a_d:
    pylab.scatter(a_di[:, 0], a_di[:, 1])


pylab.show()
