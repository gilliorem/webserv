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


## Making a constructor private


## Class member function calling the constructor

```c++
auto res = ConfigParser::parse("server.conf");
if (!res.is_success()) {
    std::cerr << "Error parsing configuration: " << res.error() << std::endl;
    return 1;
}
auto config = res.value();
```

the statement above implies that we have an `Expected` class

## Handle error by returning infomation to the caller: `Expected` (`std::expected`)

A classic parser will return a boolean value to indicate the parsing fail or succeed.
in the parser there are multiples ways the checks can fail. With a strict type `bool`, we miss the reason it failed.

To return both our object and the error detailed we need a class that carries 2 information:
```c++
template <typename T>
class Expected
{
    private:
    T value;
    bool hasValue = false;
    std::string message = "";
    
    public:
    Expected(const std::string& msg) {}
}
```

## Expected having a private Constructor
as the object will be instantiate on `Expected::fail()` or `Expected::success()` calls

## Expected having static methods that instantiate the class
*In fail case, we instantiate the object Expected and we pass the literal error message*
*In success case, we instantiate the Expected object and we pass an empty string error and the Value must be the Config instance*

```c++
static Expected success()
{
    return Expected:
}
static Expected failure () 
```


## Git Version control

Initial scenario: (Happening on PC)
I've written some changes in `conf_file_parser.cpp` in my PC. I *didn't push*.
I've written some changes in `conf_file_parser.cpp` in my MAC. I *pushed*.
I've realized that the version of the file that I want is the *PC* .

==> I have a *conflict* *I can't push nor pull from PC*
I can't `git pull` because I have uncomitted local changes that would be overwritten by origin/main (upstream branch(github))
I can't `git push` because the origin/main has a newer commit that the local PC doesn't have.

I need to commit all the uncommited files, so *git won't block the merge*
`git add -A && git commit -m "Save PC changes"` 
I run `git fetch origin` : (fetch will download the `parser.cpp` present in origin/main in the `.git`): *preserving the local files*
I will need to merge (go over each file version(MAC|PC)) and chose what change I want to go with.
Once I've made those changes, I can now merge: keep one version locally of the `conf_file_parser.cpp` (at this point, local and upstream is the same version)

There are two other files that I've made modifications on my MAC I do want to pull from the orgin/main `.gitignore` and `patterns.md`
I want to merge the files that I havent modified locally and keep the PC version for the file that has been modified both in origin and locally:
`git merge`:
solve conflict (modifying the temp file)

`git checkout HEAD -- conf_file_parser.cpp` is for us to keep only the local part and whipeout the mac changes.

`finally we need to commit push`
