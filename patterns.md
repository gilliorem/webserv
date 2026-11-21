# PATTERNS

logic or programming patterns discovered while working on webserv.

Logs of the logical errors | assumptions made about concepts
what I did wrong
where was my understanding faulty
the fix
the general rule out of this.


- Have to split word by word
Assumption:
it helps us ignore the whitespaces

## Process a string word by word
`std::string strToBeProcessed`
Key line:
`std::istringstream iss(strToBeProcessed)`: constructs an 'input string stream' object and *maintains an internal read cursor (position pointe)* 
this send the `strToBeProcessed` into the stream buffer (input)

`std::string word` individual temporary word (as we will process the input)
the reading/processing loop:
`while (iss >> word)
	// do something with word
`
Every time `iss >> word` executes:
- `iss` advances the read cursor until the next whitespace
- `iss` extracts every char into `word` until the next whitespace
- `iss` remember where it stops for later `is >> word` iteration.


## Constructor and static



## Handle error by returning infomation to the caller: `Expected` (`std::expected`)

A classic parser will return a boolean value to indicate the parsing fail or succeed.
in the parser there are multiples ways the checks can fail. With a strict type `bool`, we miss the reason it failed.

To return both our object and the error detailed we need a class that carries 2 information:
```c++
template <typename T>
class Expected
{
    T value;
    bool hasValue = false;
    std::string message = "";
    
    Expected(const std::string& msg)
}
```
