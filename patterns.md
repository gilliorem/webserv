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

