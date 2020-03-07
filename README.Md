# Simple Path Finding Algorithm

This is a basic implementation of ``Uniform Cost Search``, which is just a more optimized variation of ``Breath First Search`` making use of a priority queue.

The project can be compiled using,
````
g++ find_route.cpp -o <outfile_name>
````
and then run using
```
./find_route <input_filename> <start_location> <destination_location>
```

a sample compilation and run would be,
```
g++ find_route.cpp -o find_route
./find_route input1.txt Bremen Kassel
```

Along with ``Uniform Cost Search``, if a heuristic file is provided the program will use ``A*``, which is compiled in the same way, but run using,
```
./find_route <input_filename> <start_location> <destination_location> <heuristic_file>
```

The basic structures of all the input files are in this directory.