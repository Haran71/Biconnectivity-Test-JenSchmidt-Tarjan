#!/bin/bash

# Change to the src directory
cd src

# Compile the jen and tarjan algorithms
g++ -std=c++17 jen.cpp -o jen
g++ -std=c++17 tarjan.cpp -o tarjan

# Initialize the flag variable to true
match=true

# Loop over all input files in the data directory
for file in ../data/*.in
do
  echo "Checking $file..."
  
  # Run the first algorithm on the input file
  output1=$(./jen $file)
  
  # Run the second algorithm on the input file
  output2=$(./tarjan $file)
  
  # Compare the output of the two algorithms
  if [ "$output1" != "$output2" ]
  then
    # Set the flag variable to false if there is a mismatch
    match=false
    
    # Exit the loop as soon as a mismatch is found
    break
  fi
done

# Print the final result
if $match
then
  echo "True"
else
  echo "False"
fi
