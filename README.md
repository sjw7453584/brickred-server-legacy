brickred-server
===============
This is the brickred studio game server library.
It implemented basic functionality which a c++ game server needed,
such as network, timer, multithread, log util and so on.

Platform support
----------------
only linux (IOService use epoll), maybe port to other platforms in the future

Dependency
----------
compatiable with c++03, no external dependency

Compile
-------
you need to install gmake and g++, then just

    $ make release

Compatiable
-----------
On the system do not have *epoll_create1*, please edit the mak/libbrickred.mak, add the line:

    CPP_FLAG = -DBRICKRED_DONT_HAVE_EPOLL_CREATE1

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

