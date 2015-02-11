# C++ Docker Client

[![Build Status](https://webapi.biicode.com/v1/badges/lasote/lasote/docker_client/master)](https://www.biicode.com/lasote/docker_client) 

![lasote/docker_client](http://blog.biicode.com/wp-content/uploads/sites/2/2015/01/biicode-cplus-docker.jpg)

[Docker Rest API v1.16](https://docs.docker.com/reference/api/docker_remote_api_v1.16/) implementation with C++11 using lambda functions for callbacks.

This library is hosted in **[Biicode](http://www.biicode.com) C++ dependency manager**.

Biicode block [lasote/docker_client](http://www.biicode.com/lasote/docker_client)

Also in [github repository https://github.com/lasote/docker_client](https://github.com/lasote/docker_client)

### Build status

*Visual Studio 2012:* [![Build status](https://ci.appveyor.com/api/projects/status/v9ty8y8xusmmptj3?svg=true)](https://ci.appveyor.com/project/lasote/docker-client)

*Linux gcc:* [![Build Status](https://travis-ci.org/lasote/docker_client.svg?branch=master)](https://travis-ci.org/lasote/docker_client)

Also working with: **Windows** with **MinGW** >=4.8, **OSx** with **Clang** > 6.0 

### Want to try it?

The project has many dependencies, we recommend you to use [biicode](http://www.biicode.com) to handle them:

[Get started with biicode](http://docs.biicode.com/c++/gettingstarted.html)

Include this header in your source code file:

    #include "lasote/docker_client/client.h"

Download the required dependencies:

    bii find

Build the project:

    bii cpp:build # to build the project

Take a look to the example: http://www.biicode.com/examples/docker_client


### How to use it


	DockerClient client("http://localhost:4243");

	// Error callback for all examples
	ERR_F error_cb = [] (int status, string desc) {
	  cout << "Error: " << status <<  endl  << desc;
	};

	auto c5 = client.logs_container([] (string out) {
      cout << "Response: " << out << endl;
	}, error_cb, "c7ddced66641", true, true, true, true, "all");


	auto c6 = client.list_containers([] ( jsonxx::Object ret) {
	  cout << "Containers: " << ret.json() << endl;
	}, error_cb, false, 13);

	auto c7 = client.list_images([] ( jsonxx::Object ret) {
	  cout << "Images: " << ret.json() << endl;
	}, error_cb); 

	// Its based on libuv event loop, so, run all the requests
	run_loop();

