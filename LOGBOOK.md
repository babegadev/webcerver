# Webcerver logbook
A place where I log my progress throughout this project. I will be documenting my learning process, taking notes, explaining my thought process, etc.

## Part 1: Learning fundamentals

### 1.1: Strings in c
I had to implement my own version of `strlen` and `strncmp`. I had already done this before on my learn c excercises, but it was a nice refresher. It strengthened my understanding of strings in c by just imagining that everything is flat, and in the case of strings, the end is terminated by `\0`. I also learned the importance of typecasting as the std strncmp function casts the type before comparing to prevent weird arithmetic stuffs.

### 1.2: Request line splitter
This was a challenging one. Normally, I would just call string.split(" ") function in python/js and be done with it. But now, I had to really rethink the way I program. I initially started by using while loops and an index tracker, going through the code until I reaches the req_length or if the current character is a space. At first, I had some problem with the indexing and the length, but then I managed to get a basic version working. However, I tested my code with several bad inputs and it couldn't handle many edge cases. I then created a helper function to traverse the current string up to a certain character while passing the I pointer. I used dereferencing to pass I between. Then, I ran into several issues with double spaces and etc, but I settled on the solution of returning a number through the helper function to determine the current subset length. It was a game changer and after adding some more error checking, I managed to handle all of the edge cases successfully. Btw it's 3am and I have a stats exam at 10 today I'm so cooked lol.

### 1.3: Reading a whole file into heap buffer of exactly the right size
Okay this one's pretty tough. I'm currently able to get the file size using fseek() and ftell(), and I can already allocate memory in heap using malloc(size) using the size from earlier. However, I'm still unable to write the whole text file to the output. I think i need to use a loop, but reference the pointer at certain index. Okay turns out there is another method called fread() that can read the whole file at once. I also learned that in these cases, passing a double pointer might be necessary, or instead we can use the idiomatic convention of returning a char* pointer instead like waht malloc does. Final improvements include error checking and freeing memory.

### 1.4: Dynamic array of items that doubles capacity when full
This one I think I have done before in my learn-c, where I created an IntVec, I think the key is to track len and cap. Completed the function(s) from the given function headers. Added checks to prevent size from overflowing by comparing to MAX_SIZE. Split the grow function into its own static helper.

## Part 2 - Tooling: compiler, debugger, sanitizers, make

### 2.1: Catching erros with tools:
- Off by one: ASan caught this. Running the compiled binary with -Wall and -Wextra does not give the warning and it fails silently.
- Use after free: ASan caught this one, but compiling with no flag and running the binary still flags an error.
- Read uninitialized: this one got caught by the -Wall flag.