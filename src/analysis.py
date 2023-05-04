import os
import psutil
import subprocess
import time
import matplotlib.pyplot as plt
import pandas as pd

# Define the input directory and algorithm executables
dense_input_dir = "../data/dense"
sparse_input_dir = "../data/sparse"
jen_exec = "./jen"
tarjan_exec = "./tarjan"

plots_dir = "../plots/"

# Initialize lists to store the data
dense_file_names = []
dense_jen_runtimes = []
dense_tarjan_runtimes = []
dense_jen_mem_usages = []
dense_tarjan_mem_usages = []
dense_node_count = []


sparse_file_names = []
sparse_jen_runtimes = []
sparse_tarjan_runtimes = []
sparse_jen_mem_usages = []
sparse_tarjan_mem_usages = []
sparse_node_count = []



# Loop over all input files in the directory
count = 0
for file_name in os.listdir(dense_input_dir):
    if file_name.endswith(".txt"):
        # count += 1
        # if(count >10):
        #     break
        file_path = os.path.join(dense_input_dir, file_name)
        
        # Run the jen algorithm and measure runtime and memory usage
        jen_start_time = time.monotonic()
        jen_process = psutil.Process()
        jen_process.memory_info().rss
        subprocess.run([jen_exec, file_path], stdout=subprocess.DEVNULL)
        jen_elapsed_time = time.monotonic() - jen_start_time
        jen_mem_usage = jen_process.memory_info().rss
        
        # Run the tarjan algorithm and measure runtime and memory usage
        tarjan_start_time = time.monotonic()
        tarjan_process = psutil.Process()
        tarjan_process.memory_info().rss
        subprocess.run([tarjan_exec, file_path], stdout=subprocess.DEVNULL)
        tarjan_elapsed_time = time.monotonic() - tarjan_start_time
        tarjan_mem_usage = tarjan_process.memory_info().rss
        
        # Store the data in the lists
        dense_file_names.append(file_name)
        dense_jen_runtimes.append(jen_elapsed_time)
        dense_tarjan_runtimes.append(tarjan_elapsed_time)
        dense_jen_mem_usages.append(jen_mem_usage)
        dense_tarjan_mem_usages.append(tarjan_mem_usage)

        f = open(file_path)
        dense_node_count.append(int(f.readline().strip("\n")))
        f.close()
        
        # Print the summary of results
        # print(f"File: {file_name}")
        # print(f"Jen algorithm runtime: {jen_elapsed_time:.3f} seconds, memory usage: {jen_mem_usage} bytes")
        # print(f"Tarjan algorithm runtime: {tarjan_elapsed_time:.3f} seconds, memory usage: {tarjan_mem_usage} bytes")

count = 0
for file_name in os.listdir(sparse_input_dir):
    if file_name.endswith(".txt"):
        # count += 1
        # if(count > 10):
        #     break;
        file_path = os.path.join(sparse_input_dir, file_name)
        
        # Run the jen algorithm and measure runtime and memory usage
        jen_start_time = time.monotonic()
        jen_process = psutil.Process()
        jen_process.memory_info().rss
        subprocess.run([jen_exec, file_path], stdout=subprocess.DEVNULL)
        jen_elapsed_time = time.monotonic() - jen_start_time
        jen_mem_usage = jen_process.memory_info().rss
        
        # Run the tarjan algorithm and measure runtime and memory usage
        tarjan_start_time = time.monotonic()
        tarjan_process = psutil.Process()
        tarjan_process.memory_info().rss
        subprocess.run([tarjan_exec, file_path], stdout=subprocess.DEVNULL)
        tarjan_elapsed_time = time.monotonic() - tarjan_start_time
        tarjan_mem_usage = tarjan_process.memory_info().rss
        
        # Store the data in the lists
        sparse_file_names.append(file_name)
        sparse_jen_runtimes.append(jen_elapsed_time)
        sparse_tarjan_runtimes.append(tarjan_elapsed_time)
        sparse_jen_mem_usages.append(jen_mem_usage)
        sparse_tarjan_mem_usages.append(tarjan_mem_usage)

        f = open(file_path)
        sparse_node_count.append(int(f.readline().strip("\n")))
        f.close()




dense_table_data = {'Input': dense_file_names,'Nodes': dense_node_count,'Jen Time': dense_jen_runtimes, 'Tarjan Time': dense_tarjan_runtimes,'Jen Memory': dense_jen_mem_usages,
             'Tarjan Memory': dense_tarjan_mem_usages}
dense_df = pd.DataFrame(dense_table_data)

sparse_table_data = {'Input': sparse_file_names,'Nodes': sparse_node_count,'Jen Time': sparse_jen_runtimes, 'Tarjan Time': sparse_tarjan_runtimes,'Jen Memory': sparse_jen_mem_usages,
             'Tarjan Memory': sparse_tarjan_mem_usages}
sparse_df = pd.DataFrame(sparse_table_data)



sparse_plot = sparse_df.drop("Input",axis=1).groupby('Nodes',as_index=False).mean().sort_values('Nodes',ascending=True)
dense_plot = dense_df.drop("Input",axis=1).groupby('Nodes',as_index=False).mean().sort_values('Nodes',ascending=True)

# sparse_plot = sparse_plot.sort_values('Nodes',ascending=True)
# print(sparse_plot.head(5))

sparse_plot.plot(x='Nodes',y=['Jen Time','Tarjan Time'],kind="line")
plt.savefig("../output/sparse_runtime.png")

dense_plot.plot(x='Nodes',y=['Jen Time','Tarjan Time'],kind="line")
plt.savefig("../output/dense_runtime.png")

sparse_plot.to_excel("sparse.xlsx",index=False)
dense_plot.to_excel("dense.xlsx",index=False)


