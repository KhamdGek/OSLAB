# UNIX Shell
## Lab Report 2
Shell.c file contains the c code which was provided in the lab. 
The code should be run using a terminal and commands entered would be parsed. The history commands can be retireved using !! which would also be executed in the terminal diplaying the output of the code.

### Lab task Part A and Part III has been implemented in `Shell.c` file. For testing run following command

`gcc -o Shell Shell.c`

`./Shell`

'ls -al' # run any command so that later can be retrieve using History feature

`!!`


### Lab task Part IV has been implemented in `main.c` file. For testing run following command

`gcc -o main main.c`

`./main`
#### For imput redirection

`ls > out.txt`

`cat out.txt`

#### For output redirection

`sort < in.txt`