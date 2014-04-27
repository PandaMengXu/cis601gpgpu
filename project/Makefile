
all:cpu_latency_boxplot cpu_busy cache_latency_size_boxplot cpu_latency_boxplot_cpuindex cache_busy

cpu_latency_boxplot: cpu_latency_boxplot.cc
	g++ -g -o $@ $^

cpu_busy: cpu_busy.cc
	g++ -g -o $@ $^
cpu_latency_boxplot_cpuindex: cpu_latency_boxplot_cpuindex.cc
	g++ -g -o $@ $^

cach_latency_size_boxplot: cache_latency_size_boxplot.cc
	g++ -g -O0 -o $@ $^
    
cache_busy: cache_busy.cc
	g++ -g -O0 -o $@ $^

clean:
	rm cpu_latency_boxplot cache_latency_size_boxplot cpu_latency_boxplot_cpuindex cache_latency_size_boxplot
