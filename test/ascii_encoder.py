import argparse
def ascii_encode(input_file, output_file):
    
    # read the input file
    with open(input_file, 'r') as f:
        text = f.read()

    # encode the input file with ascii
    ascii_text = text.encode('ascii', 'ignore')

    # write into output file
    with open(output_file, 'wb') as f:
        f.write(ascii_text)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Encode the input file with ascii and save to output file')
    parser.add_argument('input', type=str, help='Input file path')
    parser.add_argument('output', type=str, help='Output file path')
    args = parser.parse_args()
    ascii_encode(args.input, args.output)

