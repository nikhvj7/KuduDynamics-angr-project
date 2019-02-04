# KuduDynamics-angr-project

The goal of the project is to generate a ROP attack for three binaries using angr. The repo contains three binaries simplest.c, simpler.c and simplified.c and each of them contains a vulnerability which could be exploited to achieve a buffer overflow. We use angr to generate inputs which would create a segfault