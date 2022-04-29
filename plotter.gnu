# #### LATTICE IMAGE MATRIX ####
# Ifile =  "../Data/lattice.dat"
# Ifile2 =  "../Data/lattice2.dat"
# Ofile = "../Plots/lattice.png"
# Ofile2 = "../Plots/lattice2.png"
# set terminal pngcairo enhanced font ",18" size 1600,1500
# set output Ofile
# set xrange [0:]
# set yrange [0:]

# set xtics 0,50,500
# set ytics 0,50,500
# #set grid front xtics ytics lw 3 lc rgb 'blue'
# unset key
# set palette color 
# plot Ifile matrix w image
# set output Ofile2
# plot Ifile2 matrix w image
################################################################
# SPINS - MAGNETIZATION PER SWEEP AT ONE TEMPERATURE
# Ifile = "../Data/spins.dat"
# Ofile = "../Plots/spinsHighTHighiter.png"
# set terminal pngcairo enhanced font ",18" size 1600,1500
# set output Ofile
# #set xrange [-500:]
# #set yrange [:1.1]
# unset key
# set style line 1 lc rgb "black" pt 7 ps 0.5
# set title "Average Spin over MC Time @ T = 0.01"
# plot Ifile ls 1 with lines
################################################################
## #ENERGIES
# Ifile = "../Data/energies.dat"
# Ofile = "../Plots/energiesHighTHighiter.png"
# set terminal pngcairo enhanced font ",22" size 1600,1500
# set output Ofile
# unset key
# set style line 1 lc rgb "black" pt 7 ps 0.5 
# set title "Energy over Time @ T = 0.01"
# plot Ifile ls 1 with lines 
################################################################
### MAGNETIZATION VS TEMPERATURE
# Ifile = "../Data/MvsT100.dat"
# Ofile = "../Plots/MvsT100.png"
# set terminal pngcairo enhanced font ",24" size 1700,1400
# set output Ofile
# #set xrange [6:]
# set yrange [:1.1]
# set key
# set style line 1 lc rgb "black" lw 2 ps 2
# set style line 2 lc rgb "red" lw 1.5
# set style line 3 lc rgb "blue" pt 22 ps 1
# set title "Average Magnetization at Equilibrium per Temperature for n = 100"
# set xlabel 'T [Temperature]'
# set ylabel 'M/N'
# set arrow from 15.88,-0.78 to 15.88,1.05 nohead ls 2 dt 5
# plot Ifile ls 1 with lines title "M(T)/N", \
# 1/0 ls 2 dt 2 title "Analytical Tc = 15.88"
# #FINAL - 10000 SWEEPS n = 100
################################################################
### LOG MAGNETIZATION VS LOG TEMPERATURE
# Ifile = "../Data/logMvslogT40.dat"
# Ofile = "../Plots/logMvslogT40.png"
# set terminal pngcairo enhanced font ",24" size 1700,1400
# set output Ofile
# set xrange [0:2.7]
# set yrange [-0.13:0.01]
# set key bottom right
# set style line 1 lc rgb "black" pt 7 ps 1
# set style line 2 lc rgb "red" lw 1 
# set title "log(M) at Equilibrium per Logarithmic Temperature around Tc for n = 40"
# set xlabel 'log(Tc - T)'
# set ylabel 'log(M)'
# set grid
# f(x) = m*x + b
# fit f(x) Ifile every ::110::140 via m,b
# plot Ifile ls 1 title "log(M)", f(x) ls 2 title "y = 0.0952x - 0.15"
# plot Ifile ls 1 title "log(M)"
# # # #m = 0.089605 -> B = 1/11,  n = 20
# # # m = 0.0894241 -> B = 1/11,  n = 40
# # 28 percent error from actual (1/8)

################################################################
### ENERGY per TEMPERATURE
# Ifile = "../Data/EvsT.dat"
# Ofile = "../Plots/EvsT.png"
# set terminal pngcairo enhanced font ",24" size 1500,1200
# set output Ofile
# unset key
# set style line 1 lc rgb "black" pt 7 ps 0.5
# set title "Energy at equilibrium per Temperature"
# set xlabel 'T (Temperature)'
# set ylabel 'Energy At Equilibrium'

# plot Ifile ls 1 with lines
################################################################
# ### SPECIFIC HEAT per TEMPERATURE
# Ifile = "../Data/CvsT30.dat"
# Ifile2 = "../Data/CvsT10.dat"
# Ifile4 = "../Data/CvsT50.dat"
# Ofile = "../Plots/CvsT50.png"

# set terminal pngcairo enhanced font ",24" size 1600,1500
# set output Ofile
# unset key

# set style line 1 lc rgb "black" lw 2 ps 0.5
# set style line 4 lc rgb "orange" lw 2 ps 0.5
# set style line 2 lc rgb "red" lw 2
# set style line 3 lc rgb "blue" lw 2
# set style line 5 lc rgb "violet" lw 2
# set style line 6 lc rgb "brown" lw 2
# set style line 7 lc rgb "cyan" lw 2

# set title "Specific Heat per Spin per Temperature At n = 50"
# set xlabel 'T (Temperature)'
# set ylabel 'C/N'
# set key top right
# #set xrange [5:25]
# #set yrange [0.1:0.35]
# #set arrow from 15.88,0.1 to 15.88,0.33 nohead ls 2 dt 2
# #set arrow from 16.1,0.1 to 16.1,0.33 nohead ls 3 dt 2
# #plot Ifile4 ls 1 with lines title "C(T)/N"


# # SUPERPOSITION OF SPECIFIC HEATS
# Ofile2 = "../Plots/CvsTCOMBO.png"
# set output Ofile2
# unset arrow
# set arrow from 15.88,0 to 15.88,0.75 nohead ls 2 dt 4
# set arrow from 16.5,0 to 16.5,0.75 nohead ls 3 dt 4
# set arrow from 16.1,0 to 16.1,0.75 nohead ls 7 dt 4 
# set key top right
# set title "Specific Heat per Spin per Temperature for n = 10,30,50"
# plot Ifile2 ls 4 title "n = 10" with lines, Ifile ls 1 title "n = 30" with lines,\
# Ifile4 ls 6 title "n = 50" with lines,\
# 1/0 title "Analytical Tc = 15.88" ls 2 dt 4,\
# 1/0 title "C max for n = 10, T=16.5" ls 3 dt 4,\
# 1/0 title "C max for n = 30,60 T=16.1" ls 7 dt 4
# n = 10 at 200000 sweeps -> Peak at 16.2
# n = 30 at 50000 sweeps -> Peak at 16.1
# n = 60 at 20000 sweeps -> Peak at 16

################################################################
### Magnetization per H
# Ifile = "../Data/MvsHSMALL.dat"
# Ofile = "../Plots/MvsHSMALL.png"
# set terminal pngcairo enhanced font ",24" size 1500,1200
# set output Ofile
# unset key
# set style line 1 lc rgb "black" lw 2 ps 0.5
# set style line 2 lc rgb "black" pt 22 ps 2
# set title "Magnetization per Spin at Equilibrium per H for n = 30 at T = Tc/3 = 5.3"
# set xlabel 'H [External Magnetic Field]'
# set ylabel 'M/N'
# set xrange [:5]
# set yrange [-1.1:1.1]

# plot Ifile ls 1 with lines, Ifile ls 2
# Ifile2 = "../Data/MvsHBIG.dat"
# Ofile2 = "../Plots/MvsHBIG.png"
# set output Ofile2
# set title "Magnetization per Spin at Equilibrium per H for n = 30 at T = 1.5Tc = 23.82"
# unset xrange
# plot Ifile2 ls 1 with lines,Ifile2 ls 2
