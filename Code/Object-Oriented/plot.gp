set term png
set logscale x
set label at 1,1
set output "S_vs_t_sizes.png"
plot "S_vs_t_L100.txt" w l, "S_vs_t_L200.txt" w l, "S_vs_t_L300.txt" w l, "S_vs_t_L400.txt" w l, "S_vs_t_L500.txt" w l