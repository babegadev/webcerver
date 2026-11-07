# Webcerver logbook
A place where I log my progress throughout this project. I will be documenting my learning process, taking notes, explaining my thought process, etc.

## Part 1: Learning fundamentals

### 1.1: Strings in c
I had to implement my own version of `strlen` and `strncmp`. I had already done this before on my learn c excercises, but it was a nice refresher. It strengthened my understanding of strings in c by just imagining that everything is flat, and in the case of strings, the end is terminated by `\0`. I also learned the importance of typecasting as the std strncmp function casts the type before comparing to prevent weird arithmetic stuffs.

### 1.2: Request line splitter
This was a challenging one. Normally, I would just call string.split(" ") function in python/js and be done with it. But now, I had to really rethink the way I program. I initially started by using while loops and an index tracker, going through the code until I reaches the req_length or if the current character is a space. At first, I had some problem with the indexing and the length, but then I managed to get a basic version working. However, I tested my code with several bad inputs and it couldn't handle many edge cases. I then created a helper function to traverse the current string up to a certain character while passing the I pointer. I used dereferencing to pass I between. Then, I ran into several issues with double spaces and etc, but I settled on the solution of returning a number through the helper function to determine the current subset length. It was a game changer and after adding some more error checking, I managed to handle all of the edge cases successfully. Btw it's 3am and I have a stats exam at 10 today I'm so cooked lol.