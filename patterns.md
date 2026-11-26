# PATTERNS

logic or programming patterns discovered while working on webserv.

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

## Split line by line

`std::string dataToProcess`
`std::string line`
using `std::ifstream file` or `std::string str`
whether it's a file or a string, we need to pipe it in a stream so that getline can process it.
`std::istringstream stream(dataToProcess)`
`while std::getline(stream, line)` this version of getline does not include the newline at the end. (the C POSIX `getline` version does.)

## The `Expected` class
The Expected class has a private constructor because it is its member functions that will instantiate the class. this is the constructor signature:
`Expected(const T& v, const std::string& msg, bool hasValue)`
The Expected constructor will construct a T. The first arugment has to be a const referene to that T v object in order to able to construct it and have it as a reference like the message. 
Later the sucess function will construct as follow:
`static Expected success(const T& v) { return Expected(v, "", true); }`
`static Expected fail(const std::string& errorMsg) { return Expected(T(), errorMsg, false); }`

parser call:
```c++
Expected parser()
{
	lines = getlines();
	if (lines.size() < 3)
		return Expected::fail("Error: conf file missing line")
	
	findColon = std::string::find.lines[1](":");
	port = std::atoi(lines[1].substr(findColon));	
	return Expected::success(Conf(port, lines[2]))
}
```
## Having getters in the Expected class
I first thought that having getters in the Expected class was useless as we are setting the values in the member functions `success()` and `fail()`. WRONG.
We need getters for our caller: the `main()` will need to check if the Expected values are correct, and it will need the port, the location.

main call:
```c++
Expected<Conf> res parse(argv[1]);
if (!res.success())
{
	std::cerr << res.error();
	return 1;
}
const Config& cfg = res.value();
std::cout << "Port: " << cfg.getPort() << "Location: " << cfg.getLocation() << std::endl;
```
Another main call:
```c++
auto res = ConfigParser::parse("server.conf") -> Expected<Conf>;
if (!res.is_success()) {
    std::cerr << "Error parsing configuration: " << res.error() << std::endl;
    return 1;
}
auto config = res.value() -> Conf;
```

## THE BARE MINIMUM CONFIG TO RUN A WEBSERV
For a server to run it just needs two elements (lines):
`listen 80` the port to listen to.
`root /var/www/` : the base filestystem

Without a port, the OS can't create the listening socket.
Without a base filesystem, the program doesnt not know what file read


## THE COMPILE TIME RUN TIME ANALOGY
Parsing the config file can be seen as the compile time.
Starting the server loop using Config attributes can be seen as the runtime.
Startup / Configuration Phase: The program starts, reads webserv.conf, parses text into C++ objects, validates syntax, and builds your Config class. 

Runtime Phase (Runs forever until Ctrl+C): Once the configuration is loaded, the program enters its main while (running) loop. During this phase, the program reacts to live external events: new TCP connections, incoming HTTP bytes, CGI processes finishing, and socket write-readiness.

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
