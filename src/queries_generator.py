import csv

def transform_coordinates(input_file, output_file, i):
    with open(input_file, mode='r') as infile, open(output_file, mode='w', newline='') as outfile:
        reader = csv.reader(infile)
        writer = csv.writer(outfile)

        # Read each row and apply the transformation
        for row in reader:
            # Extract the original X, Y values and operation
            X1, Y1, X2, Y2, operation = float(row[0]), float(row[1]), float(row[2]), float(row[3]), row[4]

            # Apply the transformations
            new_X1 = X1 + X1 * i
            new_Y1 = Y1 + Y1 * i
            new_X2 = X2 - X2 * i
            new_Y2 = Y2 - Y2 * i

            # Write the new row to the output file
            writer.writerow([new_X1, new_Y1, new_X2, new_Y2, operation])

# Define the value of i
i = 0.1  # Example value

# Provide input and output file paths
input_file = 'query.csv'
output_file = 'output_' + str(i) + '.csv'

# Call the function to perform the transformation
transform_coordinates(input_file, output_file, i)

print(f"Transformation completed. New CSV saved as {output_file}.")
