# Installation Guide (Linux Only)

## Prerequisites

Ensure your system has the following packages installed:

- `qt6-base-dev`: Required for building CGAL with Qt6 support.
- `libboost-all-dev`: Provides Boost libraries, needed by CGAL.
- `libgmp3-dev`: For arbitrary precision arithmetic, used by GetFEM.
- `libmpfr-dev`: Supports precise floating-point arithmetic, used by GetFEM.
- `libqhull-dev`: Handles convex hull computations, used by GetFEM.
- `libsuperlu-dev`: Provides a linear solver for GetFEM.

```sh
sudo apt install qt6-base-dev libboost-all-dev libgmp3-dev libmpfr-dev libqhull-dev libsuperlu-dev
```

## CGAL Installation

```sh
git clone --branch v6.0.1 https://github.com/CGAL/cgal.git
cd cgal
mkdir build
cd build
cmake ..
sudo make install -j4
```
## SuperLU Installation (Static Build)

We install the libsuperlu-dev package via apt to ensure that the required macros are set up so GetFEM can locate it. However, GetFEM requires a statically built version of SuperLU, which must be compiled manually.

```sh
git clone https://github.com/xiaoyeli/superlu.git
cd superlu
mkdir build
cd build
cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON
make CFLAGS="-fPIC" CXXFLAGS="-fPIC" -j4
sudo make install
```

## GetFEM Installation

```sh
git clone https://github.com/getfem/getfem.git
cd getfem
mkdir build
cd build
cmake ..
sudo make install -j4
```