

input_1 = 'output_examples/allgen100x100'
input_2 = 'output.txt'

with open(input_1, 'r') as f:
    f1 = f.readlines()

with open(input_2, 'r') as f:
    f2 = f.readlines()

for i, line in enumerate(f1):
    if line != f2[i]:
        print(f"Fail in line {i+1}")