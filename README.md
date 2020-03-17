# CS-550 Section 01 Spring 2020 Assignment 02

## Character Device Driver to List Processes
### My project so far
- So far, using the example from our class page as a basis for my initial attempt at making a character device driver, I am able to build and run this simple module. 

- Additionally, taking the advice in the assignment instructions, I have looked into the `for_each_process` macro, and the   `task` struct. Using both, I was able to demonstrate a simple loop that prints task/process information to the console via the `KERN_ALERT` macro.

### TODO
- [ ] Find specific information requested about the process, some of which I already have
- [ ] Implement the read and write interface in the driver, writing the code for each
- [ ] Research and implement open of multiple simultaneous file descriptors
- [ ] Error handling
- [ ] Implement user space program
