clang++ -O3 -Wno-c++11-compat-deprecated-writable-strings -Wno-parentheses -Wno-writable-strings -std=c++0x taas-fargo.cpp -o taas-fargo `pkg-config --cflags --libs glib-2.0` -lm
