import argparse
from collections import Counter
import re

def count_words(file_path):
    with open(file_path, 'r') as file:
        text = file.read().lower()

    words = re.findall(r'\b\w+\b', text)
    word_counts = Counter(words)
    sorted_word_counts = sorted(word_counts.items(), key=lambda x: x[1], reverse=True)

    return sorted_word_counts

def save_word_counts(word_counts, output_file):
    with open(output_file, 'w') as file:
        for word, count in word_counts:
            file.write(f'{word}: {count}\n')

def main(input_file, output_file):
    word_counts = count_words(input_file)
    save_word_counts(word_counts, output_file)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Count words in a text file and save word counts to another file.')
    parser.add_argument('input', type=str, help='Input file path')
    args = parser.parse_args()
    output_file = args.input.split(".")[0]+"_count.txt"
    main(args.input, output_file)

