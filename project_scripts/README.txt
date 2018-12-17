To execute script call either population_clang.cpp or population_cli.cpp.
Population_cli.cpp will create a binary that prints to the screen everything that it writes to the file.
population_clang.cpp will only write the file and not print to the screen.

The human_muted.h header file is used by population_clang.cpp and the human.h header file is used by population_cli.cpp.
Only difference between the 2 header files is that one prints to screen and the other one does not.

The plots and csv files were created from the the binary file: pop_cli_no_cout.bin.
Everything was generated from within the python script. The script calls the binary file and passes it the 7 required cli
parameters.
