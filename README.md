# C-C-tasks
### Task:
A string is written in the file. The program must read the file name from the first command line argument and calculate the number of such characters in this line that are in the alphabet after the character passed as the third command line argument. To calculate the number of characters, the program must create N child processes (N is passed by the second command line argument) and pass each of them a part of the resulting string. Each of the child processes must calculate the number of characters corresponding to the condition in the part of the string passed to it and return it to the parent. The parent process should sum up the numbers received from the children and output the total amount to the console. If the source file does not exist, or less than 2 characters are written in it, you should output the appropriate message to the user and shut down the program. The position in the alphabet in this problem refers to the position in the ASCII table.

### windowsProcessesApi.cpp and taskSolution.cpp (for Windows)
It works with .exe of taskSolution.cpp file (.cpp file can be compiled like: ```g++ <file.cpp>```).
To run this code it is needed to create a file with a string longer than 1 symbol. Everything should be located in one place. There it can be run.
Run: ```./windowsProcessesApi.exe <filename> <N> <letter>```, where ```<N>``` is a number of subprocesses, which will divide the string to solve a task.

### linuxProcesses.cpp (for Linux)
It is only needed to create file.txt, to compile linuxProcesses.cpp file and run it.


### cross-platformСodeProcesses.py (for any OS)
To run such script use: ```python cross-platformСodeProcesses.py <filename> <N> <letter>```.

### The same task with threads
The same solution programs:
For Windows: `windowsThreadsApi.cpp`
For Linux: `linuxThreads.cpp`
For any OS: `cross-platformThreads.cpp`

### Example:
I created file.txt and wrote there ```abcd efab XyZ```.
I ran the program and the result is:
```There are 5 letters, which are after letter 'd' in alphabet```.
