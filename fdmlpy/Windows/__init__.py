import os
os.add_dll_directory("@Boost_LIBRARY_DIRS@")
os.add_dll_directory("@GMP_LIBRARY_DIR@")
os.add_dll_directory("@MPFR_LIBRARY_DIR@")
from .fdmlpy import *