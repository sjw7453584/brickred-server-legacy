brickred-server
===============
This is the brickred studio game server core library.
It implemented basic functionality which a c++ game server needed.

Features
========
* reactor network io
* multithread support
* min-heap based timer
* async log util
* http protocol support
* websocket protocol support
* random number generator
* command-line option parser

Platform support
----------------
only linux, maybe port to other platforms (unix likes) in the future

Dependency
----------
compatiable with c++03
* libbrickredcore is no external dependency

Compile
-------
you need to install gmake and g++, then just:
```
$ config.sh --prefix=<prefix>
$ make && make install
```

if you want build test programmes add option:
```
$ config.sh --prefix=<prefix> --build-test
```
after compile you can find them in bin dir

Example
-------
in the dir src/test, there are some examples to show how to use the library.

Licence
-------
Copyright Kaien Chen(kaienkira@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
