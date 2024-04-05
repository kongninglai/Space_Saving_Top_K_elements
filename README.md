# Space_Saving_Top_K_elements
Implement a data mining algorithm in C which approximately finds the top-K heavy hitters (or frequent items) after making a single pass over a data stream.

## Reference
Algorithm: Efficient Computation of Frequent and Top-k Elements in Data Streams
https://home.cse.ust.hk/~raywong/comp5331/References/EfficientComputationOfFrequentAndTop-kElementsInDataStreams.pdf

Hash function for words: uthash
https://github.com/troydhanson/uthash

## How to run
- First `make` to make the execuatable find_freq_items
- Run program with command `./find_freq_items file_path k`
- It will output the top k elements in given file.
- Note that the capacity can be changed in `find_freq_items.h` 
- 
- Also implemented the top k' elements as the algorithm described in paper,
- which will output the top k' ~= k elements with guaranteed and order.
- To use top k' instead of top k, call `find_top_k_more(k);` in line 415 `find_freq_items.c`


## How to test
- I have four test files in folder `test/`: `simple1.txt`, `simple2.txt`, `poems.txt`, `paragraph.txt`, with their actual sorted word counts `*_count.txt`
- The word counts files are created by `validate.py`, which count the given input file's words using `re.findall`
- To use `validate.py`, call `python3 validate.py file_path`, it will automatically save the word counts in `*_count.txt`
- There's also `ascii_encoder.py`, call `python3 ascii_encoder.py input_file output_file` to convert input_file to `ascii-encoded` and save in `output_file`
## Logs
2024/04/04: 
- Finish file reading and tokenization. Next step is hash the tokenized word.
- Finish hash function. Next we can begin to implement the algorithm.

2024/04/05:
- Finish algorithm. Simple test passed. Next step is to create test database
- and implement the argument parser.
- Fixed one small bug.
- Finished argumant parser and test database.
