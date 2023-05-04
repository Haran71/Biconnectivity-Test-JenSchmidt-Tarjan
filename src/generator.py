import os
import random
import sys

# Define the number of files to generate
num_files = 300

# Define the range of possible values for N
min_N = 50
max_N = 200


# Define the probability of an outgoing edge existing
p = 0.8 if sys.argv[1] == "dense" else 0.1

# Define the directory to save the files in
graph_type = sys.argv[1]
dir_name = '../data/' + graph_type



# Create the directory if it doesn't exist
if not os.path.exists(dir_name):
    os.mkdir(dir_name)

# Generate 100 input files with random N and edges
for i in range(num_files):
    # Generate random N for this file
    N = random.randint(min_N, max_N)

    # Generate random edges for each node
    edges = {}
    for j in range(N):
        outgoing_edges = [k for k in range(N) if random.random() < p and k != j]
        edges[j] = outgoing_edges

    # Write the edges to a file in the desired format
    file_name = f"{dir_name}/{graph_type}_graph_{i}.txt"
    with open(file_name, 'w') as f:
        f.write(str(N) + '\n')
        for j in range(N):
            outgoing_edges = ' '.join(str(k) for k in edges[j])
            f.write(outgoing_edges + '\n')
