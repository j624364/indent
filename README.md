# indent

This program just indents text from either a file or from standard input.

## Building

Currently only works on *nix. No external dependencies other than a C compiler and GNU make.

To build, run `make`
To install, run `make install`. You may need to type a password.

## Running

It will automatically read from stdin unless a file has been specified. It takes the following command line args:

| Option            | Use                                                              | Default Value |
| ----------------- | ---------------------------------------------------------------- | ------------- |
| --level or -l     | Sets the number of characters to indent by.                      | 4             |
| --character or -c | Sets the string to indent with.                                  | " "           |
| valid file        | Prints the file with the indent options specified at that point. | N/A           |

Any other option will print a help message. If the file does not exist, it will be assumed to be an unknown option and will print the help message also.
