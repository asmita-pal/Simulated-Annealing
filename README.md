# Simulated-Annealing
Simulated Annealing algorithm for a Slicing Floorplan. 

1. Parameters
They are of the following form:
2.0(*chipratio *)
10 (* nmoves *)
0.85 (* ratio *)
-1 (* t0 *)
0.005 (* lambdatf *)
3 (* iseed *)
6 (* n *)
0.99 (* P *)
0.001 (* epsilon *)
chipratio:Final chip ratio must be between 1/chipratio and chipratio;
chipratio must be greater than 1.0
nmoves:Number of attempt moves per temperature iteration. Use 10
as the default value. Larger values of nmoves in general
produce better results. The total execution time of the
program grows linearly with respect to nmoves.
Ratio:Ratio for decreasing temperatures. Use 0.85 as the default
value. Larger values of ratios in general produce better
results and have much longer execution time.
t0:Initial temperature. If the input t0 is negative (e.g. -1), the
program will compute an initial temperature (in this case you
will need to add a negative sign to make the initial
temperature positive); otherwise the program will use the
input positive t0 as the initial temperature. Default value is
-1.
lambdatf:The program will change ratio to 0.1 when temperature falls
below lambdatf * t0. After ratio is set to 0.1, the program in
general will terminate very quickly.
iseed:Seed for random number generator.
n:Number of modules (this will depend on your input file).
P:Initial probability of accepting uphill moves
N:Calculated using N = nmoves * n
